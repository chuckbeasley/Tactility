#ifdef ESP_PLATFORM
#include <sdkconfig.h>
#endif

#if defined(CONFIG_SOC_WIFI_SUPPORTED) || defined(CONFIG_SLAVE_SOC_WIFI_SUPPORTED)

#include <esp_event.h>
#include <esp_netif.h>
#include <esp_wifi.h>
#include <esp_wifi_default.h>

#include <tactility/concurrent/mutex.h>
#include <tactility/device.h>
#include <tactility/driver.h>
#include <tactility/drivers/esp32_wifi.h>
#include <tactility/drivers/wifi.h>
#include <tactility/error_esp32.h>
#include <tactility/log.h>
#include <tactility/time.h>

#include <TactilityCpp/Allocator.h>

#if defined(CONFIG_ESP_HOSTED_ENABLED)
#include <tactility/drivers/esp32_esp_hosted_ota.h>
#endif

#include "tactility/system_event.h"


#include <algorithm>
#include <cstring>
#include <new>

#define TAG "esp32_wifi"

namespace {

constexpr uint16_t WIFI_SCAN_RECORD_LIMIT = 32;

struct Esp32WifiCtx {
    Device* device = nullptr;

    Mutex mutex{};
    WifiRadioState radioState = WIFI_RADIO_STATE_OFF;
    WifiStationState stationState = WIFI_STATION_STATE_DISCONNECTED;
    bool scanning = false;
    char targetSsid[33] = {};
    esp_netif_ip_info_t ipInfo{};
    wifi_ap_record_t scanResults[WIFI_SCAN_RECORD_LIMIT] = {};
    uint16_t scanResultCount = 0;

    esp_netif_t* netif = nullptr;
    esp_event_handler_instance_t wifiEventHandler = nullptr;
    esp_event_handler_instance_t ipEventHandler = nullptr;

    // Dedup for WIFI_EVENT/IP_EVENT notifications: on the esp_hosted/Wi-Fi Remote transport
    // (e.g. Tab5's P4 host + C6 co-processor), the RPC layer has been observed delivering the
    // exact same event twice in a row (same base, same event_id, same millisecond - not two
    // genuinely separate occurrences). Native WiFi doesn't exhibit this, but the handler is
    // shared, so the guard applies unconditionally; it's a no-op for well-separated real events.
    esp_event_base_t lastEventBase = nullptr;
    int32_t lastEventId = -1;
    TickType_t lastEventTick = 0;

    Mutex subscriptionsMutex{};
    WifiEventSubscription* subscriptions = nullptr;
};

#define GET_CTX(device) (static_cast<Esp32WifiCtx*>(device_get_driver_data(device)))

// Promiscuous-mode RX trampoline. ESP-IDF's esp_wifi_set_promiscuous_rx_cb() takes only a
// function pointer (no user context), so a single file-scope channel carries the callback + context
// that the app registered via api_set_promiscuous_callback(). There is exactly one WiFi device per
// board, so a file-scope channel is unambiguous and keeps the RX path allocation-free.
static WifiPromiscuousCallback g_promiscuousCallback = nullptr;
static void* g_promiscuousContext = nullptr;

// Called from ESP-IDF's Wi-Fi task. Must be short and non-blocking: copy nothing, just forward the
// raw 802.11 frame (payload[0], sig_len in the rx_ctrl, FCS included) to the app's callback. The app
// is responsible for any further filtering/copying.
static void promiscuous_rx_cb(void* buffer, wifi_promiscuous_pkt_type_t type) {
    if (g_promiscuousCallback == nullptr || buffer == nullptr) {
        return;
    }
    auto* pkt = static_cast<wifi_promiscuous_pkt_t*>(buffer);

    WifiPromiscuousPacketInfo info {};
    info.rssi = static_cast<int8_t>(pkt->rx_ctrl.rssi);
    info.channel = pkt->rx_ctrl.channel;
    switch (type) {
        case WIFI_PKT_MGMT: info.type = WIFI_PROMISCUOUS_PACKET_TYPE_MGMT; break;
        case WIFI_PKT_CTRL: info.type = WIFI_PROMISCUOUS_PACKET_TYPE_CTRL; break;
        case WIFI_PKT_DATA: info.type = WIFI_PROMISCUOUS_PACKET_TYPE_DATA; break;
        default:           info.type = WIFI_PROMISCUOUS_PACKET_TYPE_MISC; break;
    }

    // sig_len is the reception MPDU length including the 4-byte 802.11 FCS; the payload buffer holds
    // exactly that many bytes. Forwarding length=sig_len keeps the provider/consumer (e.g. PCAP
    // writer with its radiotap "FCS at end" flag) consistent.
    g_promiscuousCallback(g_promiscuousContext, pkt->payload, pkt->rx_ctrl.sig_len, info);
}

WifiAuthenticationType to_wifi_authentication_type(wifi_auth_mode_t mode) {
    switch (mode) {
        case WIFI_AUTH_OPEN: return WIFI_AUTHENTICATION_TYPE_OPEN;
        case WIFI_AUTH_WEP: return WIFI_AUTHENTICATION_TYPE_WEP;
        case WIFI_AUTH_WPA_PSK: return WIFI_AUTHENTICATION_TYPE_WPA_PSK;
        case WIFI_AUTH_WPA2_PSK: return WIFI_AUTHENTICATION_TYPE_WPA2_PSK;
        case WIFI_AUTH_WPA_WPA2_PSK: return WIFI_AUTHENTICATION_TYPE_WPA_WPA2_PSK;
        case WIFI_AUTH_WPA2_ENTERPRISE: return WIFI_AUTHENTICATION_TYPE_WPA2_ENTERPRISE;
        case WIFI_AUTH_WPA3_PSK: return WIFI_AUTHENTICATION_TYPE_WPA3_PSK;
        case WIFI_AUTH_WPA2_WPA3_PSK: return WIFI_AUTHENTICATION_TYPE_WPA2_WPA3_PSK;
        case WIFI_AUTH_WAPI_PSK: return WIFI_AUTHENTICATION_TYPE_WAPI_PSK;
        case WIFI_AUTH_OWE: return WIFI_AUTHENTICATION_TYPE_OWE;
        case WIFI_AUTH_WPA3_ENT_192: return WIFI_AUTHENTICATION_TYPE_WPA3_ENT_192;
        case WIFI_AUTH_DUMMY_1: return WIFI_AUTHENTICATION_TYPE_WPA3_EXT_PSK;
        case WIFI_AUTH_DUMMY_2: return WIFI_AUTHENTICATION_TYPE_WPA3_EXT_PSK_MIXED_MODE;
        default: return WIFI_AUTHENTICATION_TYPE_OPEN;
    }
}

void fire_event(Esp32WifiCtx* ctx, WifiEvent event) {
    mutex_lock(&ctx->subscriptionsMutex);
    for (WifiEventSubscription* sub = ctx->subscriptions; sub != nullptr; sub = sub->internal.next) {
        mutex_lock(&sub->internal.ring_mutex);
        if (sub->internal.count < WIFI_EVENT_QUEUE_CAPACITY) {
            uint8_t tail = (sub->internal.head + sub->internal.count) % WIFI_EVENT_QUEUE_CAPACITY;
            sub->internal.queue[tail] = event;
            sub->internal.count++;
        }
        mutex_unlock(&sub->internal.ring_mutex);
        task_event_group_signal(sub->internal.event_group, sub->bit);
    }
    mutex_unlock(&ctx->subscriptionsMutex);
}

// ---- ESP-IDF event handling (runs on the esp_event task) ----

void on_wifi_or_ip_event(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
    auto* ctx = static_cast<Esp32WifiCtx*>(arg);

    // See Esp32WifiCtx::lastEventBase/lastEventId/lastEventTick - collapse an immediate duplicate
    // delivery of the same event (observed on the esp_hosted/Wi-Fi Remote transport) into one.
    constexpr uint32_t DEDUP_WINDOW_MS = 50; // well under any real re-occurrence of the same event
    TickType_t now = get_ticks();
    bool is_duplicate = event_base == ctx->lastEventBase && event_id == ctx->lastEventId &&
        (now - ctx->lastEventTick) <= millis_to_ticks(DEDUP_WINDOW_MS);
    ctx->lastEventBase = event_base;
    ctx->lastEventId = event_id;
    ctx->lastEventTick = now;
    if (is_duplicate) {
        LOG_D(TAG, "Ignoring duplicate WiFi event %d", (int)event_id);
        return;
    }

    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        mutex_lock(&ctx->mutex);
        bool was_pending = ctx->stationState == WIFI_STATION_STATE_CONNECTION_PENDING;
        ctx->stationState = WIFI_STATION_STATE_DISCONNECTED;
        memset(&ctx->ipInfo, 0, sizeof(ctx->ipInfo));
        mutex_unlock(&ctx->mutex);

        WifiEvent state_event = {};
        state_event.type = WIFI_EVENT_TYPE_STATION_STATE_CHANGED;
        state_event.station_state = WIFI_STATION_STATE_DISCONNECTED;
        fire_event(ctx, state_event);

        if (was_pending) {
            WifiEvent result_event = {};
            result_event.type = WIFI_EVENT_TYPE_STATION_CONNECTION_RESULT;
            result_event.connection_error = WIFI_STATION_CONNECTION_ERROR_TARGET_NOT_FOUND;
            fire_event(ctx, result_event);
            NetworkDisconnectedEvent disconnected_event = {
                .device = ctx->device
            };
            system_event_emit(KERNEL_EVENT_NETWORK_DISCONNECTED, &disconnected_event, sizeof(disconnected_event));
        }
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        auto* got_ip = static_cast<ip_event_got_ip_t*>(event_data);

        mutex_lock(&ctx->mutex);
        ctx->ipInfo = got_ip->ip_info;
        ctx->stationState = WIFI_STATION_STATE_CONNECTED;
        mutex_unlock(&ctx->mutex);

        WifiEvent state_event = {};
        state_event.type = WIFI_EVENT_TYPE_STATION_STATE_CHANGED;
        state_event.station_state = WIFI_STATION_STATE_CONNECTED;
        fire_event(ctx, state_event);

        WifiEvent result_event = {};
        result_event.type = WIFI_EVENT_TYPE_STATION_CONNECTION_RESULT;
        result_event.connection_error = WIFI_STATION_CONNECTION_ERROR_NONE;
        fire_event(ctx, result_event);

        NetworkConnectedEvent connected_event = {
            .device = ctx->device,
            .ipv4_addr = ctx->ipInfo.ip.addr,
            .gateway = ctx->ipInfo.gw.addr,
        };
        system_event_emit(KERNEL_EVENT_NETWORK_CONNECTED, &connected_event, sizeof(connected_event));
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_SCAN_DONE) {
        mutex_lock(&ctx->mutex);
        ctx->scanning = false;
        uint16_t count = WIFI_SCAN_RECORD_LIMIT;
        esp_err_t err = esp_wifi_scan_get_ap_records(&count, ctx->scanResults);
        ctx->scanResultCount = (err == ESP_OK) ? count : 0;
        mutex_unlock(&ctx->mutex);

        WifiEvent event = {};
        event.type = WIFI_EVENT_TYPE_SCAN_FINISHED;
        fire_event(ctx, event);
    }
}

// ---- Work, always run inline on the caller (see note at the top) ----

error_t bring_up_wifi(Esp32WifiCtx* ctx) {
    ctx->netif = esp_netif_create_default_wifi_sta();
    if (ctx->netif == nullptr) {
        LOG_E(TAG, "Failed to create default STA netif");
        return ERROR_RESOURCE;
    }

    // Warning: this is the memory-intensive operation. It uses over 100kB of
    // RAM with default settings.
    wifi_init_config_t init_config = WIFI_INIT_CONFIG_DEFAULT();
    esp_err_t err = esp_wifi_init(&init_config);
    if (err != ESP_OK) {
        LOG_E(TAG, "esp_wifi_init failed: %s", esp_err_to_name(err));
        esp_netif_destroy(ctx->netif);
        ctx->netif = nullptr;
        return esp_err_to_error(err);
    }

    esp_wifi_set_storage(WIFI_STORAGE_RAM);

    err = esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &on_wifi_or_ip_event, ctx, &ctx->wifiEventHandler);
    if (err != ESP_OK) {
        LOG_E(TAG, "Failed to register WIFI_EVENT handler: %s", esp_err_to_name(err));
        esp_wifi_deinit();
        esp_netif_destroy(ctx->netif);
        ctx->netif = nullptr;
        return esp_err_to_error(err);
    }

    err = esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &on_wifi_or_ip_event, ctx, &ctx->ipEventHandler);
    if (err != ESP_OK) {
        LOG_E(TAG, "Failed to register IP_EVENT handler: %s", esp_err_to_name(err));
        esp_event_handler_instance_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID, ctx->wifiEventHandler);
        ctx->wifiEventHandler = nullptr;
        esp_wifi_deinit();
        esp_netif_destroy(ctx->netif);
        ctx->netif = nullptr;
        return esp_err_to_error(err);
    }

    err = esp_wifi_set_mode(WIFI_MODE_STA);
    if (err != ESP_OK) {
        LOG_E(TAG, "esp_wifi_set_mode failed: %s", esp_err_to_name(err));
        esp_event_handler_instance_unregister(IP_EVENT, IP_EVENT_STA_GOT_IP, ctx->ipEventHandler);
        esp_event_handler_instance_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID, ctx->wifiEventHandler);
        ctx->ipEventHandler = nullptr;
        ctx->wifiEventHandler = nullptr;
        esp_wifi_deinit();
        esp_netif_destroy(ctx->netif);
        ctx->netif = nullptr;
        return esp_err_to_error(err);
    }

    err = esp_wifi_start();
    if (err != ESP_OK) {
        LOG_E(TAG, "esp_wifi_start failed: %s", esp_err_to_name(err));
        esp_event_handler_instance_unregister(IP_EVENT, IP_EVENT_STA_GOT_IP, ctx->ipEventHandler);
        esp_event_handler_instance_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID, ctx->wifiEventHandler);
        ctx->ipEventHandler = nullptr;
        ctx->wifiEventHandler = nullptr;
        esp_wifi_set_mode(WIFI_MODE_NULL);
        esp_wifi_deinit();
        esp_netif_destroy(ctx->netif);
        ctx->netif = nullptr;
        return esp_err_to_error(err);
    }

    mutex_lock(&ctx->mutex);
    ctx->radioState = WIFI_RADIO_STATE_ON;
    mutex_unlock(&ctx->mutex);

    WifiEvent radio_event = {};
    radio_event.type = WIFI_EVENT_TYPE_RADIO_STATE_CHANGED;
    radio_event.radio_state = WIFI_RADIO_STATE_ON;
    fire_event(ctx, radio_event);

    LOG_I(TAG, "WiFi radio on");
    return ERROR_NONE;
}

void bring_down_wifi(Esp32WifiCtx* ctx) {
    mutex_lock(&ctx->mutex);
    bool was_connected = ctx->stationState != WIFI_STATION_STATE_DISCONNECTED;
    bool was_scanning = ctx->scanning;
    ctx->stationState = WIFI_STATION_STATE_DISCONNECTED;
    ctx->scanning = false;
    mutex_unlock(&ctx->mutex);

    if (was_scanning) {
        esp_wifi_scan_stop();
    }
    if (was_connected) {
        esp_wifi_disconnect();
    }

    // Detach netif from the internal WiFi event handlers before stopping,
    // otherwise esp_netif_destroy() can race with esp_wifi_stop()'s own
    // netif teardown (see esp32_ble/WifiEsp.cpp for the same issue).
    if (ctx->netif != nullptr) {
        esp_wifi_clear_default_wifi_driver_and_handlers(ctx->netif);
    }

    esp_wifi_stop();
    esp_wifi_set_mode(WIFI_MODE_NULL);

    if (ctx->wifiEventHandler != nullptr) {
        esp_event_handler_instance_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID, ctx->wifiEventHandler);
        ctx->wifiEventHandler = nullptr;
    }
    if (ctx->ipEventHandler != nullptr) {
        esp_event_handler_instance_unregister(IP_EVENT, IP_EVENT_STA_GOT_IP, ctx->ipEventHandler);
        ctx->ipEventHandler = nullptr;
    }

    esp_wifi_deinit();

    if (ctx->netif != nullptr) {
        esp_netif_destroy(ctx->netif);
        ctx->netif = nullptr;
    }

    mutex_lock(&ctx->mutex);
    ctx->radioState = WIFI_RADIO_STATE_OFF;
    mutex_unlock(&ctx->mutex);

    WifiEvent radio_event = {};
    radio_event.type = WIFI_EVENT_TYPE_RADIO_STATE_CHANGED;
    radio_event.radio_state = WIFI_RADIO_STATE_OFF;
    fire_event(ctx, radio_event);

    LOG_I(TAG, "WiFi radio off");
}

// ---- WifiApi ----

error_t api_get_radio_state(Device* device, WifiRadioState* state) {
    auto* ctx = GET_CTX(device);
    if (ctx == nullptr || state == nullptr) return ERROR_INVALID_ARGUMENT;
    mutex_lock(&ctx->mutex);
    *state = ctx->radioState;
    mutex_unlock(&ctx->mutex);
    return ERROR_NONE;
}

error_t api_get_station_state(Device* device, WifiStationState* state) {
    auto* ctx = GET_CTX(device);
    if (ctx == nullptr || state == nullptr) return ERROR_INVALID_ARGUMENT;
    mutex_lock(&ctx->mutex);
    *state = ctx->stationState;
    mutex_unlock(&ctx->mutex);
    return ERROR_NONE;
}

error_t api_get_access_point_state(Device* device, WifiAccessPointState* state) {
    auto* ctx = GET_CTX(device);
    if (ctx == nullptr || state == nullptr) return ERROR_INVALID_ARGUMENT;
    // Access point mode isn't implemented by this driver.
    *state = WIFI_ACCESS_POINT_STATE_STOPPED;
    return ERROR_NONE;
}

bool api_is_scanning(Device* device) {
    auto* ctx = GET_CTX(device);
    if (ctx == nullptr) return false;
    mutex_lock(&ctx->mutex);
    bool scanning = ctx->scanning;
    mutex_unlock(&ctx->mutex);
    return scanning;
}

error_t api_scan(Device* device) {
    auto* ctx = GET_CTX(device);
    if (ctx == nullptr) return ERROR_INVALID_STATE;

    mutex_lock(&ctx->mutex);
    if (ctx->radioState != WIFI_RADIO_STATE_ON || ctx->scanning) {
        mutex_unlock(&ctx->mutex);
        return ERROR_INVALID_STATE;
    }
    mutex_unlock(&ctx->mutex);

    esp_err_t err = esp_wifi_scan_start(nullptr, false);
    if (err != ESP_OK) {
        return esp_err_to_error(err);
    }

    mutex_lock(&ctx->mutex);
    ctx->scanning = true;
    mutex_unlock(&ctx->mutex);

    WifiEvent event = {};
    event.type = WIFI_EVENT_TYPE_SCAN_STARTED;
    fire_event(ctx, event);
    return ERROR_NONE;
}

error_t api_get_scan_results(Device* device, WifiApRecord* results, size_t* num_results) {
    auto* ctx = GET_CTX(device);
    if (ctx == nullptr || results == nullptr || num_results == nullptr) return ERROR_INVALID_ARGUMENT;

    mutex_lock(&ctx->mutex);
    size_t count = std::min<size_t>(*num_results, ctx->scanResultCount);
    for (size_t i = 0; i < count; i++) {
        const wifi_ap_record_t& src = ctx->scanResults[i];
        WifiApRecord& dst = results[i];
        memset(dst.ssid, 0, sizeof(dst.ssid));
        memcpy(dst.ssid, src.ssid, std::min(sizeof(dst.ssid) - 1, sizeof(src.ssid)));
        dst.rssi = src.rssi;
        dst.channel = src.primary;
        memcpy(dst.bssid, src.bssid, sizeof(dst.bssid));
        dst.authentication_type = to_wifi_authentication_type(src.authmode);
    }
    *num_results = count;
    mutex_unlock(&ctx->mutex);
    return ERROR_NONE;
}

error_t api_station_get_ipv4_address(Device* device, char* ipv4) {
    auto* ctx = GET_CTX(device);
    if (ctx == nullptr || ipv4 == nullptr) return ERROR_INVALID_ARGUMENT;
    mutex_lock(&ctx->mutex);
    esp_ip4addr_ntoa(&ctx->ipInfo.ip, ipv4, 16);
    mutex_unlock(&ctx->mutex);
    return ERROR_NONE;
}

error_t api_station_get_target_ssid(Device* device, char* ssid) {
    auto* ctx = GET_CTX(device);
    if (ctx == nullptr || ssid == nullptr) return ERROR_INVALID_ARGUMENT;
    mutex_lock(&ctx->mutex);
    constexpr size_t ssid_buffer_size = sizeof(ctx->targetSsid);
    strncpy(ssid, ctx->targetSsid, ssid_buffer_size);
    mutex_unlock(&ctx->mutex);
    return ERROR_NONE;
}

error_t api_station_connect(Device* device, const char* ssid, const char* password, int32_t channel) {
    auto* ctx = GET_CTX(device);
    if (ctx == nullptr || ssid == nullptr) return ERROR_INVALID_ARGUMENT;

    mutex_lock(&ctx->mutex);
    bool radio_on = ctx->radioState == WIFI_RADIO_STATE_ON;
    bool was_connected = ctx->stationState != WIFI_STATION_STATE_DISCONNECTED;
    mutex_unlock(&ctx->mutex);

    if (!radio_on) {
        return ERROR_INVALID_STATE;
    }

    if (was_connected) {
        esp_wifi_disconnect();
    }

    wifi_config_t config {};
    config.sta.channel = static_cast<uint8_t>(channel);
    config.sta.scan_method = WIFI_FAST_SCAN;
    config.sta.sort_method = WIFI_CONNECT_AP_BY_SIGNAL;
    config.sta.threshold.rssi = -127;
    config.sta.pmf_cfg.capable = true;
    strncpy(reinterpret_cast<char*>(config.sta.ssid), ssid, sizeof(config.sta.ssid) - 1);
    if (password != nullptr && password[0] != '\0') {
        strncpy(reinterpret_cast<char*>(config.sta.password), password, sizeof(config.sta.password) - 1);
        config.sta.threshold.authmode = WIFI_AUTH_WPA2_PSK;
    }

    esp_err_t err = esp_wifi_set_config(WIFI_IF_STA, &config);
    if (err != ESP_OK) {
        return esp_err_to_error(err);
    }

    mutex_lock(&ctx->mutex);
    strncpy(ctx->targetSsid, ssid, sizeof(ctx->targetSsid) - 1);
    ctx->targetSsid[sizeof(ctx->targetSsid) - 1] = '\0';
    ctx->stationState = WIFI_STATION_STATE_CONNECTION_PENDING;
    mutex_unlock(&ctx->mutex);

    err = esp_wifi_connect();
    if (err != ESP_OK) {
        mutex_lock(&ctx->mutex);
        ctx->stationState = WIFI_STATION_STATE_DISCONNECTED;
        mutex_unlock(&ctx->mutex);
        return esp_err_to_error(err);
    }

    WifiEvent event = {};
    event.type = WIFI_EVENT_TYPE_STATION_STATE_CHANGED;
    event.station_state = WIFI_STATION_STATE_CONNECTION_PENDING;
    fire_event(ctx, event);
    return ERROR_NONE;
}

error_t api_station_disconnect(Device* device) {
    auto* ctx = GET_CTX(device);
    if (ctx == nullptr) return ERROR_INVALID_STATE;

    mutex_lock(&ctx->mutex);
    bool was_connected = ctx->stationState != WIFI_STATION_STATE_DISCONNECTED;
    mutex_unlock(&ctx->mutex);

    if (!was_connected) {
        return ERROR_NONE;
    }

    // The DISCONNECTED state change is published by the WIFI_EVENT handler
    // once ESP-IDF confirms the disconnect, so we don't fire it here.
    esp_err_t err = esp_wifi_disconnect();
    return err == ESP_OK ? ERROR_NONE : esp_err_to_error(err);
}

error_t api_station_get_rssi(Device* device, int32_t* rssi) {
    auto* ctx = GET_CTX(device);
    if (ctx == nullptr || rssi == nullptr) return ERROR_INVALID_ARGUMENT;

    int native_rssi = 0;
    esp_err_t err = esp_wifi_sta_get_rssi(&native_rssi);
    if (err != ESP_OK) {
        return esp_err_to_error(err);
    }
    *rssi = native_rssi;
    return ERROR_NONE;
}

error_t api_set_promiscuous(Device* device, bool enable) {
    auto* ctx = GET_CTX(device);
    if (ctx == nullptr) return ERROR_INVALID_STATE;

    esp_err_t err = esp_wifi_set_promiscuous(enable);
    if (err != ESP_OK) {
        return esp_err_to_error(err);
    }
    if (enable) {
        // Management, data and misc - deliberately NOT control.
        //
        // WIFI_PROMIS_FILTER_MASK_ALL also forwards control frames, and on any real channel that
        // means an ACK for every acknowledged frame: thousands per second, each delivered to the
        // app callback from the driver's own receive path, where each one is copied twice. That work
        // competes with the driver's handling of beacons, and the station then times out on them and
        // drops the link - observed consistently about nine seconds after the sniffer is enabled,
        // with the channel uninvolved and internal memory healthy (43 KB free, no low-memory warning).
        // Nothing the app needs is a control frame: EAPOL/PMKID are data, and deauth/assoc/beacons are
        // management. The previously stated reason for ALL - observing ACK/RTS noise for the
        // association-sleep attack - is not worth losing the association over, and can be revisited
        // with a narrower filter if that feature ever needs it.
        wifi_promiscuous_filter_t filter {
            .filter_mask = WIFI_PROMIS_FILTER_MASK_MGMT | WIFI_PROMIS_FILTER_MASK_DATA | WIFI_PROMIS_FILTER_MASK_MISC
        };
        esp_wifi_set_promiscuous_filter(&filter);
    }
    return ERROR_NONE;
}

error_t api_get_promiscuous(Device* device, bool* enabled) {
    auto* ctx = GET_CTX(device);
    if (ctx == nullptr || enabled == nullptr) return ERROR_INVALID_ARGUMENT;

    esp_err_t err = esp_wifi_get_promiscuous(enabled);
    return err == ESP_OK ? ERROR_NONE : esp_err_to_error(err);
}

error_t api_set_promiscuous_callback(Device* device, WifiPromiscuousCallback callback, void* context) {
    auto* ctx = GET_CTX(device);
    if (ctx == nullptr) return ERROR_INVALID_STATE;

    // Route the caller's callback/context through the file-scope trampoline. Even when `callback`
    // is null (unregister), keep the trampoline registered and let it no-op - avoiding any concern
    // about whether esp_wifi accepts a null rx cb.
    g_promiscuousCallback = callback;
    g_promiscuousContext = context;

    esp_err_t err = esp_wifi_set_promiscuous_rx_cb(promiscuous_rx_cb);
    return err == ESP_OK ? ERROR_NONE : esp_err_to_error(err);
}

error_t api_set_channel(Device* device, uint8_t channel) {
    auto* ctx = GET_CTX(device);
    if (ctx == nullptr) return ERROR_INVALID_STATE;

    esp_err_t err = esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE);
    return err == ESP_OK ? ERROR_NONE : esp_err_to_error(err);
}

error_t api_station_get_bssid(Device* device, uint8_t* bssid) {
    auto* ctx = GET_CTX(device);
    if (ctx == nullptr || bssid == nullptr) return ERROR_INVALID_ARGUMENT;

    // The driver already has this - it logs the BSSID when the station associates - it was simply
    // never exposed. Addressing a deauth at the associated access point needs it, and taking it
    // from the connection is what stops the target drifting between cells heard on the same channel.
    wifi_ap_record_t ap_info = {};
    esp_err_t err = esp_wifi_sta_get_ap_info(&ap_info);
    if (err != ESP_OK) return esp_err_to_error(err);

    memcpy(bssid, ap_info.bssid, 6);
    return ERROR_NONE;
}

error_t api_send_raw_frame(Device* device, const uint8_t* frame, size_t length) {
    auto* ctx = GET_CTX(device);
    if (ctx == nullptr || frame == nullptr) return ERROR_INVALID_ARGUMENT;
    if (length < 24 || length > 1500) return ERROR_INVALID_ARGUMENT;

    // esp_wifi_80211_tx sends raw 802.11 frames (beacon/probe/action + non-QoS data). en_sys_seq=true
    // works whether or not the STA is connected, so beacon/probe injection is unaffected by the
    // radio's association state. (Deauth frames are rejected by the stock WiFi library's sanity
    // check, so they silently don't transmit here - see the WifiToolbox notes.)
    esp_err_t err = esp_wifi_80211_tx(WIFI_IF_STA, frame, static_cast<int>(length), true);
    return err == ESP_OK ? ERROR_NONE : esp_err_to_error(err);
}

error_t api_set_radio_on(Device* device) {
    auto* ctx = GET_CTX(device);
    if (ctx == nullptr) return ERROR_INVALID_STATE;

    mutex_lock(&ctx->mutex);
    bool already_on = ctx->radioState == WIFI_RADIO_STATE_ON;
    mutex_unlock(&ctx->mutex);
    if (already_on) return ERROR_NONE;

    return bring_up_wifi(ctx);
}

error_t api_set_radio_off(Device* device) {
    auto* ctx = GET_CTX(device);
    if (ctx == nullptr) return ERROR_INVALID_STATE;

    mutex_lock(&ctx->mutex);
    bool already_off = ctx->radioState == WIFI_RADIO_STATE_OFF;
    mutex_unlock(&ctx->mutex);
    if (already_off) return ERROR_NONE;

    bring_down_wifi(ctx);
    return ERROR_NONE;
}

error_t api_event_subscribe(Device* device, WifiEventSubscription* sub, TaskEventGroup* event_group) {
    auto* ctx = GET_CTX(device);
    if (ctx == nullptr || sub == nullptr || event_group == nullptr) return ERROR_INVALID_ARGUMENT;

    uint32_t bit;
    error_t claim_result = task_event_group_claim_bit(event_group, &bit);
    if (claim_result != ERROR_NONE) {
        return claim_result;
    }

    mutex_lock(&ctx->subscriptionsMutex);

    // Avoid cyclic subscription list that would loop forever
    for (WifiEventSubscription* existing = ctx->subscriptions; existing != nullptr; existing = existing->internal.next) {
        if (existing == sub) {
            mutex_unlock(&ctx->subscriptionsMutex);
            task_event_group_release_bit(event_group, bit);
            return ERROR_INVALID_STATE;
        }
    }

    sub->internal.event_group = event_group;
    sub->bit = bit;
    sub->internal.next = ctx->subscriptions;
    ctx->subscriptions = sub;
    mutex_unlock(&ctx->subscriptionsMutex);
    return ERROR_NONE;
}

error_t api_event_unsubscribe(Device* device, WifiEventSubscription* sub) {
    auto* ctx = GET_CTX(device);
    if (ctx == nullptr || sub == nullptr) return ERROR_INVALID_ARGUMENT;

    error_t result = ERROR_NOT_FOUND;
    mutex_lock(&ctx->subscriptionsMutex);
    for (WifiEventSubscription** link = &ctx->subscriptions; *link != nullptr; link = &(*link)->internal.next) {
        if (*link == sub) {
            *link = sub->internal.next;
            result = ERROR_NONE;
            break;
        }
    }
    mutex_unlock(&ctx->subscriptionsMutex);

    if (result == ERROR_NONE) {
        task_event_group_release_bit(sub->internal.event_group, sub->bit);
    }
    return result;
}

error_t api_get_firmware_ops(Device* /*device*/, const FirmwareOps** ops, void** ctx) {
    // ops/ctx are caller-supplied output pointers, reachable from external (ELF) apps via
    // wifi_get_firmware_ops() - validate at this API boundary rather than trusting the caller.
    if (ops == nullptr || ctx == nullptr) {
        return ERROR_INVALID_ARGUMENT;
    }
#if defined(CONFIG_ESP_HOSTED_ENABLED)
    // Only meaningful on a hosted board (P4+C6/C5 etc.) - this wifi device is backed by a real
    // co-processor with its own updatable firmware there. On a native (non-hosted) chip, this
    // device's "radio" is the chip's own built-in WiFi, nothing to update via this interface.
    *ops = esp32_esp_hosted_ota_get_ops();
    *ctx = nullptr; // esp32_esp_hosted_ota's FirmwareOps functions are all singleton/global, no per-call ctx needed
    return ERROR_NONE;
#else
    return ERROR_NOT_SUPPORTED;
#endif
}

const WifiApi esp32_wifi_api = {
    .set_radio_on = api_set_radio_on,
    .set_radio_off = api_set_radio_off,
    .get_radio_state = api_get_radio_state,
    .get_station_state = api_get_station_state,
    .get_access_point_state = api_get_access_point_state,
    .is_scanning = api_is_scanning,
    .scan = api_scan,
    .get_scan_results = api_get_scan_results,
    .station_get_ipv4_address = api_station_get_ipv4_address,
    .station_get_target_ssid = api_station_get_target_ssid,
    .station_get_bssid = api_station_get_bssid,
    .station_connect = api_station_connect,
    .station_disconnect = api_station_disconnect,
    .station_get_rssi = api_station_get_rssi,
    .event_subscribe = api_event_subscribe,
    .event_unsubscribe = api_event_unsubscribe,
    .get_firmware_ops = api_get_firmware_ops,
    .set_promiscuous = api_set_promiscuous,
    .get_promiscuous = api_get_promiscuous,
    .set_promiscuous_callback = api_set_promiscuous_callback,
    .set_channel = api_set_channel,
    .send_raw_frame = api_send_raw_frame
};

error_t start_device(Device* device) {
    // Prefers PSRAM/SPIRAM, falling back to internal RAM when unavailable - aborts on true OOM
    // (see OptExternalAllocator's own doc), so no null check here.
    tt::OptExternalAllocator<Esp32WifiCtx> allocator;
    auto* ctx = allocator.allocate(1);
    new (ctx) Esp32WifiCtx();

    ctx->device = device;
    mutex_construct(&ctx->mutex);
    mutex_construct(&ctx->subscriptionsMutex);
    device_set_driver_data(device, ctx);

    return ERROR_NONE;
}

error_t stop_device(Device* device) {
    auto* ctx = GET_CTX(device);
    if (ctx == nullptr) return ERROR_NONE;

    if (ctx->radioState == WIFI_RADIO_STATE_ON) {
        bring_down_wifi(ctx);
    }

    // Release any subscribers that never unsubscribed: device_stop() doesn't wait for apps still
    // using this device, so a later wifi_event_unsubscribe() would find no ctx and skip releasing
    // the bit.
    mutex_lock(&ctx->subscriptionsMutex);
    for (WifiEventSubscription* sub = ctx->subscriptions; sub != nullptr;) {
        WifiEventSubscription* next = sub->internal.next;
        // Force-close, don't destruct: a concurrent wifi_event_poll() may hold/await this same
        // lock (see WifiEventSubscription::internal::closed).
        mutex_lock(&sub->internal.ring_mutex);
        sub->internal.closed = true;
        mutex_unlock(&sub->internal.ring_mutex);
        task_event_group_release_bit(sub->internal.event_group, sub->bit);
        sub = next;
    }
    ctx->subscriptions = nullptr;
    mutex_unlock(&ctx->subscriptionsMutex);

    device_set_driver_data(device, nullptr);
    mutex_destruct(&ctx->subscriptionsMutex);
    mutex_destruct(&ctx->mutex);
    ctx->~Esp32WifiCtx();
    tt::OptExternalAllocator<Esp32WifiCtx>().deallocate(ctx, 1);

    return ERROR_NONE;
}

} // namespace

extern "C" {

extern Module platform_esp32_module;

Driver esp32_wifi_driver = {
    .name = "esp32_wifi",
    .compatible = (const char*[]) { "espressif,esp32-wifi", nullptr },
    .start_device = start_device,
    .stop_device = stop_device,
    .api = (const void*)&esp32_wifi_api,
    .device_type = &WIFI_TYPE,
    .owner = &platform_esp32_module,
    .internal = nullptr
};

} // extern "C"

#endif // CONFIG_SOC_WIFI_SUPPORTED or CONFIG_SLAVE_SOC_WIFI_SUPPORTED
