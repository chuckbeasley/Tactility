#include <format>
#include <string>

#include <lvgl/lvgl.h>

#include <Tactility/app/btmanage/View.h>
#include <Tactility/app/btmanage/BtManagePrivate.h>
#include <Tactility/app/btpeersettings/BtPeerSettings.h>
#include <Tactility/lvgl/Style.h>
#include <Tactility/lvgl/Toolbar.h>
#include <Tactility/bluetooth/Bluetooth.h>
#include <Tactility/bluetooth/BluetoothSettings.h>
#include <Tactility/bluetooth/BluetoothPairedDevice.h>
#include <Tactility/Tactility.h>

#include <app/event.h>
#include <lvgl/widgets/toolbar.h>

namespace tt::app::btmanage {

// Upper bound on peer-list rebuild frequency. A busy RF environment can produce dozens of
// BT_EVENT_PEER_FOUND per second and each rebuild is a full lv_obj_clean() + repopulate.
static constexpr uint32_t LIST_REBUILD_INTERVAL_MS = 750;

static void onBackPressed(lv_event_t* event) {
    auto* ctx = static_cast<Context*>(lv_event_get_user_data(event));
    // Async, non-blocking - must NOT call app_manager_stop() directly here: that bound-waits
    // (thread_join) for this app's own thread to finish, which needs the LVGL lock
    // (window_manager_remove()) - but this callback runs ON the LVGL task, which would
    // deadlock against itself.
    AppEvent closeEvent { .type = APP_EVENT_CLOSE, .timestamp = 0, .result = {} };
    app_event_emit(ctx->appInstanceId, &closeEvent);
}

static void onEnableSwitchChanged(lv_event_t* event) {
    auto* enable_switch = static_cast<lv_obj_t*>(lv_event_get_target(event));
    bool is_on = lv_obj_has_state(enable_switch, LV_STATE_CHECKED);
    auto* ctx = static_cast<Context*>(lv_event_get_user_data(event));
    ctx->bindings.onBtToggled(ctx, is_on);
}

static void onEnableOnBootSwitchChanged(lv_event_t* event) {
    auto* enable_switch = static_cast<lv_obj_t*>(lv_event_get_target(event));
    bool is_on = lv_obj_has_state(enable_switch, LV_STATE_CHECKED);
    // setEnableOnBoot() updates the in-memory cache before it touches the filesystem, so
    // dispatching keeps the (slow) save off the LVGL task while shouldEnableOnBoot() still
    // reflects the new value almost immediately.
    getMainDispatcher().dispatch([is_on] {
        bluetooth::settings::setEnableOnBoot(is_on);
    });
}

static void onEnableOnBootParentClicked(lv_event_t* event) {
    auto* enable_switch = static_cast<lv_obj_t*>(lv_event_get_user_data(event));
    bool new_state = !lv_obj_has_state(enable_switch, LV_STATE_CHECKED);
    if (new_state) {
        lv_obj_add_state(enable_switch, LV_STATE_CHECKED);
    } else {
        lv_obj_remove_state(enable_switch, LV_STATE_CHECKED);
    }
    // add/remove_state does not fire LV_EVENT_VALUE_CHANGED, so persist here directly.
    getMainDispatcher().dispatch([new_state] {
        bluetooth::settings::setEnableOnBoot(new_state);
    });
}

static void onScanButtonClicked(lv_event_t* event) {
    auto* ctx = static_cast<Context*>(lv_event_get_user_data(event));
    Device* dev = nullptr;
    device_get_first_active_by_type(&BLUETOOTH_TYPE, &dev);
    bool scanning = dev ? bluetooth_is_scanning(dev) : false;
    if (dev) {
        device_put(dev);
    }
    ctx->bindings.onScanToggled(ctx, !scanning);
}

// region Peer list callbacks

struct PeerListItemData {
    void* context;
    State* state;
    Bindings* bindings;
    size_t index;
    bool isPaired;
};

void View::onConnect(lv_event_t* event) {
    auto* data = static_cast<PeerListItemData*>(lv_event_get_user_data(event));

    if (data->isPaired) {
        // Open the per-device settings screen for paired devices
        auto peers = data->state->getPairedPeers();
        if (data->index < peers.size()) {
            btpeersettings::start(bluetooth::settings::addrToHex(peers[data->index].addr));
        }
    } else {
        // Unrecognised scan result — initiate pairing
        auto peers = data->state->getScanResults();
        if (data->index < peers.size()) {
            data->bindings->onPairPeer(data->context, peers[data->index].addr);
        }
    }
}

// endregion Peer list callbacks

static uint8_t mapRssiToPercentage(int8_t rssi) {
    auto abs_rssi = std::abs(rssi);
    if (abs_rssi < 30) abs_rssi = 30;
    if (abs_rssi > 90) abs_rssi = 90;
    return static_cast<uint8_t>((float)(90 - abs_rssi) / 60.f * 100.f);
}

void View::createPeerListItem(const bluetooth::PeerRecord& record, bool isPaired, size_t index) {
    const auto percentage = mapRssiToPercentage(record.rssi);
    const auto label = record.name.empty()
        ? std::format("Unknown ({:02x}{:02x}{:02x}{:02x}{:02x}{:02x}) {}%",
            record.addr[0], record.addr[1], record.addr[2],
            record.addr[3], record.addr[4], record.addr[5],
            percentage)
        : std::format("{} {}%", record.name, percentage);

    auto* button = lv_list_add_button(peers_list, nullptr, label.c_str());

    auto* item_data = new PeerListItemData { context, state, bindings, index, isPaired };
    lv_obj_set_user_data(button, item_data);
    lv_obj_add_event_cb(button, onConnect, LV_EVENT_SHORT_CLICKED, item_data);
    lv_obj_add_event_cb(button, [](lv_event_t* e) {
        delete static_cast<PeerListItemData*>(lv_obj_get_user_data(lv_event_get_current_target_obj(e)));
    }, LV_EVENT_DELETE, nullptr);
}

// region Secondary updates

void View::updateBtToggle() {
    lv_obj_clear_state(enable_switch, LV_STATE_ANY);
    switch (state->getRadioState()) {
        using enum bluetooth::RadioState;
        case On:
            lv_obj_add_state(enable_switch, LV_STATE_CHECKED);
            break;
        case OnPending:
            lv_obj_add_state(enable_switch, LV_STATE_CHECKED);
            lv_obj_add_state(enable_switch, LV_STATE_DISABLED);
            break;
        case Off:
            lv_obj_remove_state(enable_switch, LV_STATE_CHECKED);
            lv_obj_remove_state(enable_switch, LV_STATE_DISABLED);
            break;
        case OffPending:
            lv_obj_remove_state(enable_switch, LV_STATE_CHECKED);
            lv_obj_add_state(enable_switch, LV_STATE_DISABLED);
            break;
    }
}

void View::updateEnableOnBootToggle() {
    if (enable_on_boot_switch != nullptr) {
        lv_obj_clear_state(enable_on_boot_switch, LV_STATE_ANY);
        if (bluetooth::settings::shouldEnableOnBoot()) {
            lv_obj_add_state(enable_on_boot_switch, LV_STATE_CHECKED);
        } else {
            lv_obj_remove_state(enable_on_boot_switch, LV_STATE_CHECKED);
        }
    }
}

void View::updateScanning() {
    if (state->getRadioState() == bluetooth::RadioState::On && state->isScanning()) {
        lv_obj_remove_flag(scanning_spinner, LV_OBJ_FLAG_HIDDEN);
    } else {
        lv_obj_add_flag(scanning_spinner, LV_OBJ_FLAG_HIDDEN);
    }
}

void View::createEnableOnBootRow(lv_obj_t* parent) {
    // Deliberately a sibling of peers_list rather than a child: updatePeerList() calls
    // lv_obj_clean(), so a row living inside the list would be destroyed and recreated on
    // every rebuild. That is what made this switch flip back to its old value - the tap
    // persisted asynchronously, but the rebuild that followed recreated the switch from the
    // not-yet-updated setting. Built once here, it simply keeps whatever the user set.
    auto* wrapper = lv_obj_create(parent);
    lv_obj_set_size(wrapper, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_set_style_pad_hor(wrapper, 8, LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(wrapper, 0, LV_STATE_DEFAULT);
    lv_obj_remove_flag(wrapper, LV_OBJ_FLAG_SCROLLABLE);

    auto* enable_label = lv_label_create(wrapper);
    lv_label_set_text(enable_label, "Enable on boot");
    lv_obj_align(enable_label, LV_ALIGN_LEFT_MID, 0, 0);

    enable_on_boot_switch = lv_switch_create(wrapper);
    lv_obj_align(enable_on_boot_switch, LV_ALIGN_RIGHT_MID, 0, 0);
    lv_obj_add_event_cb(enable_on_boot_switch, onEnableOnBootSwitchChanged, LV_EVENT_VALUE_CHANGED, nullptr);
    lv_obj_add_event_cb(wrapper, onEnableOnBootParentClicked, LV_EVENT_SHORT_CLICKED, enable_on_boot_switch);

    if (lvgl_get_ui_density() == LVGL_UI_DENSITY_COMPACT) {
        lv_obj_set_style_pad_ver(wrapper, 2, LV_STATE_DEFAULT);
    } else {
        lv_obj_set_style_pad_ver(wrapper, 8, LV_STATE_DEFAULT);
    }

    updateEnableOnBootToggle();
}

void View::updatePeerList() {
    const int32_t scroll_y = lv_obj_get_scroll_y(peers_list);

    lv_obj_clean(peers_list);

    using enum bluetooth::RadioState;
    if (state->getRadioState() == On) {
        // Paired peers section
        auto paired = state->getPairedPeers();
        if (!paired.empty()) {
            lv_list_add_text(peers_list, "Paired");
            for (size_t i = 0; i < paired.size(); ++i) {
                createPeerListItem(paired[i], true, i);
            }
        }

        // Scan results section
        auto scan_results = state->getScanResults();
        lv_list_add_text(peers_list, "Available");
        if (!scan_results.empty()) {
            for (size_t i = 0; i < scan_results.size(); ++i) {
                createPeerListItem(scan_results[i], false, i);
            }
        } else if (!state->isScanning()) {
            auto* no_devices_label = lv_label_create(peers_list);
            lv_label_set_text(no_devices_label, "No devices found.");
        }

        // Scan button
        auto* scan_button = lv_button_create(peers_list);
        lv_obj_set_width(scan_button, LV_PCT(100));
        lv_obj_set_style_margin_ver(scan_button, 4, LV_STATE_DEFAULT);
        auto* scan_label = lv_label_create(scan_button);
        lv_label_set_text(scan_label, state->isScanning() ? "Stop scan" : "Scan");
        lv_obj_add_event_cb(scan_button, onScanButtonClicked, LV_EVENT_SHORT_CLICKED, context);
    }

    // Restore where the user was: a rebuild triggered by a newly discovered peer must not
    // yank the list back to the top while they are reading further down.
    if (scroll_y > 0) {
        lv_obj_update_layout(peers_list);
        lv_obj_scroll_to_y(peers_list, scroll_y, LV_ANIM_OFF);
    }
}

bool View::isUserInteractingWithList() const {
    if (peers_list == nullptr) {
        return false;
    }
    for (lv_indev_t* indev = lv_indev_get_next(nullptr); indev != nullptr; indev = lv_indev_get_next(indev)) {
        if (lv_indev_get_scroll_obj(indev) == peers_list) {
            return true;
        }
        if (lv_indev_get_state(indev) != LV_INDEV_STATE_PRESSED) {
            continue;
        }
        // A press anywhere inside the list means a tap or drag is in flight; rebuilding now
        // would delete the pressed widget out from under the input device.
        for (lv_obj_t* obj = lv_indev_get_active_obj(); obj != nullptr; obj = lv_obj_get_parent(obj)) {
            if (obj == peers_list) {
                return true;
            }
        }
    }
    return false;
}

// endregion Secondary updates

void View::init(void* newContext, lv_obj_t* parent) {
    context = newContext;

    lv_obj_set_flex_flow(parent, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_row(parent, 0, LV_STATE_DEFAULT);

    root = parent;

    // Toolbar
    auto* toolbar = lvgl_toolbar_create(parent, "Bluetooth");
    lvgl_toolbar_set_nav_action(toolbar, LV_SYMBOL_CLOSE, onBackPressed, context);

    scanning_spinner = lvgl_toolbar_add_spinner_action(toolbar);

    enable_switch = lvgl_toolbar_add_switch_action(toolbar);
    lv_obj_add_event_cb(enable_switch, onEnableSwitchChanged, LV_EVENT_VALUE_CHANGED, context);

    // Persistent settings row, above (and outside of) the rebuilt peer list.
    createEnableOnBootRow(parent);

    // Peer list
    peers_list = lv_list_create(parent);
    lv_obj_set_flex_grow(peers_list, 1);
    lv_obj_set_width(peers_list, LV_PCT(100));

    // A rebuild deferred by the throttle (or by an in-progress touch) needs something to
    // flush it: the BT event that requested it may well have been the last one.
    rebuild_timer = lv_timer_create([](lv_timer_t* timer) {
        static_cast<View*>(lv_timer_get_user_data(timer))->update();
    }, LIST_REBUILD_INTERVAL_MS, this);
    lv_obj_add_event_cb(parent, [](lv_event_t* e) {
        auto* view = static_cast<View*>(lv_event_get_user_data(e));
        if (view->rebuild_timer != nullptr) {
            lv_timer_delete(view->rebuild_timer);
            view->rebuild_timer = nullptr;
        }
        view->root = nullptr;
        view->peers_list = nullptr;
        view->enable_on_boot_switch = nullptr;
    }, LV_EVENT_DELETE, this);
}

void View::update() {
    if (root == nullptr) {
        return;
    }

    updateBtToggle();
    updateScanning();

    const auto current_radio = state->getRadioState();
    const auto current_scanning = state->isScanning();
    const auto current_scan_count = state->getScanResultCount();
    const auto current_paired_count = state->getPairedPeerCount();

    const bool content_changed = !hasRenderedList ||
        current_radio != lastListRadioState ||
        current_scanning != lastListScanning ||
        current_scan_count != lastScanResultCount ||
        current_paired_count != lastPairedCount;

    if (!content_changed && !listRebuildPending) {
        return;
    }

    // BT_EVENT_PEER_FOUND arrives in bursts while scanning. Rebuilding on each one destroys
    // the widget the user is touching, so scrolling never gets a chance to start. Defer
    // while they are interacting, and otherwise coalesce to at most one rebuild per period.
    if (isUserInteractingWithList()) {
        listRebuildPending = true;
        return;
    }

    const uint32_t now = lv_tick_get();
    if (hasRenderedList && (uint32_t)(now - lastListRenderTick) < LIST_REBUILD_INTERVAL_MS) {
        listRebuildPending = true;
        return;
    }

    updatePeerList();
    listRebuildPending = false;
    hasRenderedList = true;
    lastListRenderTick = now;
    lastListRadioState = current_radio;
    lastListScanning = current_scanning;
    lastScanResultCount = current_scan_count;
    lastPairedCount = current_paired_count;
}

} // namespace tt::app::btmanage
