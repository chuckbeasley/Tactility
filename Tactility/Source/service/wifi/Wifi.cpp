#include <Tactility/service/wifi/Wifi.h>

#include <Tactility/CoreDefines.h>
#include <Tactility/LogMessages.h>
#include <Tactility/RecursiveMutex.h>
#include <Tactility/Tactility.h>
#include <Tactility/Timer.h>
#include <Tactility/service/Service.h>
#include <Tactility/service/ServiceManifest.h>
#include <Tactility/service/ServiceRegistration.h>
#include <Tactility/service/wifi/WifiBootSplashInit.h>
#include <Tactility/service/wifi/WifiGlobals.h>

#include <tactility/check.h>
#include <tactility/device.h>
#include <tactility/drivers/wifi.h>
#include <tactility/log.h>
#include <tactility/system_event.h>
#include <tactility/time.h>
#include <tactility/wifi_auto_scan.h>

#include <algorithm>
#include <atomic>

#ifdef ESP_PLATFORM
#include <esp_heap_caps.h>
#endif

namespace tt::service::wifi {

constexpr auto* TAG = "WifiService";
constexpr auto AUTO_SCAN_INTERVAL = 10000; // ms

const char* radioStateToString(RadioState state) {
    switch (state) {
        using enum RadioState;
        case OnPending:
            return TT_STRINGIFY(OnPending);
        case On:
            return TT_STRINGIFY(On);
        case ConnectionPending:
            return TT_STRINGIFY(ConnectionPending);
        case ConnectionActive:
            return TT_STRINGIFY(ConnectionActive);
        case OffPending:
            return TT_STRINGIFY(OffPending);
        case Off:
            return TT_STRINGIFY(Off);
    }
    check(false, "not implemented");
}

extern const ServiceManifest manifest;

std::shared_ptr<ServiceContext> findServiceContext() {
    return findServiceContextById(manifest.id);
}

namespace {

// Everything below wraps a TactilityKernel WIFI_TYPE device: the driver owns
// the radio state, station state and scan results, this file only tracks the
// bits the kernel driver doesn't (in-flight connection target/credentials,
// auto-connect bookkeeping).

/** State lives for the entire process; only ever (re)initialized by onStart(). */
struct WifiServiceState {
    Device* device = nullptr;
    std::shared_ptr<PubSub<WifiEvent>> pubsub = std::make_shared<PubSub<WifiEvent>>();
    RecursiveMutex mutex;
    bool secureConnection = false;
    // Internal: set by connect()/disconnect() while a manual attempt is in flight, cleared on
    // connection success/failure. Distinct from externalScanPause below - the two must not
    // clobber each other, otherwise a caller's explicit pause (e.g. AutoScanPauseGuard during a
    // co-processor OTA) can be silently cleared by an unrelated connect/disconnect finishing.
    bool pauseAutoConnect = false;
    // Set by a deliberate disconnect() and only cleared by an explicit connect() or a radio
    // off->on cycle. Distinct from pauseAutoConnect: a user who disconnects wants to stay
    // disconnected until they pick a network, but still wants the network list to keep
    // refreshing, so this blocks auto-connect without blocking scanning.
    bool userDisconnected = false;
    // External: only setAutoScanPaused() may set/clear this. Read alongside pauseAutoConnect to
    // gate scan scheduling (both must be false to scan).
    std::atomic<bool> externalScanPause{false};
    bool connectionTargetRemember = false;
    settings::WifiApSettings connectionTarget;
    uint16_t scanRecordLimit = TT_WIFI_SCAN_RECORD_LIMIT;
    TickType_t lastScanTime = MAX_TICKS;
    std::unique_ptr<Timer> autoConnectTimer;
    bool bootEventSubscribed = false;
};

WifiServiceState state;
bool started = false;

void onWifiDeviceEvent(Device* device, void* context, ::WifiEvent event);
void dispatchScan();
void dispatchInitialAutoConnect();

// ---- Helpers ----

void publish(WifiEvent event) {
    state.pubsub->publish(event);
}

void publishRadioState(WifiRadioState radio_state) {
    WifiEvent event = {};
    event.type = WIFI_EVENT_TYPE_RADIO_STATE_CHANGED;
    event.radio_state = radio_state;
    publish(event);
}

RadioState combineRadioState(WifiRadioState radio, WifiStationState station) {
    switch (radio) {
        case WIFI_RADIO_STATE_OFF: return RadioState::Off;
        case WIFI_RADIO_STATE_ON_PENDING: return RadioState::OnPending;
        case WIFI_RADIO_STATE_OFF_PENDING: return RadioState::OffPending;
        case WIFI_RADIO_STATE_ON:
            switch (station) {
                case WIFI_STATION_STATE_CONNECTION_PENDING: return RadioState::ConnectionPending;
                case WIFI_STATION_STATE_CONNECTED: return RadioState::ConnectionActive;
                case WIFI_STATION_STATE_DISCONNECTED: default: return RadioState::On;
            }
    }
    return RadioState::Off;
}

// ---- Dispatched work (runs on the main task) ----

void dispatchSetEnabled(bool enabled) {
    LOG_I(TAG, "dispatchSetEnabled(%d)", (int)enabled);
    if (!started || state.device == nullptr) return;

    bool ready = device_is_ready(state.device);
    if (enabled == ready) {
        LOG_W(TAG, "Can't enable/disable from current state");
        return;
    }

    if (enabled) {
        publishRadioState(WIFI_RADIO_STATE_ON_PENDING);

#ifdef ESP_PLATFORM
        LOG_I(TAG, "WiFi init headroom: internal_free=%u internal_largest=%u", (unsigned)heap_caps_get_free_size(MALLOC_CAP_INTERNAL),
            (unsigned)heap_caps_get_largest_free_block(MALLOC_CAP_INTERNAL));
#endif
        if (device_start(state.device) != ERROR_NONE) {
            LOG_E(TAG, "Failed to start WiFi device");
            publishRadioState(WIFI_RADIO_STATE_OFF);
            return;
        }

        if (wifi_add_event_callback(state.device, nullptr, onWifiDeviceEvent) != ERROR_NONE) {
            LOG_E(TAG, "Failed to register WiFi event callback");
            device_stop(state.device);
            publishRadioState(WIFI_RADIO_STATE_OFF);
            return;
        }

        state.pauseAutoConnect = false;
        state.userDisconnected = false;
        state.lastScanTime = 0;
        publishRadioState(WIFI_RADIO_STATE_ON);

        // Get online immediately instead of waiting for onAutoConnectTimer() to become eligible:
        // shouldScanForAutoConnect() compares against absolute uptime, so lastScanTime=0 means
        // "once the system has been up AUTO_SCAN_INTERVAL", not "now". This also skips the full
        // dual-band scan entirely when a saved network is available.
        getMainDispatcher().dispatch([] { dispatchInitialAutoConnect(); });
    } else {
        publishRadioState(WIFI_RADIO_STATE_OFF_PENDING);

        if (device_stop(state.device) != ERROR_NONE) {
            LOG_E(TAG, "Failed to stop WiFi device");
            publishRadioState(WIFI_RADIO_STATE_ON);
            return;
        }

        wifi_remove_event_callback(state.device, onWifiDeviceEvent);

        state.secureConnection = false;
        publishRadioState(WIFI_RADIO_STATE_OFF);
    }
}

void dispatchScan() {
    LOG_I(TAG, "dispatchScan()");
    if (!started || state.device == nullptr || !device_is_ready(state.device)) return;

    state.lastScanTime = get_ticks();

    error_t result = wifi_scan(state.device);
    if (result != ERROR_NONE) {
        LOG_I(TAG, "Can't start scan (%s)", error_to_string(result));
    }
}

// Finds a saved auto-connect AP without needing scan results. esp_wifi_connect() does its own
// targeted search for the SSID, so a full discovery sweep is unnecessary just to get online.
bool findSavedAutoConnectAp(settings::WifiApSettings& out) {
    for (const auto& ssid : settings::getSavedSsids()) {
        settings::WifiApSettings loaded;
        if (settings::load(ssid, loaded) && loaded.autoConnect) {
            out = loaded;
            return true;
        }
    }
    return false;
}

// Runs once when the radio comes up. A full scan on this dual-band part sweeps ~38 channels
// across 2.4GHz and 5GHz (5GHz DFS channels must be scanned passively) and costs ~10s, which is
// pure latency when we already know which network we want. If nothing is saved we fall back to
// scanning so the UI still gets a network list and auto-connect can pick something up later.
void dispatchInitialAutoConnect() {
    if (!started || state.device == nullptr || !device_is_ready(state.device)) return;

    settings::WifiApSettings target;
    if (!state.userDisconnected && findSavedAutoConnectAp(target)) {
        LOG_I(TAG, "Auto-connecting to %s without scanning first", target.ssid.c_str());
        connect(target, false);
        // connect() pauses auto-connect (it assumes a manual/user call); undo that since this
        // call was automatic. A failed attempt also unpauses via STATION_CONNECTION_RESULT,
        // after which the periodic scan-based auto-connect takes over.
        state.pauseAutoConnect = false;
    } else {
        dispatchScan();
    }
}

void dispatchConnect() {
    LOG_I(TAG, "dispatchConnect()");
    if (!started || state.device == nullptr) return;

    settings::WifiApSettings target;
    {
        auto lock = state.mutex.asScopedLock();
        if (!lock.lock(50 / portTICK_PERIOD_MS)) {
            LOG_E(TAG, LOG_MESSAGE_MUTEX_LOCK_FAILED_FMT, "dispatchConnect()");
            return;
        }
        target = state.connectionTarget;
    }

    LOG_I(TAG, "Connecting to %s", target.ssid.c_str());

    error_t result = wifi_station_connect(state.device, target.ssid.c_str(), target.password.c_str(), target.channel);
    if (result != ERROR_NONE) {
        LOG_E(TAG, "Failed to connect to %s (%s)", target.ssid.c_str(), error_to_string(result));
        WifiEvent event = {};
        event.type = WIFI_EVENT_TYPE_STATION_CONNECTION_RESULT;
        // The driver couldn't even initiate the connection attempt; there's no
        // more specific WifiStationConnectionError for that.
        event.connection_error = WIFI_STATION_CONNECTION_ERROR_TIMEOUT;
        publish(event);
    }
    // On success, WIFI_EVENT_TYPE_STATION_STATE_CHANGED / _CONNECTION_RESULT arrive
    // asynchronously via onWifiDeviceEvent().
}

void dispatchDisconnect() {
    LOG_I(TAG, "dispatchDisconnect()");
    if (!started || state.device == nullptr) return;

    error_t result = wifi_station_disconnect(state.device);
    if (result != ERROR_NONE) {
        LOG_E(TAG, "Failed to disconnect (%s)", error_to_string(result));
    }
    // The Disconnected event arrives asynchronously via onWifiDeviceEvent().
}

bool findAutoConnectAp(settings::WifiApSettings& out) {
    for (const auto& record : getScanResults()) {
        if (settings::contains(record.ssid)) {
            settings::WifiApSettings loaded;
            if (settings::load(record.ssid, loaded)) {
                if (loaded.autoConnect) {
                    out = loaded;
                    return true;
                }
            } else {
                LOG_E(TAG, "Failed to load credentials for ssid %s", record.ssid);
            }
        }
    }
    return false;
}

void dispatchAutoConnect() {
    LOG_I(TAG, "dispatchAutoConnect()");
    if (state.userDisconnected) {
        // The user deliberately disconnected; only an explicit connect() may put them back on.
        return;
    }
    if (state.pauseAutoConnect || state.externalScanPause.load()) {
        // A manual disconnect() or an in-progress manual connect() has paused
        // auto-connect, or a caller (e.g. AutoScanPauseGuard) has externally paused it.
        // This is called on every SCAN_FINISHED, not just the auto-connect timer's own
        // scans (e.g. WifiManage re-scans on show), so it must honor the pause instead of
        // reconnecting unconditionally.
        return;
    }
    RadioState radio_state = getRadioState();
    if (radio_state == RadioState::ConnectionActive || radio_state == RadioState::ConnectionPending) {
        // Already connected (or connecting): reconnecting to the same AP would just
        // force a pointless disconnect/reconnect blip, e.g. when WifiManage's
        // on-show scan finishes while we're already on the saved auto-connect AP.
        return;
    }
    settings::WifiApSettings target;
    if (findAutoConnectAp(target)) {
        LOG_I(TAG, "Auto-connecting to %s", target.ssid.c_str());
        connect(target, false);
        // connect() pauses auto-connect (it assumes a manual/user call); undo that
        // since this call was automatic.
        state.pauseAutoConnect = false;
    }
}

// Gates the periodic scan that keeps the network list fresh. Deliberately *not* tied to
// auto-connect eligibility: RadioState::On only means "on and not connected", so gating on it
// alone stopped all scanning the moment we associated - the user then had to toggle the radio
// off and on to see any available networks. Auto-connect has its own guards in
// dispatchAutoConnect(), so scanning while connected is safe.
bool shouldScanPeriodically() {
    const auto radio_state = getRadioState();
    // ConnectionPending is excluded on purpose: scanning mid-association can disrupt it.
    const bool radio_scannable = (radio_state == RadioState::On || radio_state == RadioState::ConnectionActive) &&
        !isScanning() && !state.pauseAutoConnect && !state.externalScanPause.load();
    if (!radio_scannable) return false;

    TickType_t current_time = get_ticks();
    bool scan_time_has_looped = current_time < state.lastScanTime;
    bool no_recent_scan = (current_time - state.lastScanTime) > (AUTO_SCAN_INTERVAL / portTICK_PERIOD_MS);
    return scan_time_has_looped || no_recent_scan;
}

void onAutoConnectTimer() {
    if (!started || state.device == nullptr) return;
    if (shouldScanPeriodically()) {
        getMainDispatcher().dispatch([] { dispatchScan(); });
    }
}

// ---- Kernel driver event bridge ----

void onWifiDeviceEvent(Device* device, void* /*context*/, ::WifiEvent event) {
    switch (event.type) {
        case WIFI_EVENT_TYPE_SCAN_FINISHED:
            getMainDispatcher().dispatch([] { dispatchAutoConnect(); });
            break;

        case WIFI_EVENT_TYPE_STATION_STATE_CHANGED:
            if (event.station_state == WIFI_STATION_STATE_DISCONNECTED) {
                // Don't touch pauseAutoConnect here: it tracks an in-flight connection attempt
                // and is cleared by WIFI_EVENT_TYPE_STATION_CONNECTION_RESULT below. A
                // deliberate disconnect is tracked separately via userDisconnected.
                NetworkDisconnectedEvent disconnected_event = { .device = device };
                system_event_emit(KERNEL_EVENT_NETWORK_DISCONNECTED, &disconnected_event, sizeof(disconnected_event));

                if (state.userDisconnected) {
                    // Refresh the network list right away so the user can pick a different AP.
                    // The periodic scan can't be relied on here: WifiManage externally pauses
                    // auto-scan for as long as it is open, which is exactly when this happens.
                    getMainDispatcher().dispatch([] { dispatchScan(); });
                }
            }
            break;

        case WIFI_EVENT_TYPE_STATION_CONNECTION_RESULT:
            if (event.connection_error == WIFI_STATION_CONNECTION_ERROR_NONE) {
                settings::WifiApSettings target;
                bool remember;
                {
                    auto lock = state.mutex.asScopedLock();
                    if (lock.lock(50 / portTICK_PERIOD_MS)) {
                        target = state.connectionTarget;
                        remember = state.connectionTargetRemember;
                        state.secureConnection = !target.password.empty();
                    } else {
                        remember = false;
                    }
                }
                {
                    auto lock = state.mutex.asScopedLock();
                    if (lock.lock(50 / portTICK_PERIOD_MS)) {
                        state.pauseAutoConnect = false;
                    }
                }
                LOG_I(TAG, "Connected to %s", target.ssid.c_str());
                if (remember && !settings::save(target)) {
                    LOG_E(TAG, "Failed to store credentials");
                }
            } else {
                // The pending connection attempt (which paused auto-connect via connect())
                // failed; unpause so auto-connect can try other saved APs.
                auto lock = state.mutex.asScopedLock();
                if (lock.lock(50 / portTICK_PERIOD_MS)) {
                    state.pauseAutoConnect = false;
                }
            }
            break;

        default:
            break;
    }

    // Forward the event as-is: subscribers inspect event.type and the
    // relevant union field directly, same as this function does.
    publish(event);
}

void autoScanSetPaused(bool paused) {
    LOG_I(TAG, "autoScanSetPaused(%d)", (int)paused);
    state.externalScanPause = paused;
}

} // namespace

// region Public functions

std::shared_ptr<PubSub<WifiEvent>> getPubsub() {
    return state.pubsub;
}

RadioState getRadioState() {
    if (!started || state.device == nullptr || !device_is_ready(state.device)) {
        return RadioState::Off;
    }

    WifiRadioState radio = WIFI_RADIO_STATE_OFF;
    WifiStationState station = WIFI_STATION_STATE_DISCONNECTED;
    wifi_get_radio_state(state.device, &radio);
    wifi_get_station_state(state.device, &station);
    return combineRadioState(radio, station);
}

std::string getConnectionTarget() {
    RadioState radio_state = getRadioState();
    if (radio_state != RadioState::ConnectionPending && radio_state != RadioState::ConnectionActive) {
        return "";
    }

    char ssid[33] = {};
    if (wifi_station_get_target_ssid(state.device, ssid) != ERROR_NONE) {
        return "";
    }
    return { ssid };
}

void scan() {
    LOG_I(TAG, "scan()");
    if (!started || state.device == nullptr) return;
    getMainDispatcher().dispatch([] { dispatchScan(); });
}

bool isScanning() {
    if (!started || state.device == nullptr) return false;
    return wifi_is_scanning(state.device);
}

void connect(const settings::WifiApSettings& ap, bool remember) {
    LOG_I(TAG, "connect(%s, %d)", ap.ssid.c_str(), (int)remember);
    if (!started || state.device == nullptr) return;

    bool radio_off;
    {
        auto lock = state.mutex.asScopedLock();
        if (!lock.lock(10 / portTICK_PERIOD_MS)) {
            LOG_E(TAG, LOG_MESSAGE_MUTEX_LOCK_FAILED);
            return;
        }
        // Stop auto-connecting until the connection is established.
        state.pauseAutoConnect = true;
        // Picking a network (or an auto-connect that got past the userDisconnected guard)
        // ends the "stay disconnected" state.
        state.userDisconnected = false;
        state.connectionTarget = ap;
        state.connectionTargetRemember = remember;
        radio_off = !device_is_ready(state.device);
    }

    getMainDispatcher().dispatch([radio_off] {
        if (radio_off) {
            dispatchSetEnabled(true);
        }
        dispatchConnect();
    });
}

void disconnect() {
    LOG_I(TAG, "disconnect()");
    if (!started || state.device == nullptr) return;

    {
        auto lock = state.mutex.asScopedLock();
        if (!lock.lock(10 / portTICK_PERIOD_MS)) {
            LOG_E(TAG, LOG_MESSAGE_MUTEX_LOCK_FAILED);
            return;
        }
        state.connectionTarget = settings::WifiApSettings("", "");
        // A manual disconnect must survive until the user picks a network again. Note this
        // deliberately does not set pauseAutoConnect: that also suppresses scanning, which
        // would leave the network list frozen on the AP the user just left.
        state.userDisconnected = true;
        state.pauseAutoConnect = false;
    }

    getMainDispatcher().dispatch([] { dispatchDisconnect(); });
}

void setAutoScanPaused(bool paused) {
    autoScanSetPaused(paused);
}

void setScanRecords(uint16_t records) {
    LOG_I(TAG, "setScanRecords(%u)", records);
    if (!started) return;
    auto lock = state.mutex.asScopedLock();
    if (lock.lock(10 / portTICK_PERIOD_MS)) {
        state.scanRecordLimit = records;
    }
}

std::vector<WifiApRecord> getScanResults() {
    std::vector<WifiApRecord> records;
    if (!started || state.device == nullptr) return records;

    records.resize(state.scanRecordLimit);
    size_t count = records.size();
    if (wifi_get_scan_results(state.device, records.data(), &count) != ERROR_NONE) {
        records.clear();
        return records;
    }

    records.resize(count);
    return records;
}

void setEnabled(bool enabled) {
    LOG_I(TAG, "setEnabled(%d)", (int)enabled);
    if (!started || state.device == nullptr) return;
    getMainDispatcher().dispatch([enabled] { dispatchSetEnabled(enabled); });
}

bool isConnectionSecure() {
    return state.secureConnection;
}

int getRssi() {
    if (!started || state.device == nullptr) return 1;
    int32_t rssi = 0;
    if (wifi_station_get_rssi(state.device, &rssi) == ERROR_NONE) {
        return rssi;
    }
    return 1;
}

std::string getIp() {
    if (!started || state.device == nullptr) return "";
    char ipv4[16] = {};
    if (wifi_station_get_ipv4_address(state.device, ipv4) != ERROR_NONE) {
        return "";
    }
    return { ipv4 };
}

// endregion Public functions

namespace {

void onBootCompleted(struct SystemEvent* /*event*/, void* /*context*/) {
    bootSplashInit();
}

class WifiService final : public Service {

public:

    bool onStart(ServiceContext& /*service*/) override {
        check(!started);

        wifi_auto_scan_set_paused_function(autoScanSetPaused);

        state.device = wifi_find_first_registered_device();
        if (state.device == nullptr) {
            LOG_W(TAG, "No WiFi device found");
        }

        if (system_event_callback_add(KERNEL_EVENT_BOOT_COMPLETED, onBootCompleted, nullptr) == ERROR_NONE) {
            state.bootEventSubscribed = true;
        }

        auto timer_interval = std::min(2000, AUTO_SCAN_INTERVAL);
        state.autoConnectTimer = std::make_unique<Timer>(Timer::Type::Periodic, timer_interval, [] { onAutoConnectTimer(); });
        // We want to try and scan more often in case of startup or scan lock failure.
        state.autoConnectTimer->start();

        started = true;
        return true;
    }

    void onStop(ServiceContext& /*service*/) override {
        check(started);
        started = false;

        state.autoConnectTimer->stop();
        state.autoConnectTimer = nullptr; // Must release as it holds a reference via its callback.

        if (state.bootEventSubscribed) {
            system_event_callback_remove(KERNEL_EVENT_BOOT_COMPLETED, onBootCompleted);
            state.bootEventSubscribed = false;
        }

        if (state.device != nullptr && device_is_ready(state.device)) {
            wifi_remove_event_callback(state.device, onWifiDeviceEvent);
            device_stop(state.device);
        }

        state.secureConnection = false;
        state.pauseAutoConnect = false;
        state.device = nullptr;

        wifi_auto_scan_set_paused_function(nullptr);
    }
};

} // namespace

extern const ServiceManifest manifest = {
    .id = "wifi",
    .createService = create<WifiService>
};

} // namespace tt::service::wifi
