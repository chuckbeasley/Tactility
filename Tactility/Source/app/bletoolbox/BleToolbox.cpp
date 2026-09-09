#include <sdkconfig.h>

#include <Tactility/Tactility.h>
#include <Tactility/Timer.h>
#include <Tactility/bluetooth/Bluetooth.h>

#include <app/event.h>
#include <app/manager.h>
#include <app/start.h>
#include <app/manifest.h>
#include <app/scheduler.h>

#include <lvgl_window_manager/window_manager.h>

#include <lvgl/lvgl.h>
#include <lvgl/widgets/toolbar.h>

#include <tactility/concurrent/task_event_group.h>
#include <tactility/device.h>
#include <tactility/drivers/bluetooth.h>
#include <tactility/log.h>
#include <tactility/time.h>

#include <array>
#include <cstdio>
#include <cstring>
#include <deque>
#include <format>
#include <memory>
#include <string>
#include <vector>

#include "BleSpamPayloads.h"

namespace tt::app::bletoolbox {

constexpr auto* TAG = "BleToolbox";

extern const ::AppManifest manifest;

namespace {

enum class Screen { Main, Scan, Spam, Airtag, Observer };

// AirTag / Apple "Find My" device advertisement signature (from the offline-finding protocol,
// see "Who Can Find My Devices?", PETS 2021): manufacturer data with Apple company ID 0x004C,
// then type 0x12 (Nearby Info / offline finding) and data length 0x19 (25).
constexpr uint8_t APPLE_COMPANY_LO = 0x4C;
constexpr uint8_t APPLE_COMPANY_HI = 0x00;
constexpr uint8_t AIRTAG_OF_TYPE = 0x12;

constexpr uint32_t POLL_INTERVAL_MS = 300;
constexpr uint32_t SPAM_INTERVAL_MS = 200;
constexpr size_t MAX_SCAN_RESULTS = 64;

// BLE Observer: a passive/active advertising sniffer. `filter_duplicates = 0` means the radio
// reports every advertising packet (not just one per address), so a beacon whose telemetry changes
// between broadcasts (e.g. an Eddystone TLM battery/temperature frame) is captured on every
// report. We cap the in-memory log so a dense RF environment can't exhaust the heap.
constexpr size_t OBS_LOG_MAX = 200;
// Aggressive-but-sane scan window so the observer catches most broadcasts without saturating the
// controller. Units are NimBLE scan intervals (0.625 ms): 0x0040 => 40 ms.
constexpr uint16_t OBS_SCAN_ITVL = 0x0040;
constexpr uint16_t OBS_SCAN_WINDOW = 0x0030;

// A device discovered during scan/monitor. `manuf` carries the advertisement manufacturer bytes.
struct Peer {
    std::array<uint8_t, 6> addr;
    char name[BT_NAME_MAX + 1];
    int8_t rssi;
    uint8_t addr_type;
    uint8_t manuf[32];
    uint8_t manuf_len;
    uint32_t seenCount;
};

// One raw advertising report captured by the BLE Observer (a "sniffer" frame). Unlike the scan
// tables, these are NOT de-duplicated by address, so a busy beacon fills the log with every packet.
struct ObsEntry {
    std::array<uint8_t, 6> addr;
    uint8_t addr_type;
    int8_t rssi;
    uint8_t adv_type;
    uint8_t manuf[32];
    uint8_t manuf_len;
    uint16_t svc_uuid16;
    uint8_t svc_data[24];
    uint8_t svc_data_len;
    uint8_t adv_data[31];
    uint8_t adv_len;
    char name[BT_NAME_MAX + 1];
    // Milliseconds since the observer started (for a per-frame timestamp).
    uint32_t uptimeMs;
};

struct Context {
    uint32_t appInstanceId = 0;
    Screen screen = Screen::Main;

    Device* dev = nullptr;
    BtEventSubscription btSub {};

    // Scan (all devices) + AirTag monitor share the same scan subscription.
    bool scanRunning = false;
    bool airtagRunning = false;
    bool spamRunning = false;
    // BLE Observer (advertising sniffer). Uses the same scan subscription but with a raw,
    // non-de-duplicated, passive scan, and does NOT do GATT name resolution.
    bool obsRunning = false;
    uint32_t obsStartMs = 0;
    size_t currentPayload = 0;
    bool randomizeAddress = true;
    // Which spoof families are on (Apple, Android, Windows, Samsung). All on by default. Written
    // by the LVGL task (family switch handler), read by the spam timer task - benign one-tick race.
    bool enabled[kFamilyCount] = { true, true, true, true };

    std::vector<Peer> scanPeers;
    std::vector<Peer> airtagPeers;
    uint32_t scanCount = 0;

    std::deque<ObsEntry> obsEntries;

    std::unique_ptr<Timer> pollTimer;
    std::unique_ptr<Timer> spamTimer;

    // UI widgets.
    lv_obj_t* body = nullptr;
    lv_obj_t* statusLabel = nullptr;
    lv_obj_t* countLabel = nullptr;
    lv_obj_t* list = nullptr;
    lv_obj_t* startButtonLabel = nullptr;
    lv_obj_t* spamLabel = nullptr;
    lv_obj_t* obsLogLabel = nullptr;

    bool uiDirty = false;
    bool listDirty = false;
    bool obsDirty = false;
};

// ---- Helpers ----

static void formatAddr(const uint8_t addr[6], char* out, size_t outLen) {
    std::snprintf(out, outLen, "%02x:%02x:%02x:%02x:%02x:%02x",
        addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);
}

// Apple offline-finding ("Nearby Info", 0x12) devices — i.e. AirTag / Find My trackers.
static bool isOfflineFinding(const uint8_t* manuf, uint8_t manufLen) {
    return manufLen >= 4 &&
           manuf[0] == APPLE_COMPANY_LO && manuf[1] == APPLE_COMPANY_HI &&
           manuf[2] == AIRTAG_OF_TYPE;
}

// Enables the BT radio if not already on. Null-safe: a missing device must not be dereferenced.
static void ensureBluetoothOn(Device* dev) {
    if (dev == nullptr) {
        LOG_W(TAG, "No Bluetooth device available");
        return;
    }
    if (!bluetooth::isRadioOnOrPending(dev)) {
        LOG_I(TAG, "Enabling Bluetooth radio");
        bluetooth::start(dev);
    }
}

// ---- Event-driven list update (called from the app task, which owns LVGL) ----

// @return true if the peer list content changed (a new device was added, or a name/RSSI update
// actually changed an existing entry). Callers only trigger a (LVGL-locked) list rebuild on real
// changes, so a burst of same-device events doesn't hammer the widget tree.
static bool updatePeers(Peer* peer, std::vector<Peer>& peers) {
    for (auto& existing : peers) {
        if (existing.addr == peer->addr) {
            bool changed = existing.rssi != peer->rssi;
            existing.rssi = peer->rssi;
            existing.seenCount++;
            if (peer->name[0] != '\0') {
                if (std::strcmp(existing.name, peer->name) != 0) {
                    std::strncpy(existing.name, peer->name, BT_NAME_MAX);
                    existing.name[BT_NAME_MAX] = '\0';
                    changed = true;
                }
            }
            return changed;
        }
    }
    if (peers.size() < MAX_SCAN_RESULTS) {
        peers.push_back(*peer);
        return true;
    }
    return false;
}

static const char* deviceNameOrType(const Peer& peer) {
    if (peer.name[0] != '\0') {
        return peer.name;
    }
    if (isOfflineFinding(peer.manuf, peer.manuf_len)) {
        return "AirTag";
    }
    return "-";
}

// ---- BLE Observer (advertising sniffer) helpers ----

static const char* advTypeToString(uint8_t type) {
    switch (type) {
        case BT_ADV_TYPE_IND:      return "CONN ";   // connectable undirected
        case BT_ADV_TYPE_SCAN_IND: return "SCANN";   // scannable undirected
        case BT_ADV_TYPE_NONCONN:  return "NCONN";   // non-connectable beacon
        case BT_ADV_TYPE_DIRECT:   return "DIR  ";   // connectable directed
        case BT_ADV_TYPE_SCAN_RSP: return "RSP  ";   // scan response
        default:                   return "?    ";
    }
}

static std::string hexShort(const uint8_t* data, uint8_t len) {
    std::string out;
    out.reserve(static_cast<size_t>(len) * 2);
    for (uint8_t i = 0; i < len; ++i) {
        char b[3];
        std::snprintf(b, sizeof(b), "%02x", data[i]);
        out += b;
    }
    return out;
}

// Concise, table-cell friendly summary of an advertising frame. lv_table cells do not wrap, so keep
// it short; the Type / RSSI / Addr columns carry the rest. Full per-field decode (raw PDU hex) is
// traded for a readable, always-visible sniffer summary.
static std::string decodeObserverCell(const ObsEntry& e) {
    if (e.adv_type == BT_ADV_TYPE_DIRECT) return "directed";

    // Apple iBeacon: 0x004C, type 0x02. Layout inside manuf_data:
    // [0..1] company ID, [2] type, [3..18] proximity UUID, [19..20] major, [21..22] minor.
    if (e.manuf_len >= 24 &&
        e.manuf[0] == APPLE_COMPANY_LO && e.manuf[1] == APPLE_COMPANY_HI &&
        e.manuf[2] == 0x02) {
        uint16_t major = static_cast<uint16_t>((e.manuf[19] << 8) | e.manuf[20]);
        uint16_t minor = static_cast<uint16_t>((e.manuf[21] << 8) | e.manuf[22]);
        // Shorten the 16-byte UUID to its first 4 bytes for the narrow cell.
        return std::format("iBeacon {}.. M={} m={}", hexShort(e.manuf + 3, 4), major, minor);
    }

    // Apple offline-finding (AirTag / Find My): 0x004C + type 0x12.
    if (isOfflineFinding(e.manuf, e.manuf_len)) return "Apple FindMy";

    // Eddystone (service-data UUID 0xFEAA).
    if (e.svc_uuid16 == 0xFEAA && e.svc_data_len >= 1) {
        uint8_t frame = e.svc_data[0];
        if (frame == 0x00) return "Eddystone UID";
        if (frame == 0x10) return "Eddystone URL";
        if (frame == 0x20 && e.svc_data_len >= 12) {
            // TLM: version [1], battery mV [2..3], temperature [4..5] in 0.0625 C units (= /16).
            uint16_t batt = static_cast<uint16_t>((e.svc_data[2] << 8) | e.svc_data[3]);
            int16_t tempC = static_cast<int16_t>((e.svc_data[4] << 8) | e.svc_data[5]) / 16;
            return std::format("TLM {}mV {}C", batt, tempC);
        }
        return "Eddystone";
    }

    if (e.name[0] != '\0') return std::format("Name={}", e.name);
    return "advert";
}

// Rebuild the observer log table (Type | RSSI | Addr | Info), newest-first so the latest capture is
// at the top and visible without having to scroll to it. lv_table is a bounded, self-scrolling
// widget (same as the Scan screen's list), so the controls below stay in place.
static void rebuildObserverLog(Context* ctx) {
    lv_obj_t* table = ctx->obsLogLabel;
    if (table == nullptr) return;
    const size_t count = std::min(ctx->obsEntries.size(), OBS_LOG_MAX);
    lv_table_set_row_count(table, static_cast<uint32_t>(count + 1)); // +1 for the header row
    lv_table_set_cell_value(table, 0, 0, "Type");
    lv_table_set_cell_value(table, 0, 1, "RSSI");
    lv_table_set_cell_value(table, 0, 2, "Addr");
    lv_table_set_cell_value(table, 0, 3, "Info");
    size_t row = 1;
    for (auto it = ctx->obsEntries.rbegin(); it != ctx->obsEntries.rend() && row <= count; ++it, ++row) {
        const ObsEntry& e = *it;
        lv_table_set_cell_value(table, static_cast<uint32_t>(row), 0, advTypeToString(e.adv_type));
        const std::string rssi = std::to_string(e.rssi);
        lv_table_set_cell_value(table, static_cast<uint32_t>(row), 1, rssi.c_str());
        char addr[18];
        formatAddr(e.addr.data(), addr, sizeof(addr));
        lv_table_set_cell_value(table, static_cast<uint32_t>(row), 2, addr);
        const std::string info = decodeObserverCell(e);
        lv_table_set_cell_value(table, static_cast<uint32_t>(row), 3, info.c_str());
    }
}

// Scan parameters for the observer: raw (no duplicate filter), optionally passive, and no GATT
// name resolution (a sniffer must stay non-intrusive and not initiate central connections).
static BtScanParams observerScanParams(const Context* ctx) {
    BtScanParams p = {};
    p.passive = true; // observe-only: never send SCAN_REQ (quieter, less intrusive)
    p.filter_duplicates = true; // show each device once (like BLE Scan), not every advertising packet
    p.resolve_names = false;
    p.itvl = OBS_SCAN_ITVL;
    p.window = OBS_SCAN_WINDOW;
    return p;
}

// Start an observer scan with the configured params. observerScanParams() returns by value, so we
// bind it to a named local before taking its address (taking the address of an rvalue is ill-formed).
static void observerStartScan(Device* dev, const Context* ctx) {
    BtScanParams params = observerScanParams(ctx);
    bluetooth_scan_start_params(dev, &params);
}

// Three-column table of devices: Name | Address | RSSI.
static void rebuildList(Context* ctx) {
    if (ctx->list == nullptr) return;
    const bool airtag = (ctx->screen == Screen::Airtag);
    const auto& peers = airtag ? ctx->airtagPeers : ctx->scanPeers;
    lv_table_set_row_count(ctx->list, (uint32_t)(peers.size() + 1)); // +1 for the header row
    lv_table_set_cell_value(ctx->list, 0, 0, "Name");
    lv_table_set_cell_value(ctx->list, 0, 1, "Address");
    lv_table_set_cell_value(ctx->list, 0, 2, "RSSI");
    char addr[18];
    for (size_t i = 0; i < peers.size(); ++i) {
        const uint32_t row = (uint32_t)(i + 1);
        const Peer& peer = peers[i];
        lv_table_set_cell_value(ctx->list, row, 0, deviceNameOrType(peer));
        formatAddr(peer.addr.data(), addr, sizeof(addr));
        lv_table_set_cell_value(ctx->list, row, 1, addr);
        lv_table_set_cell_value(ctx->list, row, 2, std::to_string(peer.rssi).c_str());
    }
}

// ---- BLE event handling ----

static void onBtEvent(Context* ctx, const BtEvent& event) {
    switch (event.type) {
        case BT_EVENT_SCAN_STARTED:
            ctx->scanCount = 0;
            ctx->scanPeers.clear();
            ctx->airtagPeers.clear();
            ctx->listDirty = true;
            break;

        case BT_EVENT_PEER_FOUND: {
            Peer peer {};
            memcpy(peer.addr.data(), event.peer.addr, 6);
            peer.addr_type = event.peer.addr_type;
            peer.rssi = event.peer.rssi;
            peer.manuf_len = event.peer.manuf_len;
            if (peer.manuf_len > 0) memcpy(peer.manuf, event.peer.manuf_data, peer.manuf_len);
            if (event.peer.name[0] != '\0') {
                std::strncpy(peer.name, event.peer.name, BT_NAME_MAX);
                peer.name[BT_NAME_MAX] = '\0';
            }
            peer.seenCount = 1;
            bool changed = false;
            if (ctx->airtagRunning || ctx->scanRunning) {
                if (updatePeers(&peer, ctx->scanPeers)) changed = true;
                ctx->scanCount = static_cast<uint32_t>(ctx->scanPeers.size());
            }
            if (ctx->airtagRunning && isOfflineFinding(peer.manuf, peer.manuf_len)) {
                if (updatePeers(&peer, ctx->airtagPeers)) changed = true;
            }
            if (changed) {
                ctx->listDirty = true;
            }

            // BLE Observer: capture every advertising frame (NOT de-duplicated by address) so a
            // beacon whose telemetry changes between reports is logged on each report.
            if (ctx->obsRunning) {
                ObsEntry obs {};
                obs.uptimeMs = get_millis() - ctx->obsStartMs;
                memcpy(obs.addr.data(), event.peer.addr, 6);
                obs.addr_type = event.peer.addr_type;
                obs.rssi = event.peer.rssi;
                obs.adv_type = event.peer.adv_type;
                obs.manuf_len = event.peer.manuf_len;
                if (obs.manuf_len > 0) memcpy(obs.manuf, event.peer.manuf_data, obs.manuf_len);
                obs.svc_uuid16 = event.peer.svc_uuid16;
                obs.svc_data_len = event.peer.svc_data_len;
                if (obs.svc_data_len > 0) memcpy(obs.svc_data, event.peer.svc_data, obs.svc_data_len);
                obs.adv_len = event.peer.adv_len;
                if (obs.adv_len > 0) memcpy(obs.adv_data, event.peer.adv_data, obs.adv_len);
                if (event.peer.name[0] != '\0') {
                    std::strncpy(obs.name, event.peer.name, BT_NAME_MAX);
                    obs.name[BT_NAME_MAX] = '\0';
                }
                if (ctx->obsEntries.size() >= OBS_LOG_MAX) {
                    ctx->obsEntries.pop_front();
                }
                ctx->obsEntries.push_back(std::move(obs));
                ctx->obsDirty = true;
            }
            break;
        }

        case BT_EVENT_RADIO_STATE_CHANGED:
            // Scan can only start once the radio is actually on. If a scan/monitor was requested
            // while the radio was still turning on, kick it off now instead of firing a
            // scan_start() too early (which the driver would drop).
            if (event.radio_state == BT_RADIO_STATE_ON &&
                (ctx->scanRunning || ctx->airtagRunning || ctx->obsRunning) &&
                ctx->dev != nullptr && !bluetooth_is_scanning(ctx->dev)) {
                if (ctx->obsRunning) {
                    observerStartScan(ctx->dev, ctx);
                } else {
                    bluetooth_scan_start(ctx->dev);
                }
            }
            break;

        case BT_EVENT_SCAN_FINISHED:
            // For a continuous monitor/observer, immediately restart the next scan cycle.
            if (ctx->airtagRunning) {
                Device* dev = ctx->dev;
                if (dev != nullptr && !bluetooth_is_scanning(dev)) {
                    bluetooth_scan_start(dev);
                }
            }
            if (ctx->obsRunning) {
                Device* dev = ctx->dev;
                if (dev != nullptr && !bluetooth_is_scanning(dev)) {
                    observerStartScan(dev, ctx);
                }
            }
            ctx->uiDirty = true;
            break;

        default:
            break;
    }
    ctx->uiDirty = true;
}

// ---- Spam timer (runs on the FreeRTOS timer task; touches only BLE + atomic-ish state) ----

static void onSpamTick(Context* ctx) {
    if (!ctx->spamRunning) return;
    if (bluetooth::getRadioState() != bluetooth::RadioState::On) return;
    if (kPayloadCount == 0) return;
    // Advance to the next enabled payload, wrapping around. The radio comes on asynchronously after
    // ensureBluetoothOn() requests it, so this also no-ops until the radio is actually on.
    for (size_t i = 0; i < kPayloadCount; i++) {
        ctx->currentPayload = (ctx->currentPayload + 1) % kPayloadCount;
        const Payload& payload = kPayloads[ctx->currentPayload];
        if (ctx->enabled[payload.family]) {
            bluetooth::startAdvertising(payload.data, payload.length, false, ctx->randomizeAddress);
            return;
        }
    }
}

static void onFamilySwitchChanged(lv_event_t* event) {
    auto* sw = static_cast<lv_obj_t*>(lv_event_get_target(event));
    auto* ctx = static_cast<Context*>(lv_event_get_user_data(event));
    uintptr_t family = reinterpret_cast<uintptr_t>(lv_obj_get_user_data(sw));
    if (family < kFamilyCount) {
        ctx->enabled[family] = lv_obj_has_state(sw, LV_STATE_CHECKED);
    }
}

// ---- UI screens ----

static void showMainScreen(Context* ctx);
static void showScanScreen(Context* ctx);
static void showSpamScreen(Context* ctx);
static void showAirtagScreen(Context* ctx);
static void showObserverScreen(Context* ctx);

static void ShowScreen(Context* ctx, Screen screen) {
    ctx->screen = screen;
    if (ctx->body != nullptr) {
        lv_obj_clean(ctx->body);
        // Default body is vertically scrollable (set once in createWidgets). The Observer screen
        // disables it so its log container's flex_grow gets a real (bounded) height; restore VER
        // here so every other screen keeps its normal scroll behaviour.
        lv_obj_set_scroll_dir(ctx->body, LV_DIR_VER);
    }
    ctx->statusLabel = nullptr;
    ctx->countLabel = nullptr;
    ctx->list = nullptr;
    ctx->startButtonLabel = nullptr;
    ctx->spamLabel = nullptr;
    ctx->obsLogLabel = nullptr;

    switch (screen) {
        case Screen::Main: showMainScreen(ctx); break;
        case Screen::Scan: showScanScreen(ctx); break;
        case Screen::Spam: showSpamScreen(ctx); break;
        case Screen::Airtag: showAirtagScreen(ctx); break;
        case Screen::Observer: showObserverScreen(ctx); break;
    }
    ctx->uiDirty = true;
    ctx->listDirty = true;
}

static void stopAllActive(Context* ctx) {
#if defined(CONFIG_BT_NIMBLE_ENABLED)
    ctx->spamRunning = false;
    if (ctx->spamTimer != nullptr) ctx->spamTimer->stop();
    bluetooth::stopAdvertising();
    ctx->scanRunning = false;
    ctx->airtagRunning = false;
    ctx->obsRunning = false;
    if (ctx->dev != nullptr && bluetooth_is_scanning(ctx->dev)) {
        bluetooth_scan_stop(ctx->dev);
    }
#endif
}

static void onBackPressed(lv_event_t* event) {
    auto* ctx = static_cast<Context*>(lv_event_get_user_data(event));
    if (ctx->screen == Screen::Main) {
        stopAllActive(ctx);
        app_event_emit_close(ctx->appInstanceId);
    } else {
        stopAllActive(ctx);
        ShowScreen(ctx, Screen::Main);
    }
}

static void addMenuButton(Context* ctx, const char* text, void (*cb)(lv_event_t*)) {
    auto* btn = lv_button_create(ctx->body);
    lv_obj_set_width(btn, LV_PCT(100));
    auto* label = lv_label_create(btn);
    lv_label_set_text(label, text);
    lv_obj_center(label);
    lv_obj_add_event_cb(btn, cb, LV_EVENT_SHORT_CLICKED, ctx);
}

static void onGoScan(lv_event_t* e) { ShowScreen(static_cast<Context*>(lv_event_get_user_data(e)), Screen::Scan); }
static void onGoSpam(lv_event_t* e) { ShowScreen(static_cast<Context*>(lv_event_get_user_data(e)), Screen::Spam); }
static void onGoAirtag(lv_event_t* e) { ShowScreen(static_cast<Context*>(lv_event_get_user_data(e)), Screen::Airtag); }
static void onGoObserver(lv_event_t* e) { ShowScreen(static_cast<Context*>(lv_event_get_user_data(e)), Screen::Observer); }

// ---- Main ----

static void showMainScreen(Context* ctx) {
    addMenuButton(ctx, "BLE Scan", onGoScan);
    addMenuButton(ctx, "BLE Observer", onGoObserver);
    addMenuButton(ctx, "BLE Spam", onGoSpam);
    addMenuButton(ctx, "AirTag Monitor", onGoAirtag);
}

// ---- Scan ----

static void onStartScan(lv_event_t* event) {
    auto* ctx = static_cast<Context*>(lv_event_get_user_data(event));
#if defined(CONFIG_BT_NIMBLE_ENABLED)
    if (ctx->scanRunning || ctx->airtagRunning) {
        ctx->scanRunning = false;
        ctx->airtagRunning = false;
        Device* dev = ctx->dev;
        if (dev != nullptr && bluetooth_is_scanning(dev)) {
            bluetooth_scan_stop(dev);
        }
        ctx->uiDirty = true;
        ctx->listDirty = true;
        return;
    }
    if (ctx->dev != nullptr && !bluetooth_is_scanning(ctx->dev)) {
        ensureBluetoothOn(ctx->dev);
        ctx->scanRunning = true;
        ctx->uiDirty = true;
        ctx->listDirty = true;
        if (bluetooth::getRadioState() == bluetooth::RadioState::On) {
            bluetooth_scan_start(ctx->dev);
        }
    }
#endif
}

static void showScanScreen(Context* ctx) {
    auto* label = lv_label_create(ctx->body);
    lv_label_set_text(label, "Nearby BLE devices.");
    ctx->statusLabel = lv_label_create(ctx->body);
    lv_label_set_text(ctx->statusLabel, "Stopped");

    ctx->list = lv_table_create(ctx->body);
    lv_table_set_column_count(ctx->list, 3);
    lv_table_set_column_width(ctx->list, 0, 150);
    lv_table_set_column_width(ctx->list, 1, 180);
    lv_table_set_column_width(ctx->list, 2, 90);
    // Compact rows: shrink vertical cell padding so more records fit on the small display.
    lv_obj_set_style_pad_ver(ctx->list, 2, LV_PART_ITEMS);
    lv_obj_set_style_pad_left(ctx->list, 4, LV_PART_ITEMS);
    lv_obj_set_style_pad_right(ctx->list, 4, LV_PART_ITEMS);
    lv_obj_set_style_pad_top(ctx->list, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ctx->list, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ctx->list, LV_PCT(100));
    lv_obj_set_flex_grow(ctx->list, 1);
    lv_obj_set_scroll_dir(ctx->list, LV_DIR_VER);
    lv_obj_set_style_pad_all(ctx->list, 0, LV_STATE_DEFAULT);

    auto* button = lv_button_create(ctx->body);
    lv_obj_set_width(button, LV_PCT(100));
    ctx->startButtonLabel = lv_label_create(button);
    lv_label_set_text(ctx->startButtonLabel, "Start Scan");
    lv_obj_center(ctx->startButtonLabel);
    lv_obj_add_event_cb(button, onStartScan, LV_EVENT_SHORT_CLICKED, ctx);

    ctx->countLabel = lv_label_create(ctx->body);
    lv_label_set_text(ctx->countLabel, "0 devices");
}

// ---- Spam ----

static void onSpamStartStop(lv_event_t* event) {
    auto* ctx = static_cast<Context*>(lv_event_get_user_data(event));
#if defined(CONFIG_BT_NIMBLE_ENABLED)
    ctx->spamRunning = !ctx->spamRunning;
    if (ctx->spamRunning) {
        ensureBluetoothOn(ctx->dev);
        ctx->spamTimer->start();
    } else {
        ctx->spamTimer->stop();
        bluetooth::stopAdvertising();
    }
#endif
    ctx->uiDirty = true;
}

static void onRandomizeSwitch(lv_event_t* event) {
    auto* sw = static_cast<lv_obj_t*>(lv_event_get_target(event));
    auto* ctx = static_cast<Context*>(lv_event_get_user_data(event));
    ctx->randomizeAddress = lv_obj_has_state(sw, LV_STATE_CHECKED);
}

static void showSpamScreen(Context* ctx) {
    auto* label = lv_label_create(ctx->body);
    lv_label_set_text(label, "Spoofs BLE advertisements.");

    // Per-family enable toggles (Apple / Android / Windows / Samsung).
    for (size_t i = 0; i < kFamilyCount; i++) {
        auto* btn = lv_button_create(ctx->body);
        lv_obj_set_width(btn, LV_PCT(100));
        auto* btnLabel = lv_label_create(btn);
        lv_label_set_text(btnLabel, kFamilies[i].label);
        lv_obj_center(btnLabel);
        auto* sw = lv_switch_create(btn);
        lv_obj_align(sw, LV_ALIGN_RIGHT_MID, -8, 0);
        lv_obj_set_user_data(sw, reinterpret_cast<void*>(static_cast<uintptr_t>(i)));
        lv_obj_add_event_cb(sw, onFamilySwitchChanged, LV_EVENT_VALUE_CHANGED, ctx);
        if (ctx->enabled[i]) {
            lv_obj_add_state(sw, LV_STATE_CHECKED);
        }
    }

    // Randomize address toggle.
    auto* btn = lv_button_create(ctx->body);
    lv_obj_set_width(btn, LV_PCT(100));
    auto* btnLabel = lv_label_create(btn);
    lv_label_set_text(btnLabel, "Randomize address");
    lv_obj_center(btnLabel);
    auto* sw = lv_switch_create(btn);
        lv_obj_align(sw, LV_ALIGN_RIGHT_MID, -8, 0);
    if (ctx->randomizeAddress) {
        lv_obj_add_state(sw, LV_STATE_CHECKED);
    }
    lv_obj_add_event_cb(sw, onRandomizeSwitch, LV_EVENT_VALUE_CHANGED, ctx);

    auto* button = lv_button_create(ctx->body);
    lv_obj_set_width(button, LV_PCT(100));
    ctx->startButtonLabel = lv_label_create(button);
    lv_label_set_text(ctx->startButtonLabel, "Start Spam");
    lv_obj_center(ctx->startButtonLabel);
    lv_obj_add_event_cb(button, onSpamStartStop, LV_EVENT_SHORT_CLICKED, ctx);

    // Status line: the run state on the left, the live spoof name beside it. The payload label is
    // only populated while running (see onPollTick) so the screen doesn't advertise a device that
    // isn't being broadcast.
    auto* statusRow = lv_obj_create(ctx->body);
    lv_obj_set_width(statusRow, LV_PCT(100));
    lv_obj_set_flex_flow(statusRow, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(statusRow, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);
    lv_obj_set_style_pad_all(statusRow, 0, LV_STATE_DEFAULT);
    lv_obj_set_scroll_dir(statusRow, LV_DIR_NONE);

    ctx->statusLabel = lv_label_create(statusRow);
    lv_label_set_text(ctx->statusLabel, "Stopped");

    ctx->spamLabel = lv_label_create(statusRow);
    lv_label_set_text(ctx->spamLabel, "");
    lv_obj_set_style_pad_left(ctx->spamLabel, 20, LV_STATE_DEFAULT);
}

// ---- AirTag monitor ----

static void onStartAirtag(lv_event_t* event) {
    auto* ctx = static_cast<Context*>(lv_event_get_user_data(event));
#if defined(CONFIG_BT_NIMBLE_ENABLED)
    if (ctx->airtagRunning) {
        ctx->airtagRunning = false;
        Device* dev = ctx->dev;
        if (dev != nullptr && bluetooth_is_scanning(dev)) {
            bluetooth_scan_stop(dev);
        }
        ctx->uiDirty = true;
        ctx->listDirty = true;
        return;
    }
    if (ctx->dev != nullptr) {
        ensureBluetoothOn(ctx->dev);
        ctx->airtagRunning = true;
        ctx->uiDirty = true;
        ctx->listDirty = true;
        if (bluetooth::getRadioState() == bluetooth::RadioState::On && !bluetooth_is_scanning(ctx->dev)) {
            bluetooth_scan_start(ctx->dev);
        }
    }
#endif
}

static void showAirtagScreen(Context* ctx) {
    auto* label = lv_label_create(ctx->body);
    lv_label_set_text(label, "Detects Apple Find My trackers.");
    ctx->statusLabel = lv_label_create(ctx->body);
    lv_label_set_text(ctx->statusLabel, "Stopped");

    ctx->list = lv_table_create(ctx->body);
    lv_table_set_column_count(ctx->list, 3);
    lv_table_set_column_width(ctx->list, 0, 150);
    lv_table_set_column_width(ctx->list, 1, 180);
    lv_table_set_column_width(ctx->list, 2, 90);
    // Compact rows: shrink vertical cell padding so more records fit on the small display.
    lv_obj_set_style_pad_ver(ctx->list, 2, LV_PART_ITEMS);
    lv_obj_set_style_pad_left(ctx->list, 4, LV_PART_ITEMS);
    lv_obj_set_style_pad_right(ctx->list, 4, LV_PART_ITEMS);
    lv_obj_set_style_pad_top(ctx->list, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ctx->list, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ctx->list, LV_PCT(100));
    lv_obj_set_flex_grow(ctx->list, 1);
    lv_obj_set_scroll_dir(ctx->list, LV_DIR_VER);
    lv_obj_set_style_pad_all(ctx->list, 0, LV_STATE_DEFAULT);

    auto* button = lv_button_create(ctx->body);
    lv_obj_set_width(button, LV_PCT(100));
    ctx->startButtonLabel = lv_label_create(button);
    lv_label_set_text(ctx->startButtonLabel, "Start Monitor");
    lv_obj_center(ctx->startButtonLabel);
    lv_obj_add_event_cb(button, onStartAirtag, LV_EVENT_SHORT_CLICKED, ctx);

    ctx->countLabel = lv_label_create(ctx->body);
    lv_label_set_text(ctx->countLabel, "0 AirTag(s)");
}

// ---- BLE Observer ----

static void onStartObserver(lv_event_t* event) {
    auto* ctx = static_cast<Context*>(lv_event_get_user_data(event));
#if defined(CONFIG_BT_NIMBLE_ENABLED)
    if (ctx->obsRunning) {
        ctx->obsRunning = false;
        Device* dev = ctx->dev;
        if (dev != nullptr) {
            // Stop unconditionally — is_scanning() can read false during the auto-restart gap and
            // skip the stop, leaving a freshly-restarted scan running until its window ends.
            bluetooth_scan_stop(dev);
        }
        // Toggle the button label immediately (this LVGL callback runs on the LVGL task), so the
        // state change is visible regardless of the poll tick's screen check.
        lv_label_set_text(ctx->startButtonLabel, "Start Observing");
        ctx->uiDirty = true;
        return;
    }
    if (ctx->dev != nullptr) {
        ensureBluetoothOn(ctx->dev);
        ctx->obsRunning = true;
        ctx->obsStartMs = get_millis();
        ctx->obsEntries.clear();
        ctx->obsDirty = true;
        ctx->uiDirty = true;
        lv_label_set_text(ctx->startButtonLabel, "Stop Observing");
        if (bluetooth::getRadioState() == bluetooth::RadioState::On && !bluetooth_is_scanning(ctx->dev)) {
            observerStartScan(ctx->dev, ctx);
        }
    }
#endif
}

static void onClearObserver(lv_event_t* event) {
    auto* ctx = static_cast<Context*>(lv_event_get_user_data(event));
    ctx->obsEntries.clear();
    ctx->obsDirty = true;
    // Stop the observer so the cleared log isn't instantly refilled by a still-running scan.
    ctx->obsRunning = false;
    if (ctx->dev != nullptr && bluetooth_is_scanning(ctx->dev)) {
        bluetooth_scan_stop(ctx->dev);
    }
    // Rebuild the table immediately. This callback runs on the LVGL task (so it's safe to touch the
    // widgets here); clearing obsEntries would otherwise only be picked up by the app main loop the
    // next time it wakes, which won't happen once scanning stops.
    rebuildObserverLog(ctx);
    ctx->uiDirty = true;
}

static void showObserverScreen(Context* ctx) {
    // Keep the body scrollable (as the AirTag monitor does) — setting it non-scrollable is what
    // caused the flex_grow table below to collapse instead of filling.

    // Frame counter, above the table.
    ctx->countLabel = lv_label_create(ctx->body);
    lv_obj_set_width(ctx->countLabel, LV_PCT(100));
    lv_label_set_text(ctx->countLabel, "0 frames");

    // Frame log as a table (Type | RSSI | Addr | Info), filling the space above the controls. Same
    // bounded, self-scrolling widget the Scan screen uses.
    ctx->obsLogLabel = lv_table_create(ctx->body);
    lv_table_set_column_count(ctx->obsLogLabel, 4);
    lv_table_set_column_width(ctx->obsLogLabel, 0, 72);
    lv_table_set_column_width(ctx->obsLogLabel, 1, 40);
    lv_table_set_column_width(ctx->obsLogLabel, 2, 144);
    lv_table_set_column_width(ctx->obsLogLabel, 3, 208);
    // Compact rows: shrink vertical cell padding so more records fit on the small display.
    lv_obj_set_style_pad_ver(ctx->obsLogLabel, 2, LV_PART_ITEMS);
    lv_obj_set_style_pad_left(ctx->obsLogLabel, 4, LV_PART_ITEMS);
    lv_obj_set_style_pad_right(ctx->obsLogLabel, 4, LV_PART_ITEMS);
    lv_obj_set_style_pad_top(ctx->obsLogLabel, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ctx->obsLogLabel, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ctx->obsLogLabel, LV_PCT(100));
    lv_obj_set_flex_grow(ctx->obsLogLabel, 1);
    lv_obj_set_scroll_dir(ctx->obsLogLabel, LV_DIR_VER);
    lv_obj_set_style_pad_all(ctx->obsLogLabel, 0, LV_STATE_DEFAULT);
    lv_table_set_row_count(ctx->obsLogLabel, 1);
    lv_table_set_cell_value(ctx->obsLogLabel, 0, 0, "Type");
    lv_table_set_cell_value(ctx->obsLogLabel, 0, 1, "RSSI");
    lv_table_set_cell_value(ctx->obsLogLabel, 0, 2, "Addr");
    lv_table_set_cell_value(ctx->obsLogLabel, 0, 3, "Info");

    // A single non-flex container holding both buttons (aligned left/right so they sit side by side).
    // Keeping it non-flex (not a flex-row) and fixed-height lets the flex_grow table above expand
    // and push this container to the bottom, as the stacked full-width buttons did.
    auto* bottomRow = lv_obj_create(ctx->body);
    lv_obj_set_width(bottomRow, LV_PCT(100));
    lv_obj_set_height(bottomRow, 32);
    lv_obj_set_style_pad_all(bottomRow, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(bottomRow, LV_OPA_TRANSP, LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(bottomRow, 0, LV_STATE_DEFAULT);
    lv_obj_set_scroll_dir(bottomRow, LV_DIR_NONE);

    auto* startBtn = lv_button_create(bottomRow);
    lv_obj_set_width(startBtn, LV_PCT(48));
    lv_obj_set_height(startBtn, 32);
    lv_obj_align(startBtn, LV_ALIGN_LEFT_MID, 0, 0);
    lv_obj_set_style_pad_all(startBtn, 0, LV_STATE_DEFAULT);
    ctx->startButtonLabel = lv_label_create(startBtn);
    lv_label_set_text(ctx->startButtonLabel, "Start Observing");
    lv_obj_center(ctx->startButtonLabel);
    lv_obj_add_event_cb(startBtn, onStartObserver, LV_EVENT_SHORT_CLICKED, ctx);

    auto* clearBtn = lv_button_create(bottomRow);
    lv_obj_set_width(clearBtn, LV_PCT(48));
    lv_obj_set_height(clearBtn, 32);
    lv_obj_align(clearBtn, LV_ALIGN_RIGHT_MID, 0, 0);
    lv_obj_set_style_pad_all(clearBtn, 0, LV_STATE_DEFAULT);
    auto* clearLabel = lv_label_create(clearBtn);
    lv_label_set_text(clearLabel, "Clear");
    lv_obj_center(clearLabel);
    lv_obj_add_event_cb(clearBtn, onClearObserver, LV_EVENT_SHORT_CLICKED, ctx);
}

// ---- Polling (timer task) ----

static void onPollTick(Context* ctx) {
    // Runs on the FreeRTOS timer daemon task, which is NOT the LVGL task. Mutating LVGL widgets
    // here (labels) requires the LVGL lock — exactly as BtManage's requestViewUpdate() does.
    // Without it the LVGL refresh task can race the invalidation and hang the app (task watchdog).
    lvgl_lock();
    if (ctx->statusLabel != nullptr) {
        const char* text;
        // No BLE device to drive: tell the user why the buttons appear to do nothing.
        if (ctx->dev == nullptr) {
            text = "No BLE device";
        } else if (ctx->spamRunning) {
            text = "Spamming";
        } else if (ctx->airtagRunning) {
            text = "Monitoring";
        } else if (ctx->scanRunning) {
            text = "Scanning";
        } else if (ctx->obsRunning) {
            text = "Observing";
        } else {
            text = "Stopped";
        }
        lv_label_set_text(ctx->statusLabel, text);
    }
    if (ctx->startButtonLabel != nullptr) {
        const char* text = ctx->scanRunning ? "Stop Scan" : "Start Scan";
        if (ctx->screen == Screen::Spam) {
            text = ctx->spamRunning ? "Stop Spam" : "Start Spam";
        } else if (ctx->screen == Screen::Airtag) {
            text = ctx->airtagRunning ? "Stop Monitor" : "Start Monitor";
        } else if (ctx->screen == Screen::Observer) {
            text = ctx->obsRunning ? "Stop Observing" : "Start Observing";
        }
        lv_label_set_text(ctx->startButtonLabel, text);
    }
    if (ctx->screen == Screen::Airtag && ctx->countLabel != nullptr) {
        lv_label_set_text(ctx->countLabel, std::format("{} AirTag(s)", (unsigned)ctx->airtagPeers.size()).c_str());
    }
    if (ctx->screen == Screen::Scan && ctx->countLabel != nullptr) {
        lv_label_set_text(ctx->countLabel, std::format("{} devices", (unsigned)ctx->scanPeers.size()).c_str());
    }
    if (ctx->screen == Screen::Observer) {
        if (ctx->countLabel != nullptr) {
            lv_label_set_text(ctx->countLabel, std::format("{} frames", (unsigned)ctx->obsEntries.size()).c_str());
        }
        if (ctx->obsDirty && ctx->obsLogLabel != nullptr) {
            ctx->obsDirty = false;
            // Rebuild the table here (every ~300ms) instead of on every BLE event; doing it per event
            // held the LVGL lock constantly under the flood of advertising reports and starved the
            // LVGL task (lockup). We already hold the LVGL lock in onPollTick.
            rebuildObserverLog(ctx);
        }
    }
    if (ctx->screen == Screen::Spam && ctx->spamLabel != nullptr) {
        // Show the live spoof name only while actively broadcasting; keep it blank when stopped.
        lv_label_set_text(ctx->spamLabel, ctx->spamRunning ? kPayloads[ctx->currentPayload].label : "");
    }
    lvgl_unlock();
}

// ---- App entry ----

static void createWidgets(lv_obj_t* parent, void* userData) {
    auto* ctx = static_cast<Context*>(userData);
    lv_obj_set_flex_flow(parent, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_row(parent, 0, LV_STATE_DEFAULT);

    auto* toolbar = lvgl_toolbar_create(parent, "BLE Toolbox");
    lvgl_toolbar_set_nav_action(toolbar, LV_SYMBOL_CLOSE, onBackPressed, ctx);

    ctx->body = lv_obj_create(parent);
    lv_obj_set_width(ctx->body, LV_PCT(100));
    lv_obj_set_height(ctx->body, LV_PCT(100)); // fill the window so flex_grow children have space
    lv_obj_set_flex_grow(ctx->body, 1);
    lv_obj_set_flex_flow(ctx->body, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_all(ctx->body, 8, LV_STATE_DEFAULT);
    lv_obj_set_style_pad_column(ctx->body, 6, LV_STATE_DEFAULT);
    lv_obj_set_scroll_dir(ctx->body, LV_DIR_VER);
    lv_obj_scroll_to_y(ctx->body, 0, LV_ANIM_OFF);

    showMainScreen(ctx);
}

int32_t appMain(int argc, char* argv[]) {
    uint32_t appInstanceId = app_scheduler_current_app_id();
    Context ctx;
    ctx.appInstanceId = appInstanceId;

    TaskEventGroup event_group;
    task_event_group_construct(&event_group);
    AppEventSubscription sub{};
    app_event_subscribe(&sub, &event_group);

    // Resolve the BLE device and subscribe to scan/radio events. The devicetree marks ble0
    // "disabled" (constructed but not started), so get_first_active_by_type() finds nothing and
    // every button would silently no-op (or crash on the null device in ensureBluetoothOn). Use
    // get_first_by_type() to find the not-yet-started device, then start it so the radio can be
    // driven and the driver is ready to log events.
#if defined(CONFIG_BT_NIMBLE_ENABLED)
    Device* dev = nullptr;
    if (device_get_first_by_type(&BLUETOOTH_TYPE, &dev) == ERROR_NONE && dev != nullptr) {
        ctx.dev = dev;
        bluetooth::start(dev);
        if (bluetooth_event_subscribe(dev, &ctx.btSub, &event_group) == ERROR_NONE) {
            LOG_I(TAG, "Subscribed to BLE events");
        } else {
            LOG_W(TAG, "Failed to subscribe to BLE events");
        }
    } else {
        LOG_W(TAG, "No Bluetooth device found");
        ctx.dev = nullptr;
    }
#endif

    ctx.pollTimer = std::make_unique<Timer>(Timer::Type::Periodic, millis_to_ticks(POLL_INTERVAL_MS), [&ctx] {
        onPollTick(&ctx);
    });
    ctx.spamTimer = std::make_unique<Timer>(Timer::Type::Periodic, millis_to_ticks(SPAM_INTERVAL_MS), [&ctx] {
        onSpamTick(&ctx);
    });
    ctx.pollTimer->start();

    WindowId window = window_manager_create(appInstanceId, createWidgets, &ctx);

    bool shouldClose = false;
    while (!shouldClose) {
        task_event_group_wait_any(&event_group, nullptr, portMAX_DELAY);

        AppEvent event{};
        while (app_event_poll(&sub, &event) == ERROR_NONE) {
            if (event.type == APP_EVENT_CLOSE) {
                shouldClose = true;
                break;
            }
        }

#if defined(CONFIG_BT_NIMBLE_ENABLED)
        if (ctx.dev != nullptr) {
            BtEvent bt_event{};
            while (bluetooth_event_poll(&ctx.btSub, &bt_event) == ERROR_NONE) {
                onBtEvent(&ctx, bt_event);
            }
        }
#endif

        // Rebuild the device list on the app task. This task is NOT the LVGL task, so the LVGL
        // lock must be held around any widget mutation first — the exact bug BtManage documents.
        // The peer-list rebuild creates/cleans many widgets, so it also needs the larger stack set
        // in the manifest below.
        if (ctx.listDirty && ctx.list != nullptr) {
            ctx.listDirty = false;
            lvgl_lock();
            rebuildList(&ctx);
            lvgl_unlock();
        }

        // The observer log now rebuilds inside onPollTick (throttled to ~300ms) to avoid holding the
        // LVGL lock under the flood of advertising reports.
    }

    stopAllActive(&ctx);
    ctx.pollTimer->stop();
    ctx.spamTimer->stop();

#if defined(CONFIG_BT_NIMBLE_ENABLED)
    if (ctx.dev != nullptr) {
        bluetooth_event_unsubscribe(ctx.dev, &ctx.btSub);
        if (bluetooth_is_scanning(ctx.dev)) {
            bluetooth_scan_stop(ctx.dev);
        }
        device_put(ctx.dev);
        ctx.dev = nullptr;
    }
#endif

    window_manager_remove(window);
    app_event_unsubscribe(&sub);
    task_event_group_destruct(&event_group);
    return 0;
}

} // namespace

uint32_t start() {
    uint32_t instanceId = 0;
    app_start(manifest.id, 0, nullptr, &instanceId);
    return instanceId;
}

extern const ::AppManifest manifest = {
    .id = "BleToolbox",
    .name = "BLE Toolbox",
    .category = APP_CATEGORY_USER,
    .location = { APP_LOCATION_MEMORY, reinterpret_cast<void*>(appMain) },
    .flags = 0,
    // The scan/AirTag peer-list rebuild does heavy LVGL work (creating/cleaning many widgets,
    // forcing layout, object-tree redraw recursion) on this app's own task, and the BLE Observer
    // builds a large log string. The default 8 KB stack overflows under that redraw recursion, so
    // give the app more headroom (as BtManage does); the LVGL lock must also be held around those
    // widget mutations. A ~24 KB internal stack is well under APP_STACK_SIZE_MAX (64 KB).
    .stack = { .depth = 6144 }, // 24 KB
};

} // namespace tt::app::bletoolbox
