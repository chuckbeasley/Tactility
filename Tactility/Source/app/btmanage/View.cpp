#include <cstdio>
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
    app_event_emit_close(ctx->appInstanceId);
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

// Row metadata is packed into the button's user_data instead of heap-allocated per row. The old
// pattern `new PeerListItemData` + delete on LV_EVENT_DELETE leaked when LVGL cleared the object's
// user_data before firing DELETE (so the handler read nullptr and the row leaked ~10 KB/scan,
// which over many scans/opens drained the heap toward an OOM crash). Packing the (index, isPaired)
// pair into a single pointer keeps it allocation-free and leak-free.

void View::onConnect(lv_event_t* event) {
    // The View is found via the parent list's user_data, set once in init().
    auto* button = static_cast<lv_obj_t*>(lv_event_get_current_target_obj(event));
    auto* self = static_cast<View*>(lv_obj_get_user_data(lv_obj_get_parent(button))); // peers_list
    if (self == nullptr) {
        return;
    }

    const uintptr_t packed = reinterpret_cast<uintptr_t>(lv_obj_get_user_data(button));
    const bool isPaired = (packed & 1u) != 0;
    const size_t row_key = static_cast<size_t>(packed >> 1);

    // Resolved against the rows that were actually rendered, not against the live scan results: the
    // list is rebuilt while a scan runs, so looking the peer up by position in the current results
    // would connect to whatever moved into that slot, or silently do nothing if the list shrank.
    if (row_key >= self->rowAddresses.size()) {
        return;
    }
    const auto addr = self->rowAddresses[row_key];

    if (isPaired) {
        // Open the per-device settings screen for paired devices
        btpeersettings::start(bluetooth::settings::addrToHex(addr));
    } else {
        // Unrecognised scan result — initiate pairing
        self->bindings->onPairPeer(self->context, addr);
    }
}

// endregion Peer list callbacks

static uint8_t mapRssiToPercentage(int8_t rssi) {
    auto abs_rssi = std::abs(rssi);
    if (abs_rssi < 30) abs_rssi = 30;
    if (abs_rssi > 90) abs_rssi = 90;
    return static_cast<uint8_t>((float)(90 - abs_rssi) / 60.f * 100.f);
}

// The connected row's text, in one place so the row can be rewritten in place when the live RSSI
// changes (see View::refreshConnectedRow) instead of only when the whole list is rebuilt.
static void formatConnectedRowText(
    const char* name_part,
    int8_t tx_power,
    bool rssi_valid,
    int8_t rssi,
    char* out,
    size_t out_size
) {
    if (!rssi_valid) {
        std::snprintf(out, out_size, "%s %sConnected", name_part, LV_SYMBOL_OK);
        return;
    }
    if (tx_power != 0x7F) {
        // With the peer's advertised TX power the path loss becomes computable - 20 dB at 10 cm, 40 dB
        // at 1 m and 50 dB at 3 m in free space - and it is the same number whatever the peer
        // transmits at. 0x7F is the "not present" value from the advertisement's TX Power Level field,
        // which is the common case: most keyboards simply do not send it.
        std::snprintf(out, out_size, "%s %s %ddBm\npath %ddB (peer TX %+ddBm)", name_part, LV_SYMBOL_OK,
                      (int)rssi, (int)tx_power - (int)rssi, (int)tx_power);
    } else {
        // Without it, an absolute reading is all there is. The useful signal is the slope: 2.4 GHz
        // free-space loss grows about 6 dB per doubling of distance, so moving the keyboard from
        // touching the board to arm's length should cost roughly 10 dB.
        std::snprintf(out, out_size, "%s %s %ddBm\npeer advertises no TX power", name_part, LV_SYMBOL_OK,
                      (int)rssi);
    }
}

// The label inside a list button. lv_list_add_button builds the button and a label child, and
// lv_list_get_button_text returns the label's *text*, not the object, so the object is picked out
// here: rewriting it in place is what keeps the row's identity (and the list's scroll position).
static lv_obj_t* findButtonLabel(lv_obj_t* button) {
    const uint32_t count = lv_obj_get_child_count(button);
    for (uint32_t i = 0; i < count; ++i) {
        lv_obj_t* child = lv_obj_get_child(button, i);
        if (lv_obj_check_type(child, &lv_label_class)) {
            return child;
        }
    }
    return nullptr;
}

void View::createPeerListItem(const bluetooth::PeerRecord& record, bool isPaired) {
    // Built with snprintf rather than std::format, deliberately. This runs once per row, at the
    // very bottom of a deep chain - window creation, then the list rebuild, all on this app's own
    // task - and std::format reaches vformat and its sink machinery, which is around twenty frames
    // of its own. That was the deepest point when this app crashed opening its device list: the
    // coredump shows createPeerListItem at frame 20 with frames 0..19 all inside libstdc++ format,
    // on a 16 KB stack. snprintf does the same job in a handful of frames.
    // Two stages on purpose: the name is built once, then the status is appended in whichever form
    // fits. A connected row carries two numbers - the live RSSI and the path loss it implies - and on
    // a 320 px row they only fit on a second line, which is why that one case is multi-line.
    char name_part[80];
    if (record.name.empty()) {
        std::snprintf(name_part, sizeof(name_part), "Unknown (%02x%02x%02x%02x%02x%02x)",
            record.addr[0], record.addr[1], record.addr[2],
            record.addr[3], record.addr[4], record.addr[5]);
    } else {
        std::snprintf(name_part, sizeof(name_part), "%s", record.name.c_str());
    }

    char label[192];
    // Hoisted out of the connected branch: the row remembers what it displayed, so
    // refreshConnectedRow() can tell a current row from a stale one without re-reading the controller.
    int8_t live_rssi = 0;
    bool live_rssi_valid = false;
    bool is_connected_row = false;
    if (state->isConnectingTo(record.addr)) {
        std::snprintf(label, sizeof(label), "%s %sConnecting...", name_part, LV_SYMBOL_REFRESH);
    } else if (record.connected) {
        is_connected_row = true;
        // The live connection value, not record.rssi: a connected keyboard stops advertising, so its
        // scan RSSI is frozen at whatever was heard before the connection - which is how a keyboard
        // sitting on the board could still read as weak. The peer's advertised TX power turns that
        // into a path loss, which is the figure that says whether the link is healthy: about 20 dB at
        // 10 cm, 40 dB at 1 m and 50 dB at 3 m in free space.
        live_rssi_valid = state->getConnectedRssi(live_rssi);
        formatConnectedRowText(name_part, record.txPower, live_rssi_valid, live_rssi, label, sizeof(label));
    } else if (record.rssi == 0) {
        // No reading at all: a paired peer that has not been heard in a scan (its stored record
        // carries rssi 0). Mapping that to a percentage read as "100%" - the most optimistic possible
        // answer for the one case with no data behind it - so it is left blank instead.
        std::snprintf(label, sizeof(label), "%s", name_part);
    } else if (record.txPower != 0x7F) {
        // Scanned peer: percentage from the advertisement's RSSI, plus the TX power that peer quoted in
        // the same advertisement. That number is what makes readings comparable between devices - a
        // peer transmitting at -20 dBm reads exactly like one 20 dB further away.
        std::snprintf(label, sizeof(label), "%s %u%% (TX%+d)", name_part,
                      static_cast<unsigned>(mapRssiToPercentage(record.rssi)), (int)record.txPower);
    } else {
        std::snprintf(label, sizeof(label), "%s %u%%", name_part,
                      static_cast<unsigned>(mapRssiToPercentage(record.rssi)));
    }

    auto* button = lv_list_add_button(peers_list, nullptr, label);
    // Compact rows so more peers fit on the small display (local style overrides the theme).
    lv_obj_set_style_pad_ver(button,
        lvgl_get_ui_density() == LVGL_UI_DENSITY_COMPACT ? 2 : 4,
        LV_STATE_DEFAULT);
    // Pack (row key, isPaired) into the button's user_data so onConnect() can decode it without a
    // per-row heap allocation. The key indexes rowAddresses, not the live scan results - see the
    // note on that member.
    const size_t row_key = rowAddresses.size();
    rowAddresses.push_back(record.addr);
    lv_obj_set_user_data(button, reinterpret_cast<void*>((row_key << 1) | (isPaired ? 1u : 0u)));
    // LV_EVENT_CLICKED, not SHORT_CLICKED: LVGL withholds SHORT_CLICKED when the press outlasted
    // long_press_time (400 ms), so a deliberate, slightly slow tap on a device row did nothing at all
    // - which reads as "clicking it no longer connects". CLICKED arrives on release as long as the
    // press did not turn into a scroll, which is the rule this action wants. The click guard still
    // suppresses it when the finger moved.
    lv_obj_add_event_cb(button, onConnect, LV_EVENT_CLICKED, nullptr);

    if (is_connected_row) {
        // The connected peer's row is the one row that goes stale on its own: its reading changes
        // continuously and nothing the list is rebuilt for reports that. Remember the label object and
        // what it currently says, so refreshConnectedRow() can rewrite that one label between rebuilds.
        connected_row_label = findButtonLabel(button);
        connected_row_addr = record.addr;
        connected_row_rssi = live_rssi;
        connected_row_rssi_valid = live_rssi_valid;
        connected_row_tx_power = record.txPower;
        std::snprintf(connected_row_name, sizeof(connected_row_name), "%s", name_part);
    }
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
    lv_obj_add_event_cb(wrapper, onEnableOnBootParentClicked, LV_EVENT_CLICKED, enable_on_boot_switch);

    if (lvgl_get_ui_density() == LVGL_UI_DENSITY_COMPACT) {
        lv_obj_set_style_pad_ver(wrapper, 2, LV_STATE_DEFAULT);
    } else {
        lv_obj_set_style_pad_ver(wrapper, 8, LV_STATE_DEFAULT);
    }

    updateEnableOnBootToggle();
}

void View::updatePeerList() {
    const int32_t scroll_y = lv_obj_get_scroll_y(peers_list);

    // A busy RF environment can produce dozens of peers per scan. Rebuilding the LVGL list for
    // every one of them creates a very deep widget tree on the app task, whose redraw recursion
    // overflows the stack and crashes the device with a Stack protection fault. Cap how many peers
    // we render so the widget tree stays bounded.
    //
    // This cap is load-bearing for the task's stack size, not just for legibility: measured with
    // uxTaskGetSystemState, each rendered row costs roughly 500 bytes of stack that is not given
    // back until the rebuild returns - 4 KB held ~30 rows, 6 KB ~45, and 8 KB rendered all 60
    // (30 paired + 30 available). (Those were called 16/24/32 KB here until the unit was checked:
    // stack depths in this tree are bytes, not words - see APP_STACK_SIZE_MAX in app/manifest.h.)
    // Raising this number therefore needs the manifest's stack raised with it; the two are one
    // budget.
    constexpr size_t MAX_VISIBLE_PEERS = 30;

    lv_obj_clean(peers_list);
    // The rendered rows are gone, so the addresses behind them are too. Refilled below, in the same
    // order the rows are created.
    rowAddresses.clear();
    // Same reasoning for the tracked connected row: the label it points at was just destroyed, and a
    // stale pointer here would be written to on the next refresh. Re-set by createPeerListItem below.
    connected_row_label = nullptr;
    connected_row_rssi_valid = false;

    using enum bluetooth::RadioState;
    if (state->getRadioState() == On) {
        // Paired peers section
        auto paired = state->getPairedPeers();
        if (!paired.empty()) {
            lv_list_add_text(peers_list, "Paired");
            size_t count = std::min(paired.size(), MAX_VISIBLE_PEERS);
            for (size_t i = 0; i < count; ++i) {
                createPeerListItem(paired[i], true);
            }
        }

        // Scan results section
        auto scan_results = state->getScanResults();
        lv_list_add_text(peers_list, "Available");
        if (!scan_results.empty()) {
            size_t count = std::min(scan_results.size(), MAX_VISIBLE_PEERS);
            for (size_t i = 0; i < count; ++i) {
                createPeerListItem(scan_results[i], false);
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
        lv_obj_add_event_cb(scan_button, onScanButtonClicked, LV_EVENT_CLICKED, context);
    }

    // Restore where the user was: a rebuild triggered by a newly discovered peer must not
    // yank the list back to the top while they are reading further down.
    if (scroll_y > 0) {
        lv_obj_update_layout(peers_list);
        lv_obj_scroll_to_y(peers_list, scroll_y, LV_ANIM_OFF);
    }
}

void View::refreshConnectedRow() {
    if (connected_row_label == nullptr) {
        // Nothing connected is rendered - either the list is empty of connected peers, or it has not
        // been built yet. The next rebuild takes a fresh reading anyway.
        return;
    }

    int8_t live_rssi = 0;
    const bool valid = state->getConnectedRssi(live_rssi);
    // Only touch the label when the number on screen would actually change. update() runs on every
    // loop tick of this app, and lv_label_set_text invalidates the row and re-runs layout, so writing
    // it unconditionally would spend the whole loop re-laying out a row that says the same thing.
    if (valid == connected_row_rssi_valid && (!valid || live_rssi == connected_row_rssi)) {
        return;
    }

    char label[192];
    formatConnectedRowText(connected_row_name, connected_row_tx_power, valid, live_rssi, label, sizeof(label));
    lv_label_set_text(connected_row_label, label);
    connected_row_rssi = live_rssi;
    connected_row_rssi_valid = valid;
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
    // onConnect() recovers the View through the parent list's user_data (per-row metadata is now
    // packed into each button's own user_data instead of being heap-allocated).
    lv_obj_set_user_data(peers_list, this);

    // A rebuild deferred by the throttle (or by an in-progress touch) needs something to
    // flush it: the BT event that requested it may well have been the last one.
    //
    // Taken under the context lock, because the app task also rebuilds this list - through
    // requestViewUpdate(), which holds that lock (and the LVGL lock) while it does. This timer runs
    // on the LVGL task and would otherwise rebuild concurrently, and a rebuild writes rowAddresses:
    // two tasks resizing the same std::vector while a click reads it is exactly the kind of race
    // that produces a crash or a lock-up with no pattern to it. The lock is not held inside
    // View::update() itself, so the callers that already hold it do not deadlock on a re-take.
    rebuild_timer = lv_timer_create([](lv_timer_t* timer) {
        auto* view = static_cast<View*>(lv_timer_get_user_data(timer));
        auto* ctx = static_cast<Context*>(view->context);
        if (ctx != nullptr) {
            ctx->lock();
        }
        view->update();
        if (ctx != nullptr) {
            ctx->unlock();
        }
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

    // Before the change detection below, because the live link RSSI is exactly what that detection
    // cannot see: it changes with no event, no count and no state behind it, so update() returned early
    // on every tick and the row kept the reading it was built with however far the device then moved.
    refreshConnectedRow();

    const auto current_radio = state->getRadioState();
    const auto current_scanning = state->isScanning();
    const auto current_scan_count = state->getScanResultCount();
    const auto current_paired_count = state->getPairedPeerCount();
    const auto current_connected_count = state->getConnectedPeerCount();

    const bool content_changed = !hasRenderedList ||
        current_radio != lastListRadioState ||
        current_scanning != lastListScanning ||
        current_scan_count != lastScanResultCount ||
        current_paired_count != lastPairedCount ||
        current_connected_count != lastConnectedCount;

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
    lastConnectedCount = current_connected_count;
}

} // namespace tt::app::btmanage
