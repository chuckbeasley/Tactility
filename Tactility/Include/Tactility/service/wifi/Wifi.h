#pragma once

#include <Tactility/PubSub.h>

#include <memory>
#include <string>
#include <vector>

#include <tactility/drivers/wifi.h>

#include "WifiApSettings.h"

namespace tt::service::wifi {

/** The kernel wifi driver's event type: a WifiEventType tag plus a union
 * payload (radio_state/station_state/access_point_state/connection_error
 * depending on the tag). See tactility/drivers/wifi.h. */
using WifiEvent = ::WifiEvent;

enum class RadioState {
    OnPending,
    On,
    ConnectionPending,
    ConnectionActive,
    OffPending,
    Off,
};

/** @return Get the current radio state */
RadioState getRadioState();

/** For logging purposes */
const char* radioStateToString(RadioState state);

/**
 * @brief Request scanning update. Returns immediately. Results are through pubsub.
 */
void scan();

/** @return true if wifi is actively scanning */
bool isScanning();

/** @return true the ssid name or empty string */
std::string getConnectionTarget();

/** @return the access points from the last scan (if any). It only contains public APs. */
std::vector<WifiApRecord> getScanResults();

/**
 * @brief Overrides the default scan result size of 16.
 * @param[in] records the record limit for the scan result (84 bytes per record!)
 */
void setScanRecords(uint16_t records);

/**
 * @brief Enable/disable the radio. Ignores input if desired state matches current state.
 * @param[in] enabled
 */
void setEnabled(bool enabled);

/**
 * @return the IPv4 address or empty string
 */
std::string getIp();

/**
 * @brief Connect to a network. Disconnects any existing connection.
 * Returns immediately but runs in the background. Results are through pubsub.
 * @param[in] ap
 * @param[in] remember whether to save the ap data to the settings upon successful connection
 */
void connect(const settings::WifiApSettings& ap, bool remember);

/** @brief Disconnect from the access point. Doesn't have any effect when not connected. */
void disconnect();

/** @return true if the connection isn't unencrypted. */
bool isConnectionSecure();

/**
 * @brief Suspend or resume the periodic background auto-connect scan.
 * @param[in] paused when true, the auto-connect timer stops issuing scans until resumed.
 * Intended for narrow, short-lived windows where any WiFi/co-processor traffic would be
 * unsafe (e.g. a known co-processor reboot in progress) - callers must resume when done.
 * Independent of the internal connect()/disconnect() auto-connect pause bookkeeping: only
 * a matching setAutoScanPaused(false) clears this, it is never cleared implicitly by a
 * connection succeeding/failing or the radio being enabled.
 */
void setAutoScanPaused(bool paused);

/**
 * @brief Enable or disable WiFi power save.
 * @param[in] enabled true to let the radio sleep between AP beacons (the IDF default,
 * WIFI_PS_MIN_MODEM); false to keep it awake (WIFI_PS_NONE).
 *
 * Power save is the right trade for battery life, but it puts up to a full beacon interval of
 * latency on every round trip, because the radio only wakes to hear the access point once per
 * beacon. Measured on the screen mirror: a WebSocket reply that did no work at all still took
 * ~103 ms, matching the 102.4 ms beacon interval this AP logs at boot - far above the cost of
 * producing a frame. Callers that need low latency disable it for the duration of their session
 * and re-enable it afterwards; nothing is persisted, and the state is not restored on reboot.
 * Does nothing when the radio is off.
 */
void setPowerSaveEnabled(bool enabled);

/** @return the RSSI value (negative number) or return 1 when not connected. */
int getRssi();

} // namespace
