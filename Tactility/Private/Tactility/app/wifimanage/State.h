#pragma once

#include <Tactility/service/wifi/Wifi.h>
#include <Tactility/RecursiveMutex.h>

namespace tt::app::wifimanage {

/**
 * View's state
 */
class State final {

    RecursiveMutex mutex;
    bool scanning = false;
    bool scannedAfterRadioOn = false;
    // True while an empty scan is being retried (see WifiManage.cpp): the screen uses it to keep
    // showing that it is still looking instead of claiming there are no networks yet.
    bool scanRetryPending = false;
    service::wifi::RadioState radioState;
    std::vector<WifiApRecord> apRecords;
    std::string connectSsid;

public:
    State() = default;

    void setScanning(bool isScanning);
    bool isScanning() const;

    void setScanRetryPending(bool pending);
    bool isScanRetryPending() const;

    bool hasScannedAfterRadioOn() const { return scannedAfterRadioOn; }

    void setRadioState(service::wifi::RadioState state);
    service::wifi::RadioState getRadioState() const;

    void updateApRecords();

    template <std::invocable<const std::vector<WifiApRecord>&> Func>
    void withApRecords(Func&& onApRecords) const {
        mutex.withLock([&] {
            std::invoke(std::forward<Func>(onApRecords), apRecords);
        });
    }

    std::vector<WifiApRecord> getApRecords() const {
        auto lock = mutex.asScopedLock();
        lock.lock();
        return apRecords;
    }
    size_t getApRecordCount() const {
        auto lock = mutex.asScopedLock();
        lock.lock();
        return apRecords.size();
    }

    void setConnectSsid(const std::string& ssid);
    std::string getConnectSsid() const;
};

} // namespace
