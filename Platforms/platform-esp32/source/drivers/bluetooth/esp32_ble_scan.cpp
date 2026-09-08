#ifdef ESP_PLATFORM
#include <sdkconfig.h>
#endif

#if defined(CONFIG_BT_NIMBLE_ENABLED)

#include <bluetooth/esp32_ble_internal.h>

#include <host/ble_gap.h>
#include <host/ble_hs_mbuf.h>

#include <algorithm>
#include <cstring>

constexpr auto* TAG = "esp32_ble_scan";
#include <tactility/log.h>

// ---- Module-static scan context ----
// Set at the start of ble_resolve_next_unnamed_peer; valid for the duration of
// the sequential resolution chain (single-device, single-scan at a time).
// Using BleCtx* (not Device*) so we avoid keeping a static Device reference.
static BleCtx* s_scan_ctx = nullptr;

// Cap how many unnamed peers we connect to in a single name-resolution chain. A pathological,
// extremely dense RF environment (many dozens of BLE devices) would otherwise connect to every one
// of them one at a time, keeping the radio/CPU saturated and churning GATT buffers for a long
// time — which on a memory-tight device can drive it toward an OOM/assert crash. Bounding the
// chain keeps the scan responsive and the peak allocation bounded while still resolving names for
// every realistic environment (the driver's own scan_results[] is capped at 64, so 60 lets a full
// scan resolve names for essentially every device; only >60 unnameable devices trip the bound).
static constexpr size_t BLE_NAME_RESOLVE_MAX = 60;
static size_t s_name_resolve_count = 0;

// ---- Persistent device-name cache ----
// Once a peer's name is resolved via GATT, remember it here so later scans can fill it in from the
// cached copy instead of reconnecting to the device each time. This avoids re-spending the connect
// + read-by-uuid + terminate cycle (and its transient GATT/connection buffers) for devices whose
// name we already know, which keeps repeated scans fast and the peak memory bounded. The cache is
// small and fixed-size; it persists across scans and app launches.
static constexpr size_t NAME_CACHE_MAX = 64;
static constexpr size_t CACHE_NAME_MAX = 64; // most BLE names fit well under this; keeps the table small
struct CachedName {
    ble_addr_t addr;
    char name[CACHE_NAME_MAX + 1];
};
static CachedName s_name_cache[NAME_CACHE_MAX];
static size_t s_name_cache_count = 0;

static const char* cache_lookup_name(const ble_addr_t* addr) {
    for (size_t i = 0; i < s_name_cache_count; ++i) {
        if (s_name_cache[i].addr.type == addr->type &&
            memcmp(s_name_cache[i].addr.val, addr->val, 6) == 0) {
            return s_name_cache[i].name;
        }
    }
    return nullptr;
}

static void cache_store_name(const ble_addr_t* addr, const char* name) {
    if (name == nullptr || name[0] == '\0') {
        return;
    }
    for (size_t i = 0; i < s_name_cache_count; ++i) {
        if (s_name_cache[i].addr.type == addr->type &&
            memcmp(s_name_cache[i].addr.val, addr->val, 6) == 0) {
            strncpy(s_name_cache[i].name, name, CACHE_NAME_MAX);
            s_name_cache[i].name[CACHE_NAME_MAX] = '\0';
            return;
        }
    }
    if (s_name_cache_count < NAME_CACHE_MAX) {
        s_name_cache[s_name_cache_count].addr = *addr;
        strncpy(s_name_cache[s_name_cache_count].name, name, CACHE_NAME_MAX);
        s_name_cache[s_name_cache_count].name[CACHE_NAME_MAX] = '\0';
        s_name_cache_count++;
    }
}

// ---- Scan data helpers ----

void ble_scan_clear_results(struct Device* device) {
    BleCtx* ctx = ble_get_ctx(device);
    xSemaphoreTake(ctx->scan_mutex, portMAX_DELAY);
    ctx->scan_count = 0;
    memset(ctx->scan_results, 0, sizeof(ctx->scan_results));
    xSemaphoreGive(ctx->scan_mutex);
}

// ---- GAP scan callback ----

int ble_gap_disc_event_handler(struct ble_gap_event* event, void* arg) {
    struct Device* device = (struct Device*)arg;
    BleCtx* ctx = ble_get_ctx(device);

    switch (event->type) {
        case BLE_GAP_EVENT_DISC: {
            const auto& disc = event->disc;

            BtPeerRecord record = {};
            memcpy(record.addr, disc.addr.val, BT_ADDR_LEN);
            record.addr_type = disc.addr.type;
            record.rssi      = disc.rssi;
            record.paired    = false;
            record.connected = false;

            // Advertising PDU type / connectability (raw HCI -> normalized BtAdvType).
            switch (disc.event_type) {
                case BLE_HCI_ADV_RPT_EVTYPE_ADV_IND:     record.adv_type = BT_ADV_TYPE_IND;      break;
                case BLE_HCI_ADV_RPT_EVTYPE_DIR_IND:     record.adv_type = BT_ADV_TYPE_DIRECT;    break;
                case BLE_HCI_ADV_RPT_EVTYPE_SCAN_IND:    record.adv_type = BT_ADV_TYPE_SCAN_IND;  break;
                case BLE_HCI_ADV_RPT_EVTYPE_NONCONN_IND: record.adv_type = BT_ADV_TYPE_NONCONN;   break;
                case BLE_HCI_ADV_RPT_EVTYPE_SCAN_RSP:    record.adv_type = BT_ADV_TYPE_SCAN_RSP;  break;
                default:                                  record.adv_type = BT_ADV_TYPE_UNKNOWN;   break;
            }
            record.tx_power = 0x7F; // "not present" sentinel

            struct ble_hs_adv_fields fields;
            if (ble_hs_adv_parse_fields(&fields, disc.data, disc.length_data) == 0) {
                if (fields.name != nullptr && fields.name_len > 0) {
                    size_t copy_len = std::min<size_t>(fields.name_len, BT_NAME_MAX);
                    memcpy(record.name, fields.name, copy_len);
                    record.name[copy_len] = '\0';
                } else if (const char* cached = cache_lookup_name(&disc.addr)) {
                    // Already resolved in a prior scan — use the cached name so name-resolution
                    // skips this peer and we avoid reconnecting to it.
                    size_t copy_len = std::min<size_t>(strlen(cached), BT_NAME_MAX);
                    memcpy(record.name, cached, copy_len);
                    record.name[copy_len] = '\0';
                }
                // Advertising flags (flags field is always present in the struct; 0 if absent).
                record.adv_flags = fields.flags;
                if (fields.tx_pwr_lvl_is_present) {
                    record.tx_power = fields.tx_pwr_lvl;
                }
                // Capture manufacturer-specific data (company ID + payload) so consumers such as
                // the BLE Toolbox AirTag monitor can classify Apple 0x004C devices by their
                // 0x12 offline-finding ("Nearby Info") advertisement type.
                if (fields.mfg_data != nullptr && fields.mfg_data_len > 0) {
                    size_t mlen = std::min<size_t>(fields.mfg_data_len, sizeof(record.manuf_data));
                    memcpy(record.manuf_data, fields.mfg_data, mlen);
                    record.manuf_len = static_cast<uint8_t>(mlen);
                }
                // 16-bit Service Data AD (type 0x16) — Eddystone (0xFEAA) and other service-data
                // beacons/telemetry. The field carries [2-byte UUID LE][payload].
                if (fields.svc_data_uuid16 != nullptr && fields.svc_data_uuid16_len >= 2) {
                    record.svc_uuid16 = static_cast<uint16_t>(fields.svc_data_uuid16[0] |
                                                              (fields.svc_data_uuid16[1] << 8));
                    uint8_t payload_len = static_cast<uint8_t>(fields.svc_data_uuid16_len - 2);
                    size_t copy_len = std::min<size_t>(payload_len, sizeof(record.svc_data));
                    memcpy(record.svc_data, fields.svc_data_uuid16 + 2, copy_len);
                    record.svc_data_len = static_cast<uint8_t>(copy_len);
                }
            }

            // Keep the raw advertising payload so a sniffer/observer can render hex and decode
            // fields that aren't surfaced as named members above.
            record.adv_len = std::min<uint8_t>(disc.length_data, sizeof(record.adv_data));
            if (record.adv_len > 0) {
                memcpy(record.adv_data, disc.data, record.adv_len);
            }

            {
                xSemaphoreTake(ctx->scan_mutex, portMAX_DELAY);
                bool found = false;
                for (size_t i = 0; i < ctx->scan_count; ++i) {
                    if (memcmp(ctx->scan_results[i].addr, record.addr, BT_ADDR_LEN) == 0) {
                        // Deduplicate: merge name from SCAN_RSP without clobbering ADV_IND name
                        if (record.name[0] != '\0') {
                            memcpy(ctx->scan_results[i].name, record.name, BT_NAME_MAX + 1);
                        }
                        ctx->scan_results[i].rssi = record.rssi;
                        found = true;
                        break;
                    }
                }
                if (!found && ctx->scan_count < 64) {
                    ctx->scan_results[ctx->scan_count] = record;
                    ctx->scan_addrs[ctx->scan_count]   = disc.addr; // full addr (type+val)
                    ctx->scan_count++;
                }
                xSemaphoreGive(ctx->scan_mutex);
            }

            struct BtEvent e = {};
            e.type = BT_EVENT_PEER_FOUND;
            e.peer  = record;
            ble_publish_event(device, e);
            break;
        }

        case BLE_GAP_EVENT_DISC_COMPLETE:
            LOG_I(TAG, "Scan complete (reason=%d)", event->disc_complete.reason);
            if (ctx->scan_resolve_names.load()) {
                // Keep scan_active=true; resolveNextUnnamedPeer clears it and fires ScanFinished
                // once name resolution finishes, so the UI spinner stays active throughout.
                ble_resolve_next_unnamed_peer(device, 0);
            } else {
                // Observer/sniffer mode: stay non-intrusive, finish immediately without connecting.
                ble_set_scan_active(device, false);
                struct BtEvent e = {};
                e.type = BT_EVENT_SCAN_FINISHED;
                ble_publish_event(device, e);
            }
            break;

        default:
            break;
    }
    return 0;
}

// ---- GATT Device Name resolution ----
//
// After a scan completes, briefly connect to each device that didn't include its
// name in advertising data and read Generic Access Device Name (UUID 0x2A00).
//
// Resolution is sequential: connect → read → disconnect → next device.
// Skip resolution if a profile server or HID host connection is active —
// a simultaneous central connection would fail with BLE_HS_EALREADY.

static int name_read_callback(uint16_t conn_handle, const struct ble_gatt_error* error,
                              struct ble_gatt_attr* attr, void* arg) {
    BleCtx* ctx = s_scan_ctx;

    if (error->status == 0 && attr != nullptr) {
        size_t idx = (size_t)(uintptr_t)arg;
        uint16_t len = OS_MBUF_PKTLEN(attr->om);
        if (len > 0 && len <= (uint16_t)BT_NAME_MAX) {
            char name_buf[BT_NAME_MAX + 1] = {};
            os_mbuf_copydata(attr->om, 0, len, name_buf);
            {
                xSemaphoreTake(ctx->scan_mutex, portMAX_DELAY);
                if (idx < ctx->scan_count && ctx->scan_results[idx].name[0] == '\0') {
                    memcpy(ctx->scan_results[idx].name, name_buf, len);
                    ctx->scan_results[idx].name[len] = '\0';
                    LOG_I(TAG, "Name resolved (idx=%u): %s", (unsigned)idx, name_buf);
                }
                // Remember it so a later scan fills the name from the cache instead of reconnecting.
                if (idx < ctx->scan_count) {
                    cache_store_name(&ctx->scan_addrs[idx], name_buf);
                }
                BtPeerRecord record = (idx < ctx->scan_count) ? ctx->scan_results[idx] : BtPeerRecord{};
                xSemaphoreGive(ctx->scan_mutex);

                struct BtEvent e = {};
                e.type = BT_EVENT_PEER_FOUND;
                e.peer  = record;
                ble_publish_event(ctx->device, e);
            }
        }
        return 0; // wait for BLE_HS_EDONE
    }

    // BLE_HS_EDONE, ATT error, or timeout — done with this device
    ble_gap_terminate(conn_handle, BLE_ERR_REM_USER_CONN_TERM);
    return 0;
}

static int name_res_gap_callback(struct ble_gap_event* event, void* arg) {
    size_t idx = (size_t)(uintptr_t)arg;
    BleCtx* ctx = s_scan_ctx;

    switch (event->type) {
        case BLE_GAP_EVENT_CONNECT:
            if (event->connect.status == 0) {
                LOG_I(TAG, "Name resolution: connected (idx=%u handle=%u)", (unsigned)idx, event->connect.conn_handle);
                static const ble_uuid16_t device_name_uuid = BLE_UUID16_INIT(0x2A00);
                int rc = ble_gattc_read_by_uuid(event->connect.conn_handle,
                                                1, 0xFFFF,
                                                &device_name_uuid.u,
                                                name_read_callback, arg);
                if (rc != 0) {
                    LOG_W(TAG, "Name resolution: read_by_uuid failed rc=%d", rc);
                    ble_gap_terminate(event->connect.conn_handle, BLE_ERR_REM_USER_CONN_TERM);
                }
            } else {
                LOG_I(TAG, "Name resolution: connect failed (idx=%u status=%d)", (unsigned)idx, event->connect.status);
                ble_resolve_next_unnamed_peer(ctx->device, idx + 1);
            }
            break;

        case BLE_GAP_EVENT_DISCONNECT:
            LOG_I(TAG, "Name resolution: disconnected (idx=%u)", (unsigned)idx);
            ble_resolve_next_unnamed_peer(ctx->device, idx + 1);
            break;

        default:
            break;
    }
    return 0;
}

void ble_resolve_next_unnamed_peer(struct Device* device, size_t start_idx) {
    BleCtx* ctx = ble_get_ctx(device);
    s_scan_ctx = ctx;

    // Fresh chain (called with 0 at scan completion): reset the cap counter. Re-entrant calls
    // (on disconnect / connect-fail) pass the next index, which is always > 0.
    if (start_idx == 0) {
        s_name_resolve_count = 0;
    }

    // Skip if a profile server or HID host connection attempt is active —
    // initiating a central connection simultaneously would fail (BLE_HS_EALREADY).
    if (ble_midi_get_active(device) || ble_spp_get_active(device) ||
        ble_hid_get_active(device)  || ble_hid_get_host_active(device)) {
        LOG_I(TAG, "Name resolution: skipping (server or HID host active)");
        ble_set_scan_active(device, false);
        struct BtEvent e = {};
        e.type = BT_EVENT_SCAN_FINISHED;
        ble_publish_event(device, e);
        return;
    }

    size_t i = start_idx;
    while (true) {
        ble_addr_t addr     = {};
        bool       found    = false;
        bool       radio_on = false;
        int        rc       = -1;

        // Don't start (or chain into) a new GAP connection once the radio is going down
        // (or is already off) — ble_gap_connect() racing nimble_port_stop() can block the
        // NimBLE host task and hang the stop, same class of bug as the OFF_PENDING guard
        // on advertising restart in gap_event_handler's BLE_GAP_EVENT_DISCONNECT case.
        // The check must be re-done on every iteration (not just once on entry) since a
        // failed ble_gap_connect() loops back for the next peer, and it must happen under
        // scan_mutex together with the connect call itself so a concurrent dispatch_disable()
        // can't flip radio_state between the check and the call.
        xSemaphoreTake(ctx->scan_mutex, portMAX_DELAY);
        radio_on = ctx->radio_state.load() == BT_RADIO_STATE_ON;
        if (radio_on) {
            while (i < ctx->scan_count) {
                if (ctx->scan_results[i].name[0] == '\0') {
                    addr  = ctx->scan_addrs[i];
                    found = true;
                    break;
                }
                ++i;
            }
            if (found) {
                uint8_t own_addr_type;
                ble_hs_id_infer_auto(0, &own_addr_type);
                void* idx_arg = (void*)(uintptr_t)i;
                rc = ble_gap_connect(own_addr_type, &addr, 1500, nullptr,
                                     name_res_gap_callback, idx_arg);
            }
        }
        xSemaphoreGive(ctx->scan_mutex);

        // Stop the chain once the cap is reached (checked after releasing the mutex so we never
        // return while holding it). Devices past the cap keep their address-only row.
        if (found && s_name_resolve_count >= BLE_NAME_RESOLVE_MAX) {
            LOG_I(TAG, "Name resolution: capped at %u devices", (unsigned)BLE_NAME_RESOLVE_MAX);
            ble_set_scan_active(device, false);
            struct BtEvent e = {};
            e.type = BT_EVENT_SCAN_FINISHED;
            ble_publish_event(device, e);
            return;
        }

        if (!radio_on) {
            LOG_I(TAG, "Name resolution: aborting (radio not on)");
            ble_set_scan_active(device, false);
            struct BtEvent e = {};
            e.type = BT_EVENT_SCAN_FINISHED;
            ble_publish_event(device, e);
            return;
        }

        if (!found) {
            LOG_I(TAG, "Name resolution: complete (%u devices)", (unsigned)i);
            ble_set_scan_active(device, false);
            struct BtEvent e = {};
            e.type = BT_EVENT_SCAN_FINISHED;
            ble_publish_event(device, e);
            return;
        }

        if (rc == 0) {
            s_name_resolve_count++;
            return; // name_res_gap_callback continues the chain
        }

        LOG_I(TAG, "Name resolution: ble_gap_connect failed idx=%u rc=%d, skipping", (unsigned)i, rc);
        ++i;
    }
}

#endif // CONFIG_BT_NIMBLE_ENABLED
