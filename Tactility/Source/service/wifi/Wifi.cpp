#include <Tactility/service/wifi/Wifi.h>

#include <Tactility/CoreDefines.h>
#include <Tactility/LogMessages.h>
#include <Tactility/RecursiveMutex.h>
#include <Tactility/Tactility.h>
#include <Tactility/Thread.h>
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
#include <esp_wifi.h>
#endif

namespace tt::service::wifi {

constexpr auto* TAG = "WifiService";
constexpr auto AUTO_SCAN_INTERVAL = 10000; // ms

// Consecutive failed association attempts after which auto-connect stops on its own. Measured on this
// board against cb2enterprises5G: every attempt reaches "wifi:state: assoc -> run (0x10)" and is torn
// down again with "run -> init (0xf00)" about three seconds later, IP_EVENT_STA_GOT_IP never arrives,
// and the service used to scan and re-associate every 15 seconds for as long as the board was on.
// Each attempt is a transmit burst that pulls the battery supply down far enough to stop the touch
// panel's capacitive sensing (measured: on battery, touch works with the radio off and fails with it
// on; on USB both work because the supply is ample). Three attempts ride out a genuine AP restart
// without leaving the radio retrying forever.
constexpr uint8_t AUTO_CONNECT_MAX_FAILURES = 3;

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
    RecursiveMutex mutex;
    bool secureConnection = false;
    // Internal: set by connect()/disconnect() while a manual attempt is in flight, cleared on
    // connection success/failure. Distinct from externalScanPause below - the two must not
    // clobber each other, otherwise a caller's explicit pause (e.g. AutoScanPauseGuard during a
    // co-processor OTA) can be silently cleared by an unrelated connect/disconnect finishing.
    bool pauseAutoConnect = false;
    // External: only setAutoScanPaused() may set/clear this. Read alongside pauseAutoConnect to
    // gate scan scheduling (both must be false to scan).
    std::atomic<bool> externalScanPause{false};
    bool connectionTargetRemember = false;
    settings::WifiApSettings connectionTarget;
    // Connection attempts that ended without an IP, counted consecutively. Auto-connect stops at
    // AUTO_CONNECT_MAX_FAILURES and only a manual connect() clears the count. Atomic because the
    // failures are recorded on the wifi-events thread (connection results) and read on the main task
    // (dispatchAutoConnect()/shouldScanForAutoConnect()) as well as by whichever task calls connect().
    std::atomic<uint8_t> autoConnectFailures{0};
    uint16_t scanRecordLimit = TT_WIFI_SCAN_RECORD_LIMIT;
    TickType_t lastScanTime = MAX_TICKS;
    std::unique_ptr<Timer> autoConnectTimer;
    bool bootEventSubscribed = false;

    // Dedicated consumer for WifiEvents, alive for the service's whole lifetime (started in
    // onStart(), stopped in onStop() - see dispatchSetEnabled()'s comment on why this outlives
    // radio on/off toggles): runs onWifiDeviceEvent() on its own stack instead of the ESP-IDF
    // esp_event task's, by blocking in task_event_group_wait_any() rather than being called back
    // directly from fire_event().
    TaskEventGroup wifiEventGroup {};
    WifiEventSubscription wifiEventSub {};
    Thread* wifiEventThread = nullptr;
    std::atomic<bool> wifiEventThreadRunning {false};
};

WifiServiceState state;
bool started = false;

/**
 * Whether power save is wanted, which is what the driver is set to except when the radio is off.
 *
 * Tracked rather than asked of the driver because the mirror and now the radar screen both borrow the
 * radio for a low-latency stretch and want to put back what they found.
 */
bool powerSaveEnabled = true;

void onWifiDeviceEvent(Device* device, ::WifiEvent event);

// ---- Helpers ----

// state.device is started (bookkeeping allocated) for the service's entire lifetime now - see
// dispatchSetEnabled()'s comment - so device_is_ready() no longer tracks radio-on state; query
// the driver directly instead.
bool isRadioOn() {
    if (state.device == nullptr) return false;
    WifiRadioState radio = WIFI_RADIO_STATE_OFF;
    return wifi_get_radio_state(state.device, &radio) == ERROR_NONE && radio == WIFI_RADIO_STATE_ON;
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

// ---- WifiEvent consumer thread ----
// Runs onWifiDeviceEvent() on its own stack (see WifiServiceState::wifiEventGroup's comment).

// Measured on the ESP32-C5: 808 bytes used. This thread only waits on an event group and forwards
// to the dispatcher, so 4096 was five times what it needs.
constexpr configSTACK_DEPTH_TYPE WIFI_EVENT_THREAD_STACK_SIZE = 2048;

int32_t wifiEventThreadMain() {
    // The 250ms timeout only bounds how promptly a stop request (wifiEventThreadRunning going
    // false) is noticed; a real event still wakes this immediately regardless, since
    // task_event_group_wait_any() returns as soon as the bit is signalled, whichever comes first.
    while (state.wifiEventThreadRunning.load()) {
        task_event_group_wait_any(&state.wifiEventGroup, nullptr, pdMS_TO_TICKS(250));

        WifiEvent event {};
        while (wifi_event_poll(&state.wifiEventSub, &event) == ERROR_NONE) {
            onWifiDeviceEvent(state.device, event);
        }
    }
    return 0;
}

bool startWifiEventThread() {
    task_event_group_construct(&state.wifiEventGroup);
    if (wifi_event_subscribe(state.device, &state.wifiEventSub, &state.wifiEventGroup) != ERROR_NONE) {
        task_event_group_destruct(&state.wifiEventGroup);
        return false;
    }

    state.wifiEventThreadRunning = true;
    state.wifiEventThread = new Thread("wifi-events", WIFI_EVENT_THREAD_STACK_SIZE, [] { return wifiEventThreadMain(); });
    state.wifiEventThread->start();
    return true;
}

void stopWifiEventThread() {
    if (state.wifiEventThread == nullptr) return;

    state.wifiEventThreadRunning = false;
    state.wifiEventThread->join();
    delete state.wifiEventThread;
    state.wifiEventThread = nullptr;

    wifi_event_unsubscribe(state.device, &state.wifiEventSub);
    task_event_group_destruct(&state.wifiEventGroup);
}

// ---- Dispatched work (runs on the main task) ----

// state.device is started (device_start()) once, in onStart(), and never stopped until onStop() -
// this only toggles the radio itself, so the wifi-events thread (and any app subscribed directly
// to the driver) stays subscribed across on/off toggles instead of having to resubscribe.
void dispatchSetEnabled(bool enabled) {
    LOG_I(TAG, "dispatchSetEnabled(%d)", (int)enabled);
    if (!started || state.device == nullptr) return;

    if (enabled == isRadioOn()) {
        LOG_W(TAG, "Can't enable/disable from current state");
        return;
    }

    if (enabled) {

        if (wifi_set_radio_on(state.device) != ERROR_NONE) {
            LOG_E(TAG, "Failed to enable WiFi radio");
            return;
        }

        state.pauseAutoConnect = false;
        state.lastScanTime = 0;
    } else {

        if (wifi_set_radio_off(state.device) != ERROR_NONE) {
            LOG_E(TAG, "Failed to disable WiFi radio");
            return;
        }

        state.secureConnection = false;
    }
}

void dispatchScan() {
    LOG_I(TAG, "dispatchScan()");
    if (!started || state.device == nullptr || !isRadioOn()) return;

    state.lastScanTime = get_ticks();

    error_t result = wifi_scan(state.device);
    if (result != ERROR_NONE) {
        LOG_I(TAG, "Can't start scan (%s)", error_to_string(result));
    }
}

// ---- Auto-connect retry budget ----

// Records one connection attempt that ended without an IP. At AUTO_CONNECT_MAX_FAILURES in a row,
// auto-connect stops: the failure has repeated identically every time, and each retry costs a scan
// plus an association burst on the supply that the touch panel shares (see the constant's comment).
// Both manual and automatic attempts are counted - connect() clears the count before a manual one, so
// a manual attempt that fails costs one of the budget rather than being exempt from it.
void recordConnectionFailure(const char* ssid) {
    const auto failures = state.autoConnectFailures.fetch_add(1) + 1;
    if (failures != AUTO_CONNECT_MAX_FAILURES) {
        return;
    }
    LOG_W(TAG,
        "Auto-connect stopped after %u consecutive attempts to %s failed: the station reaches run and is "
        "torn down again within seconds, without ever getting an IP, so retrying only repeats the "
        "scan-and-associate burst every 15s that drops touch sensing on battery. Connect manually to try again.",
        (unsigned)failures, ssid);
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
        // The driver refused the attempt outright, so no connection result event follows and this
        // failure would otherwise never reach the retry budget.
        LOG_E(TAG, "Failed to connect to %s (%s)", target.ssid.c_str(), error_to_string(result));
        recordConnectionFailure(target.ssid.c_str());
    }
}

// The shared body of connect(). The public entry point clears the failure count first; the automatic
// path in dispatchAutoConnect() deliberately does not, because an automatic attempt that reset the
// count would make AUTO_CONNECT_MAX_FAILURES unreachable.
void startConnection(const settings::WifiApSettings& ap, bool remember) {
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
        state.connectionTarget = ap;
        state.connectionTargetRemember = remember;
        radio_off = !isRadioOn();
    }

    getMainDispatcher().dispatch([radio_off] {
        if (radio_off) {
            dispatchSetEnabled(true);
        }
        dispatchConnect();
    });
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
    if (state.autoConnectFailures >= AUTO_CONNECT_MAX_FAILURES) {
        // Gave up on the saved AP after AUTO_CONNECT_MAX_FAILURES attempts in a row; the last failure
        // already logged why. Nothing automatic clears this - only a manual connect() does.
        return;
    }
    if (state.pauseAutoConnect || state.externalScanPause.load() || !isRadioOn()) {
        // A manual disconnect() or an in-progress manual connect() has paused
        // auto-connect, or a caller (e.g. AutoScanPauseGuard) has externally paused it.
        // This is called on every SCAN_FINISHED, not just the auto-connect timer's own
        // scans (e.g. WifiManage re-scans on show), so it must honor the pause instead of
        // reconnecting unconditionally. The radio-off check matters because a scan that was
        // already in flight can finish after the user turns the radio off. Without it,
        // connect() would call dispatchSetEnabled(true) and turn the radio back on.
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
        startConnection(target, false);
        // startConnection() pauses auto-connect (it assumes a manual/user call); undo that
        // since this call was automatic.
        state.pauseAutoConnect = false;
    }
}

bool shouldScanForAutoConnect() {
    // The periodic scan exists only to find an auto-connect AP, so once auto-connect has given up it
    // would just keep putting transmit bursts on the supply that stopping the retries removed. Scans
    // requested by apps (e.g. WifiManage on show) go through scan() and are unaffected.
    if (state.autoConnectFailures >= AUTO_CONNECT_MAX_FAILURES) return false;

    bool radio_scannable = getRadioState() == RadioState::On && !isScanning() &&
        !state.pauseAutoConnect && !state.externalScanPause.load();
    if (!radio_scannable) return false;

    TickType_t current_time = get_ticks();
    bool scan_time_has_looped = current_time < state.lastScanTime;
    bool no_recent_scan = (current_time - state.lastScanTime) > (AUTO_SCAN_INTERVAL / portTICK_PERIOD_MS);
    return scan_time_has_looped || no_recent_scan;
}

void onAutoConnectTimer() {
    if (!started || state.device == nullptr) return;
    if (shouldScanForAutoConnect()) {
        getMainDispatcher().dispatch([] { dispatchScan(); });
    }
}

// ---- Kernel driver event bridge ----

void onWifiDeviceEvent(Device* device, ::WifiEvent event) {
    switch (event.type) {
        case WIFI_EVENT_TYPE_SCAN_FINISHED:
            getMainDispatcher().dispatch([] { dispatchAutoConnect(); });
            break;

        case WIFI_EVENT_TYPE_STATION_STATE_CHANGED:
            if (event.station_state == WIFI_STATION_STATE_DISCONNECTED) {
                // Don't touch pauseAutoConnect here: a deliberate disconnect() sets it
                // and relies on it staying set until a new connection is established.
                // Resetting it on every disconnect (including deliberate ones) would
                // let auto-connect immediately reconnect the user. Attempts that fail
                // while pending are unpaused via WIFI_EVENT_TYPE_STATION_CONNECTION_RESULT below.
                NetworkDisconnectedEvent disconnected_event = { .device = device };
                system_event_emit(KERNEL_EVENT_NETWORK_DISCONNECTED, &disconnected_event, sizeof(disconnected_event));
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
                // A connection that got an IP clears the retry budget: whatever was wrong is gone.
                state.autoConnectFailures = 0;
                LOG_I(TAG, "Connected to %s", target.ssid.c_str());
                if (remember && !settings::save(target)) {
                    LOG_E(TAG, "Failed to store credentials");
                }
            } else {
                // The pending connection attempt (which paused auto-connect via connect())
                // failed; unpause so auto-connect can try other saved APs.
                std::string failed_ssid;
                auto lock = state.mutex.asScopedLock();
                if (lock.lock(50 / portTICK_PERIOD_MS)) {
                    state.pauseAutoConnect = false;
                    failed_ssid = state.connectionTarget.ssid;
                }
                // Counted outside the lock: recordConnectionFailure() logs when it gives up.
                recordConnectionFailure(failed_ssid.c_str());
            }
            break;

        default:
            break;
    }
}

void autoScanSetPaused(bool paused) {
    LOG_I(TAG, "autoScanSetPaused(%d)", (int)paused);
    state.externalScanPause = paused;
}

} // namespace

// region Public functions

RadioState getRadioState() {
    if (!started || state.device == nullptr) {
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
    // A manual request always goes through, and re-arms auto-connect: the user asking for this AP
    // (possibly after correcting its password) is new information, so it gets a fresh budget of
    // attempts even after the automatic ones in AUTO_CONNECT_MAX_FAILURES are used up.
    state.autoConnectFailures = 0;
    startConnection(ap, remember);
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
        // Manual disconnect (e.g. via app) should stop auto-connecting until a new connection is established.
        state.pauseAutoConnect = true;
    }

    getMainDispatcher().dispatch([] { dispatchDisconnect(); });
}

void setAutoScanPaused(bool paused) {
    autoScanSetPaused(paused);
}

void setPowerSaveEnabled(bool enabled) {
    powerSaveEnabled = enabled;
#ifdef ESP_PLATFORM
    // Only meaningful while the station is up; the driver rejects the call before that, and a
    // failed call here is logged rather than asserted because it is a latency optimisation, not
    // something the caller can do anything about.
    if (!isRadioOn()) {
        return;
    }

    const wifi_ps_type_t mode = enabled ? WIFI_PS_MIN_MODEM : WIFI_PS_NONE;
    const esp_err_t result = esp_wifi_set_ps(mode);
    if (result != ESP_OK) {
        LOG_W(TAG, "esp_wifi_set_ps(%d) failed: %s", (int)mode, esp_err_to_name(result));
    }
#else
    (void)enabled;
#endif
}

bool isPowerSaveEnabled() {
    return powerSaveEnabled;
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

// Both of these go into the Wi-Fi driver, whose API lock does not exist until the radio is on:
// calling one earlier faults inside the closed driver library rather than returning an error, and
// the fault is a null-pointer load at a fixed offset in current_task_is_wifi_task, reached from
// esp_wifi_sta_get_ap_info. Measured on the ESP32-C5: a caller polling RSSI 1 second into boot - the
// driver comes up around 29 seconds - put the board in a boot loop. `started` and the device being
// present are not enough on their own, because the service is started before the driver is ready;
// isRadioOn() is the same guard setPowerSaveEnabled() already relies on.
int getRssi() {
    if (!started || state.device == nullptr || !isRadioOn()) return 1;
    int32_t rssi = 0;
    if (wifi_station_get_rssi(state.device, &rssi) == ERROR_NONE) {
        return rssi;
    }
    return 1;
}

std::string getIp() {
    if (!started || state.device == nullptr || !isRadioOn()) return "";
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

        Device* wifi_device = nullptr;
        if (device_get_first_by_type(&WIFI_TYPE, &wifi_device) != ERROR_NONE) {
            LOG_W(TAG, "No WiFi device found");
        } else if (device_start(wifi_device) != ERROR_NONE) {
            LOG_E(TAG, "Failed to start WiFi device");
            device_put(wifi_device);
        } else {
            state.device = wifi_device;
            if (!startWifiEventThread()) {
                LOG_E(TAG, "Failed to subscribe to WiFi events");
            }
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

        if (state.device != nullptr) {
            if (isRadioOn()) {
                wifi_set_radio_off(state.device);
            }
            stopWifiEventThread();
            device_stop(state.device);
            device_put(state.device);
        }

        state.secureConnection = false;
        state.pauseAutoConnect = false;
        state.device = nullptr;

        wifi_auto_scan_set_paused_function(nullptr);
    }
};

} // namespace

extern const ServiceManifest manifest = {
    .id = "tactility.wifi",
    .createService = create<WifiService>
};

} // namespace tt::service::wifi
