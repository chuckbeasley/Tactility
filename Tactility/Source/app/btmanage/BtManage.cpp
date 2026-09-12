#include <lvgl/lvgl.h>

#include <Tactility/app/btmanage/BtManagePrivate.h>
#include <Tactility/app/btmanage/View.h>

#include <app/event.h>
#include <app/manager.h>
#include <app/start.h>
#include <app/manifest.h>
#include <app/scheduler.h>

#include <lvgl_window_manager/window_manager.h>

#include <tactility/check.h>
#include <tactility/device.h>
#include <tactility/log.h>

namespace tt::app::btmanage {

constexpr auto* TAG = "BtManage";

extern const ::AppManifest manifest;

// The BT device's event queue only exists while the radio is on: the driver allocates its context
// (and with it the subscription list) in start_device, and frees it on stop. An app opened with the
// radio off therefore can't subscribe yet, so it retries at this cadence until the radio comes up.
constexpr TickType_t EVENT_SUBSCRIBE_RETRY_TICKS = pdMS_TO_TICKS(250);

// How often the state is re-read and the view rebuilt while events are arriving. A scan delivers a
// burst of BT_EVENT_PEER_FOUND events, and each one used to trigger a full peer-list copy plus an
// LVGL lock - enough work to keep this task runnable continuously and starve the idle task, which is
// what tripped the task watchdog when Scan was pressed. Ten refreshes a second is imperceptible in
// the list and leaves the CPU to everything else.
constexpr TickType_t VIEW_REFRESH_INTERVAL_TICKS = pdMS_TO_TICKS(100);

static void onBtToggled(void* context, bool requestOn) {
#if defined(CONFIG_BT_NIMBLE_ENABLED)
    auto* ctx = static_cast<Context*>(context);
    Device* dev;
    if (device_get_first_by_type(&BLUETOOTH_TYPE, &dev) == ERROR_NONE) {
        bool radio_on = bluetooth::isRadioOnOrPending(dev);
        if (requestOn && !radio_on) {
            LOG_I(TAG, "Turning on");
            bluetooth::start(dev);
            // Show the transition immediately. The radio (and with it our BT event subscription)
            // comes up asynchronously, so until then the view would redraw the switch from the
            // still-"off" state and it would look like it flipped itself back off.
            if (ctx != nullptr) {
                ctx->state.setRadioState(bluetooth::RadioState::OnPending);
                requestViewUpdate(ctx);
            }
        } else if (!requestOn && radio_on) {
            LOG_I(TAG, "Turning off");
            bluetooth::stop(dev);
            if (ctx != nullptr) {
                ctx->state.setRadioState(bluetooth::RadioState::OffPending);
                requestViewUpdate(ctx);
            }
        }
        device_put(dev);
    } else {
        LOG_W(TAG, "Toggle: No bluetooth device found");
    }

#endif
}

static void onScanToggled(void* /*context*/, bool enabled) {
    Device* dev;
    if (device_get_first_active_by_type(&BLUETOOTH_TYPE, &dev) != ERROR_NONE) {
        LOG_W(TAG, "Scan: No bluetooth device found");
        return;
    }

    if (enabled) {
        bluetooth_scan_start(dev);
    } else {
        bluetooth_scan_stop(dev);
    }

    device_put(dev);
}

static void onConnectPeer(const std::array<uint8_t, 6>& addr, int profileId) {
    bluetooth::connect(addr, profileId);
}

static void onDisconnectPeer(const std::array<uint8_t, 6>& addr, int profileId) {
    bluetooth::disconnect(addr, profileId);
}

static void onPairPeer(void* /*context*/, const std::array<uint8_t, 6>& addr) {
    // Clicking a device means the user has picked one, so stop looking for others first. A scan
    // holds the radio in a duty cycle that competes with the connection it is trying to make, and
    // it also keeps rebuilding the list underneath the click.
    Device* dev;
    if (device_get_first_active_by_type(&BLUETOOTH_TYPE, &dev) == ERROR_NONE) {
        if (bluetooth_is_scanning(dev)) {
            LOG_I(TAG, "Stopping scan to connect");
            bluetooth_scan_stop(dev);
        }
        device_put(dev);
    }

    // Clicking an unrecognised scan result initiates a HID host connection.
    // Bond exchange happens automatically during the first connection.
    bluetooth::hidHostConnect(addr);
}

static void onForgetPeer(const std::array<uint8_t, 6>& addr) {
    bluetooth::unpair(addr);
}

void requestViewUpdate(Context* ctx) {
    // Lock order must match appMain()'s setup/teardown: both run under the LVGL lock
    // and then take `ctx->mutex` internally. Taking `mutex` before lvgl_lock() here would
    // invert that order and deadlock against a concurrent teardown (GUI task holding the
    // LVGL lock, waiting on `mutex`; this task holding `mutex`, waiting on the LVGL lock) -
    // exactly what happens when BT events fire rapidly (e.g. during scanning) while the app
    // is closing.
    lvgl_lock();
    ctx->lock();
    ctx->view.update();
    ctx->unlock();
    lvgl_unlock();
}

void onBtEvent(Context* ctx, const BtEvent& event) {
    auto radio_state = bluetooth::getRadioState();
    ctx->state.setRadioState(radio_state);

    // Deliberately cheap. The expensive parts - reading the scan results (a deep copy of every
    // record) and rendering - are left to the app loop, which does them at most once per
    // VIEW_REFRESH_INTERVAL_TICKS. Doing them here meant once per event, and a scan delivers a burst
    // of events: the task then never blocked, the idle task never ran, and the task watchdog fired.
    //
    // The per-event LOG_I that used to be here is gone for the same reason: at 115200 baud each line
    // blocks this task for ~4 ms, which under a peer storm is far more than the work it describes.
    switch (event.type) {
        case BT_EVENT_SCAN_STARTED:
            ctx->state.setScanning(true);
            ctx->view_dirty = true;
            break;
        case BT_EVENT_SCAN_FINISHED:
            ctx->state.setScanning(false);
            ctx->scan_results_dirty = true;
            ctx->paired_peers_dirty = true;
            ctx->view_dirty = true;
            break;
        case BT_EVENT_PEER_FOUND:
            ctx->scan_results_dirty = true;
            ctx->view_dirty = true;
            break;
        case BT_EVENT_PAIR_RESULT:
            ctx->paired_peers_dirty = true;
            ctx->view_dirty = true;
            break;
        case BT_EVENT_PROFILE_STATE_CHANGED:
            ctx->scan_results_dirty = true;
            ctx->paired_peers_dirty = true;
            ctx->view_dirty = true;
            break;
        case BT_EVENT_RADIO_STATE_CHANGED:
            ctx->view_dirty = true;
            if (event.radio_state == BT_RADIO_STATE_ON) {
                ctx->paired_peers_dirty = true;
                Device* dev = nullptr;
                if (device_get_first_active_by_type(&BLUETOOTH_TYPE, &dev) == ERROR_NONE && !bluetooth_is_scanning(dev)) {
                    bluetooth_scan_start(dev);
                }
                if (dev) {
                    device_put(dev);
                }
            }
            break;
        default:
            break;
    }
}

void onBackPressed(lv_event_t* event) {
    auto* ctx = static_cast<Context*>(lv_event_get_user_data(event));
    app_event_emit_close(ctx->appInstanceId);
}

void createWidgets(lv_obj_t* parent, void* userData) {
    auto* ctx = static_cast<Context*>(userData);
    ctx->lock();
    ctx->view.init(ctx, parent);
    ctx->view.update();
    ctx->unlock();
}

int32_t appMain(int argc, char* argv[]) {
    uint32_t appInstanceId = app_scheduler_current_app_id();
    Context ctx;
    ctx.appInstanceId = appInstanceId;
    ctx.bindings = (Bindings) {
        .onBtToggled = onBtToggled,
        .onScanToggled = onScanToggled,
        .onConnectPeer = onConnectPeer,
        .onDisconnectPeer = onDisconnectPeer,
        .onPairPeer = onPairPeer,
        .onForgetPeer = onForgetPeer,
    };

    // Initialise state before subscribing to avoid incoming events racing with it.
    ctx.state.setRadioState(bluetooth::getRadioState());
    Device* dev = nullptr;
    device_get_first_by_type(&BLUETOOTH_TYPE, &dev);

    ctx.state.setScanning(dev ? bluetooth_is_scanning(dev) : false);
    ctx.state.updateScanResults();
    ctx.state.updatePairedPeers();

    TaskEventGroup event_group {};
    task_event_group_construct(&event_group);

    AppEventSubscription sub {};
    check(app_event_subscribe(&sub, &event_group) == ERROR_NONE);

    // dev is started for the process lifetime once ble0 is enabled in the devicetree -
    // subscribe once here rather than resubscribing on every bluetooth::start()/stop() toggle.
    if (dev != nullptr) {
        if (bluetooth_event_subscribe(dev, &ctx.btEventSub, &event_group) == ERROR_NONE) {
            ctx.btDevice = dev;
        } else {
            LOG_W(TAG, "Failed to subscribe to BT events");
        }
    }

    WindowId window = window_manager_create(appInstanceId, createWidgets, &ctx);

    auto radio_state = bluetooth::getRadioState();
    bool can_scan = radio_state == bluetooth::RadioState::On;
    LOG_I(TAG, "Radio: %s, Scanning: %d, Can scan: %d",
        bluetooth::radioStateToString(radio_state),
        (int)(dev ? bluetooth_is_scanning(dev) : false),
        (int)can_scan);
    if (can_scan && dev && !bluetooth_is_scanning(dev)) {
        bluetooth_scan_start(dev);
    }

    bool shouldClose = false;
    TickType_t last_refresh_ticks = 0;
    while (!shouldClose) {
        // While unsubscribed, wake on a timeout instead of blocking forever: the BT device only
        // gains its event queue once the radio is enabled, so an app opened with the radio off has
        // to keep retrying until then.
        //
        // Bounded even once subscribed, so that a refresh left pending by the last event in a burst
        // still happens after the events stop, rather than waiting for an event that never comes.
        const TickType_t wait = (ctx.btDevice != nullptr) ? VIEW_REFRESH_INTERVAL_TICKS : EVENT_SUBSCRIBE_RETRY_TICKS;
        task_event_group_wait_any(&event_group, nullptr, wait);

        AppEvent event {};
        while (app_event_poll(&sub, &event) == ERROR_NONE) {
            switch (event.type) {
                case APP_EVENT_CLOSE:
                    shouldClose = true;
                    break;
                default:
                    break;
            }
            if (shouldClose) break;
        }
        if (shouldClose) break;

        // The driver drops every subscription when the radio stops, so release ours and let the
        // retry below re-subscribe on the next enable - otherwise the app would keep a dead
        // subscription and never see events again.
        const auto radio = bluetooth::getRadioState();
        if (ctx.btDevice != nullptr && radio == bluetooth::RadioState::Off) {
            bluetooth_event_unsubscribe(ctx.btDevice, &ctx.btEventSub);
            ctx.btDevice = nullptr;
        }

        // Only (re)subscribe once the radio is actually up.
        //
        // This used to retry unconditionally while unsubscribed, on the assumption that subscribing
        // with the radio off would fail and the retry would simply wait for it to come up. It does
        // not fail - the driver accepts the subscription - so with the rule above dropping the
        // subscription because the radio is off, the two together oscillated: subscribe, see Off,
        // unsubscribe, subscribe again, ~100 times a second. That saturated this task with logging
        // (starving the idle task into a watchdog trip) and left the UI unable to respond, which is
        // why turning Bluetooth back on appeared to do nothing at all.
        //
        // Gating on the radio state loses nothing: this loop already polls the radio by calling
        // getRadioState() every pass, so it notices the radio coming up by itself, which is the
        // whole reason the retry exists.
        if (ctx.btDevice == nullptr && dev != nullptr && radio != bluetooth::RadioState::Off) {
            if (bluetooth_event_subscribe(dev, &ctx.btEventSub, &event_group) == ERROR_NONE) {
                ctx.btDevice = dev;
                LOG_I(TAG, "Subscribed to BT events (radio became available)");
                // This transition's event fired before we could subscribe, so read the current
                // values directly rather than wait for an event that will never arrive - but only
                // mark them, exactly as the event path does. Doing the reads and the render here
                // instead was the crash on re-enabling the radio: this is the path the radio comes
                // back through, and it rebuilt the whole peer list (see the stack note on the
                // manifest) in the middle of the loop.
                ctx.scan_results_dirty = true;
                ctx.paired_peers_dirty = true;
                ctx.view_dirty = true;
                ctx.state.setScanning(bluetooth_is_scanning(dev));
                // The radio turning on is what starts a scan, via BT_EVENT_RADIO_STATE_CHANGED -
                // but that event fires while we are still unsubscribed, so on a re-enable we miss
                // the only trigger there is and scanning stays stopped. The list then shows
                // whatever the previous scan happened to leave in the cache, which is why not every
                // available device appeared. Do what the missed event would have done.
                if (radio != bluetooth::RadioState::Off && !bluetooth_is_scanning(dev)) {
                    LOG_I(TAG, "Starting scan after re-subscribing");
                    bluetooth_scan_start(dev);
                }
                // getRadioState() reports Off whenever no BT device is active, which is also true
                // mid-enable, so don't let a transient Off overwrite the pending state.
                if (radio != bluetooth::RadioState::Off) {
                    ctx.state.setRadioState(radio);
                }
            }
        }

        if (ctx.btDevice != nullptr) {
            BtEvent bt_event {};
            while (bluetooth_event_poll(&ctx.btEventSub, &bt_event) == ERROR_NONE) {
                onBtEvent(&ctx, bt_event);
            }
        }

        // The coalesced half of the event handling: whatever the events marked dirty is read once
        // here and rendered once, however many events arrived in between. This is the only place
        // either happens for BT events or for the re-subscribe path, so neither a burst of
        // peer-found events nor the radio coming back can rebuild the list mid-loop.
        if (ctx.view_dirty) {
            const TickType_t now = xTaskGetTickCount();
            if ((TickType_t)(now - last_refresh_ticks) >= VIEW_REFRESH_INTERVAL_TICKS) {
                if (ctx.scan_results_dirty) {
                    ctx.state.updateScanResults();
                    ctx.scan_results_dirty = false;
                }
                if (ctx.paired_peers_dirty) {
                    ctx.state.updatePairedPeers();
                    ctx.paired_peers_dirty = false;
                }
                ctx.view_dirty = false;
                last_refresh_ticks = now;
                requestViewUpdate(&ctx);
            }
        }
    }

    if (ctx.btDevice) {
        bluetooth_event_unsubscribe(ctx.btDevice, &ctx.btEventSub);
        ctx.btDevice = nullptr;
    }

    if (dev != nullptr) {
        device_put(dev);
    }

    window_manager_remove(window);
    check(app_event_unsubscribe(&sub) == ERROR_NONE);
    task_event_group_destruct(&event_group);

    return 0;
}

uint32_t start() {
    uint32_t instanceId = 0;
    app_start(manifest.id, 0, nullptr, &instanceId);
    return instanceId;
}

extern const ::AppManifest manifest = {
    .id = "tactility.btmanage",
    .name = "Bluetooth",
    .category = APP_CATEGORY_SETTINGS,
    .location = { APP_LOCATION_MEMORY, reinterpret_cast<void*>(appMain) },
    // Deliberately back to 16 KB rather than the 32 KB tried to survive the list-rebuild stack
    // fault: that did not fix the fault (it still faulted with 32 KB) and the extra 16 KB of
    // internal RAM starved the Bluetooth driver, which allocates a node per discovered peer and
    // was logging "malloc addr node failed" - i.e. it could no longer record every device it found.
    // The depth is instead kept down by the coalesced refresh, which rebuilds the list at most once
    // per interval instead of once per event.
    .stack = { .depth = 4096 }, // 16 KB
};

} // namespace tt::app::btmanage
