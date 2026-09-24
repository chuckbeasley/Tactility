#include <Tactility/app/btmanage/BtManagePrivate.h>

#include <tactility/log.h>

#include <cstdlib>

namespace tt::app::btmanage {

namespace {
constexpr auto* TAG = "BtManage";
}

void State::setScanning(bool isScanning) {
    auto lock = mutex.asScopedLock();
    lock.lock();
    scanning = isScanning;
}

bool State::isScanning() const {
    auto lock = mutex.asScopedLock();
    lock.lock();
    return scanning;
}

void State::setRadioState(bluetooth::RadioState s) {
    auto lock = mutex.asScopedLock();
    lock.lock();
    radioState = s;
}

bluetooth::RadioState State::getRadioState() const {
    auto lock = mutex.asScopedLock();
    lock.lock();
    return radioState;
}

void State::updateScanResults() {
    // Fetch outside the lock to avoid holding it during a service call.
    auto results = bluetooth::getScanResults();
    auto lock = mutex.asScopedLock();
    lock.lock();
    scanResults = std::move(results);
}

void State::updatePairedPeers() {
    auto peers = bluetooth::getPairedPeers();
    auto lock = mutex.asScopedLock();
    lock.lock();
    pairedPeers = std::move(peers);
}

bool State::updateConnectedRssi() {
    // The connected peer's address, read under the lock, then the controller is asked outside it.
    std::array<uint8_t, 6> connected_addr = {};
    int8_t peer_tx_power = 0x7F;
    bool have_connected = false;
    {
        auto lock = mutex.asScopedLock();
        lock.lock();
        for (const auto& peer : pairedPeers) {
            if (peer.connected) {
                connected_addr = peer.addr;
                peer_tx_power = peer.txPower;
                have_connected = true;
                break;
            }
        }
    }

    int8_t rssi = 0;
    const bool valid = have_connected && bluetooth::getConnectionRssi(connected_addr, rssi);

    bool changed = false;
    {
        auto lock = mutex.asScopedLock();
        lock.lock();
        changed = (valid != connectedRssiValid) || (valid && rssi != connectedRssi);
        // Logged on a 3 dB move rather than per refresh: this runs a couple of times a second, and the
        // point of the trace is to show what changed when the keyboard is moved. The peer's advertised
        // TX power goes in the same line so the log alone carries the path loss this reading implies.
        if (valid && (!connectedRssiValid || std::abs((int)rssi - (int)connectedRssi) >= 3)) {
            if (peer_tx_power != 0x7F) {
                LOG_I(TAG, "HID link RSSI: %d dBm (peer TX %+d dBm, path %d dB)", (int)rssi,
                      (int)peer_tx_power, (int)peer_tx_power - (int)rssi);
            } else {
                LOG_I(TAG, "HID link RSSI: %d dBm (peer advertises no TX power)", (int)rssi);
            }
        } else if (!valid && connectedRssiValid) {
            LOG_I(TAG, "HID link RSSI: no reading");
        }
        connectedRssi = rssi;
        connectedRssiValid = valid;
    }
    return changed;
}

bool State::getConnectedRssi(int8_t& out_rssi) const {
    auto lock = mutex.asScopedLock();
    lock.lock();
    if (!connectedRssiValid) {
        return false;
    }
    out_rssi = connectedRssi;
    return true;
}

void State::beginConnecting(const std::array<uint8_t, 6>& addr) {
    auto lock = mutex.asScopedLock();
    lock.lock();
    connectingAddr = addr;
    connecting = true;
}

void State::endConnecting() {
    auto lock = mutex.asScopedLock();
    lock.lock();
    connecting = false;
}

bool State::isConnectingTo(const std::array<uint8_t, 6>& addr) const {
    auto lock = mutex.asScopedLock();
    lock.lock();
    return connecting && connectingAddr == addr;
}

bool State::isConnecting() const {
    auto lock = mutex.asScopedLock();
    lock.lock();
    return connecting;
}

void State::setScanWanted(bool wanted) {
    auto lock = mutex.asScopedLock();
    lock.lock();
    scanWanted = wanted;
}

bool State::isScanWanted() const {
    auto lock = mutex.asScopedLock();
    lock.lock();
    return scanWanted;
}

} // namespace tt::app::btmanage
