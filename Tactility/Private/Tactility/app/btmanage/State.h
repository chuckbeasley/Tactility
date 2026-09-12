#pragma once

#include <Tactility/bluetooth/Bluetooth.h>
#include <Tactility/RecursiveMutex.h>

#include <array>

namespace tt::app::btmanage {

class State final {

    mutable RecursiveMutex mutex;
    bool scanning = false;
    bluetooth::RadioState radioState = bluetooth::RadioState::Off;
    std::vector<bluetooth::PeerRecord> scanResults;
    std::vector<bluetooth::PeerRecord> pairedPeers;
    // The peer a connection is currently being made to. The Bluetooth API has no state for this: it
    // reports whether a peer *is* connected (PeerRecord.connected) and not whether one is being
    // connected to, so the app tracks it from the click until an outcome event arrives.
    std::array<uint8_t, 6> connectingAddr = {};
    bool connecting = false;

public:
    State() = default;

    void setScanning(bool isScanning);
    bool isScanning() const;

    void setRadioState(bluetooth::RadioState state);
    bluetooth::RadioState getRadioState() const;

    void updateScanResults();
    void updatePairedPeers();

    /** Marks a connection as in flight to `addr`, so the view can show it as pending. */
    void beginConnecting(const std::array<uint8_t, 6>& addr);
    /** Clears any in-flight connection, whether it succeeded, failed or timed out. */
    void endConnecting();
    /** @return true when a connection to this exact peer is in flight. */
    bool isConnectingTo(const std::array<uint8_t, 6>& addr) const;
    /** @return true when any connection is in flight. */
    bool isConnecting() const;

    std::vector<bluetooth::PeerRecord> getScanResults() const {
        auto lock = mutex.asScopedLock();
        lock.lock();
        return scanResults;
    }

    std::vector<bluetooth::PeerRecord> getPairedPeers() const {
        auto lock = mutex.asScopedLock();
        lock.lock();
        return pairedPeers;
    }

    // Cheap accessors for change detection: the vector getters above deep-copy every record
    // (each holds a std::string name), which is far too costly to run on every BT event.
    size_t getScanResultCount() const {
        auto lock = mutex.asScopedLock();
        lock.lock();
        return scanResults.size();
    }

    size_t getPairedPeerCount() const {
        auto lock = mutex.asScopedLock();
        lock.lock();
        return pairedPeers.size();
    }

    // Cheap change-detection accessor (no string copies). Counts paired peers currently marked
    // connected, so the view can rebuild when a keyboard connects/disconnects even when the
    // total count is unchanged.
    size_t getConnectedPeerCount() const {
        auto lock = mutex.asScopedLock();
        lock.lock();
        size_t count = 0;
        for (const auto& peer : pairedPeers) {
            if (peer.connected) count++;
        }
        return count;
    }
};

} // namespace tt::app::btmanage
