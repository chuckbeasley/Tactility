#pragma once

#include "./View.h"
#include "./State.h"

#include <Tactility/Mutex.h>
#include <Tactility/bluetooth/Bluetooth.h>
#include <tactility/drivers/bluetooth.h>

namespace tt::app::btmanage {

struct Context {
    uint32_t appInstanceId;
    Mutex mutex;
    Bindings bindings {};
    State state;
    View view = View(&bindings, &state);
    Device* btDevice = nullptr;
    BtEventSubscription btEventSub {};

    // Set by the event handler and consumed by the app loop. Scanning produces a burst of
    // BT_EVENT_PEER_FOUND - one per peer, repeatedly - and refreshing the state for each one means
    // deep-copying the whole peer list (a std::string per record) and taking the LVGL lock, once per
    // event. That is enough to keep this task runnable continuously, which starves the idle task.
    // Only the app task touches these, so they need no synchronisation.
    bool scan_results_dirty = false;
    bool paired_peers_dirty = false;
    // Set by any event that changes something visible - including the ones that only touch
    // `scanning` or the radio state, which have no data to re-read but still need the view
    // redrawn. Without this, pressing "Stop scan" left the button reading "Stop scan" because
    // nothing asked for a redraw.
    bool view_dirty = false;
    // When the in-flight connection started, so it can be timed out. Only the app task writes it,
    // except for onPairPeer() which runs on the LVGL task but sets it at the moment it also sets the
    // connecting state, before any refresh can look at it.
    TickType_t connecting_since_ticks = 0;

    void lock() { mutex.lock(); }
    void unlock() { mutex.unlock(); }
};

void onBtEvent(Context* ctx, const struct BtEvent& event);
void requestViewUpdate(Context* ctx);

} // namespace tt::app::btmanage
