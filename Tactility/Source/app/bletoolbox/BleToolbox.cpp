#include <sdkconfig.h>

#include <Tactility/Tactility.h>
#include <Tactility/Timer.h>
#include <Tactility/bluetooth/Bluetooth.h>

#include <app/event.h>
#include <app/manager.h>
#include <app/start.h>
#include <app/manifest.h>
#include <app/scheduler.h>

#include <lvgl_window_manager/window_manager.h>

#include <lvgl/lvgl.h>
#include <lvgl/widgets/toolbar.h>

#include <tactility/concurrent/task_event_group.h>
#include <tactility/device.h>
#include <tactility/drivers/bluetooth.h>
#include <tactility/log.h>
#include <tactility/time.h>

#include <array>
#include <cstdio>
#include <cstring>
#include <format>
#include <memory>
#include <string>
#include <vector>

#include "BleSpamPayloads.h"

namespace tt::app::bletoolbox {

constexpr auto* TAG = "BleToolbox";

extern const ::AppManifest manifest;

namespace {

enum class Screen { Main, Scan, Spam, Airtag };

// AirTag / Apple "Find My" device advertisement signature (from the offline-finding protocol,
// see "Who Can Find My Devices?", PETS 2021): manufacturer data with Apple company ID 0x004C,
// then type 0x12 (Nearby Info / offline finding) and data length 0x19 (25).
constexpr uint8_t APPLE_COMPANY_LO = 0x4C;
constexpr uint8_t APPLE_COMPANY_HI = 0x00;
constexpr uint8_t AIRTAG_OF_TYPE = 0x12;

constexpr uint32_t POLL_INTERVAL_MS = 300;
constexpr uint32_t SPAM_INTERVAL_MS = 200;
constexpr size_t MAX_SCAN_RESULTS = 64;

// A device discovered during scan/monitor. `manuf` carries the advertisement manufacturer bytes.
struct Peer {
    std::array<uint8_t, 6> addr;
    char name[BT_NAME_MAX + 1];
    int8_t rssi;
    uint8_t addr_type;
    uint8_t manuf[24];
    uint8_t manuf_len;
    uint32_t seenCount;
};

struct Context {
    uint32_t appInstanceId = 0;
    Screen screen = Screen::Main;

    Device* dev = nullptr;
    BtEventSubscription btSub {};

    // Scan (all devices) + AirTag monitor share the same scan subscription.
    bool scanRunning = false;
    bool airtagRunning = false;
    bool spamRunning = false;
    size_t currentPayload = 0;
    bool randomizeAddress = true;
    // Which spoof families are on (Apple, Android, Windows, Samsung). All on by default. Written
    // by the LVGL task (family switch handler), read by the spam timer task - benign one-tick race.
    bool enabled[kFamilyCount] = { true, true, true, true };

    std::vector<Peer> scanPeers;
    std::vector<Peer> airtagPeers;
    uint32_t scanCount = 0;

    std::unique_ptr<Timer> pollTimer;
    std::unique_ptr<Timer> spamTimer;

    // UI widgets.
    lv_obj_t* body = nullptr;
    lv_obj_t* statusLabel = nullptr;
    lv_obj_t* countLabel = nullptr;
    lv_obj_t* list = nullptr;
    lv_obj_t* startButtonLabel = nullptr;
    lv_obj_t* spamLabel = nullptr;

    bool uiDirty = false;
    bool listDirty = false;
};

// ---- Helpers ----

static void formatAddr(const uint8_t addr[6], char* out, size_t outLen) {
    std::snprintf(out, outLen, "%02x:%02x:%02x:%02x:%02x:%02x",
        addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);
}

// Apple offline-finding ("Nearby Info", 0x12) devices — i.e. AirTag / Find My trackers.
static bool isOfflineFinding(const uint8_t* manuf, uint8_t manufLen) {
    return manufLen >= 4 &&
           manuf[0] == APPLE_COMPANY_LO && manuf[1] == APPLE_COMPANY_HI &&
           manuf[2] == AIRTAG_OF_TYPE;
}

// Enables the BT radio if not already on. Null-safe: a missing device must not be dereferenced.
static void ensureBluetoothOn(Device* dev) {
    if (dev == nullptr) {
        LOG_W(TAG, "No Bluetooth device available");
        return;
    }
    if (!bluetooth::isRadioOnOrPending(dev)) {
        LOG_I(TAG, "Enabling Bluetooth radio");
        bluetooth::start(dev);
    }
}

// ---- Event-driven list update (called from the app task, which owns LVGL) ----

// @return true if the peer list content changed (a new device was added, or a name/RSSI update
// actually changed an existing entry). Callers only trigger a (LVGL-locked) list rebuild on real
// changes, so a burst of same-device events doesn't hammer the widget tree.
static bool updatePeers(Peer* peer, std::vector<Peer>& peers) {
    for (auto& existing : peers) {
        if (existing.addr == peer->addr) {
            bool changed = existing.rssi != peer->rssi;
            existing.rssi = peer->rssi;
            existing.seenCount++;
            if (peer->name[0] != '\0') {
                if (std::strcmp(existing.name, peer->name) != 0) {
                    std::strncpy(existing.name, peer->name, BT_NAME_MAX);
                    existing.name[BT_NAME_MAX] = '\0';
                    changed = true;
                }
            }
            return changed;
        }
    }
    if (peers.size() < MAX_SCAN_RESULTS) {
        peers.push_back(*peer);
        return true;
    }
    return false;
}

static const char* deviceNameOrType(const Peer& peer) {
    if (peer.name[0] != '\0') {
        return peer.name;
    }
    if (isOfflineFinding(peer.manuf, peer.manuf_len)) {
        return "AirTag";
    }
    return "-";
}

// Three-column table of devices: Name | Address | RSSI.
static void rebuildList(Context* ctx) {
    if (ctx->list == nullptr) return;
    const bool airtag = (ctx->screen == Screen::Airtag);
    const auto& peers = airtag ? ctx->airtagPeers : ctx->scanPeers;
    lv_table_set_row_count(ctx->list, (uint32_t)(peers.size() + 1)); // +1 for the header row
    lv_table_set_cell_value(ctx->list, 0, 0, "Name");
    lv_table_set_cell_value(ctx->list, 0, 1, "Address");
    lv_table_set_cell_value(ctx->list, 0, 2, "RSSI");
    char addr[18];
    for (size_t i = 0; i < peers.size(); ++i) {
        const uint32_t row = (uint32_t)(i + 1);
        const Peer& peer = peers[i];
        lv_table_set_cell_value(ctx->list, row, 0, deviceNameOrType(peer));
        formatAddr(peer.addr.data(), addr, sizeof(addr));
        lv_table_set_cell_value(ctx->list, row, 1, addr);
        lv_table_set_cell_value(ctx->list, row, 2, std::to_string(peer.rssi).c_str());
    }
}

// ---- BLE event handling ----

static void onBtEvent(Context* ctx, const BtEvent& event) {
    switch (event.type) {
        case BT_EVENT_SCAN_STARTED:
            ctx->scanCount = 0;
            ctx->scanPeers.clear();
            ctx->airtagPeers.clear();
            ctx->listDirty = true;
            break;

        case BT_EVENT_PEER_FOUND: {
            Peer peer {};
            memcpy(peer.addr.data(), event.peer.addr, 6);
            peer.addr_type = event.peer.addr_type;
            peer.rssi = event.peer.rssi;
            peer.manuf_len = event.peer.manuf_len;
            if (peer.manuf_len > 0) memcpy(peer.manuf, event.peer.manuf_data, peer.manuf_len);
            if (event.peer.name[0] != '\0') {
                std::strncpy(peer.name, event.peer.name, BT_NAME_MAX);
                peer.name[BT_NAME_MAX] = '\0';
            }
            peer.seenCount = 1;
            bool changed = false;
            if (ctx->airtagRunning || ctx->scanRunning) {
                if (updatePeers(&peer, ctx->scanPeers)) changed = true;
                ctx->scanCount = static_cast<uint32_t>(ctx->scanPeers.size());
            }
            if (ctx->airtagRunning && isOfflineFinding(peer.manuf, peer.manuf_len)) {
                if (updatePeers(&peer, ctx->airtagPeers)) changed = true;
            }
            if (changed) {
                ctx->listDirty = true;
            }
            break;
        }

        case BT_EVENT_RADIO_STATE_CHANGED:
            // Scan can only start once the radio is actually on. If a scan/monitor was requested
            // while the radio was still turning on, kick it off now instead of firing a
            // scan_start() too early (which the driver would drop).
            if (event.radio_state == BT_RADIO_STATE_ON && (ctx->scanRunning || ctx->airtagRunning) &&
                ctx->dev != nullptr && !bluetooth_is_scanning(ctx->dev)) {
                bluetooth_scan_start(ctx->dev);
            }
            break;

        case BT_EVENT_SCAN_FINISHED:
            // For a continuous monitor, immediately restart the next scan cycle.
            if (ctx->airtagRunning) {
                Device* dev = ctx->dev;
                if (dev != nullptr && !bluetooth_is_scanning(dev)) {
                    bluetooth_scan_start(dev);
                }
            }
            ctx->uiDirty = true;
            break;

        default:
            break;
    }
    ctx->uiDirty = true;
}

// ---- Spam timer (runs on the FreeRTOS timer task; touches only BLE + atomic-ish state) ----

static void onSpamTick(Context* ctx) {
    if (!ctx->spamRunning) return;
    if (bluetooth::getRadioState() != bluetooth::RadioState::On) return;
    if (kPayloadCount == 0) return;
    // Advance to the next enabled payload, wrapping around. The radio comes on asynchronously after
    // ensureBluetoothOn() requests it, so this also no-ops until the radio is actually on.
    for (size_t i = 0; i < kPayloadCount; i++) {
        ctx->currentPayload = (ctx->currentPayload + 1) % kPayloadCount;
        const Payload& payload = kPayloads[ctx->currentPayload];
        if (ctx->enabled[payload.family]) {
            bluetooth::startAdvertising(payload.data, payload.length, false, ctx->randomizeAddress);
            return;
        }
    }
}

static void onFamilySwitchChanged(lv_event_t* event) {
    auto* sw = static_cast<lv_obj_t*>(lv_event_get_target(event));
    auto* ctx = static_cast<Context*>(lv_event_get_user_data(event));
    uintptr_t family = reinterpret_cast<uintptr_t>(lv_obj_get_user_data(sw));
    if (family < kFamilyCount) {
        ctx->enabled[family] = lv_obj_has_state(sw, LV_STATE_CHECKED);
    }
}

// ---- UI screens ----

static void showMainScreen(Context* ctx);
static void showScanScreen(Context* ctx);
static void showSpamScreen(Context* ctx);
static void showAirtagScreen(Context* ctx);

static void ShowScreen(Context* ctx, Screen screen) {
    ctx->screen = screen;
    if (ctx->body != nullptr) {
        lv_obj_clean(ctx->body);
    }
    ctx->statusLabel = nullptr;
    ctx->countLabel = nullptr;
    ctx->list = nullptr;
    ctx->startButtonLabel = nullptr;
    ctx->spamLabel = nullptr;

    switch (screen) {
        case Screen::Main: showMainScreen(ctx); break;
        case Screen::Scan: showScanScreen(ctx); break;
        case Screen::Spam: showSpamScreen(ctx); break;
        case Screen::Airtag: showAirtagScreen(ctx); break;
    }
    ctx->uiDirty = true;
    ctx->listDirty = true;
}

static void stopAllActive(Context* ctx) {
#if defined(CONFIG_BT_NIMBLE_ENABLED)
    ctx->spamRunning = false;
    if (ctx->spamTimer != nullptr) ctx->spamTimer->stop();
    bluetooth::stopAdvertising();
    ctx->scanRunning = false;
    ctx->airtagRunning = false;
    if (ctx->dev != nullptr && bluetooth_is_scanning(ctx->dev)) {
        bluetooth_scan_stop(ctx->dev);
    }
#endif
}

static void onBackPressed(lv_event_t* event) {
    auto* ctx = static_cast<Context*>(lv_event_get_user_data(event));
    if (ctx->screen == Screen::Main) {
        stopAllActive(ctx);
        app_event_emit_close(ctx->appInstanceId);
    } else {
        stopAllActive(ctx);
        ShowScreen(ctx, Screen::Main);
    }
}

static void addMenuButton(Context* ctx, const char* text, void (*cb)(lv_event_t*)) {
    auto* btn = lv_button_create(ctx->body);
    lv_obj_set_width(btn, LV_PCT(100));
    auto* label = lv_label_create(btn);
    lv_label_set_text(label, text);
    lv_obj_center(label);
    lv_obj_add_event_cb(btn, cb, LV_EVENT_SHORT_CLICKED, ctx);
}

static void onGoScan(lv_event_t* e) { ShowScreen(static_cast<Context*>(lv_event_get_user_data(e)), Screen::Scan); }
static void onGoSpam(lv_event_t* e) { ShowScreen(static_cast<Context*>(lv_event_get_user_data(e)), Screen::Spam); }
static void onGoAirtag(lv_event_t* e) { ShowScreen(static_cast<Context*>(lv_event_get_user_data(e)), Screen::Airtag); }

// ---- Main ----

static void showMainScreen(Context* ctx) {
    addMenuButton(ctx, "BLE Scan", onGoScan);
    addMenuButton(ctx, "BLE Spam", onGoSpam);
    addMenuButton(ctx, "AirTag Monitor", onGoAirtag);
}

// ---- Scan ----

static void onStartScan(lv_event_t* event) {
    auto* ctx = static_cast<Context*>(lv_event_get_user_data(event));
#if defined(CONFIG_BT_NIMBLE_ENABLED)
    if (ctx->scanRunning || ctx->airtagRunning) {
        ctx->scanRunning = false;
        ctx->airtagRunning = false;
        Device* dev = ctx->dev;
        if (dev != nullptr && bluetooth_is_scanning(dev)) {
            bluetooth_scan_stop(dev);
        }
        ctx->uiDirty = true;
        ctx->listDirty = true;
        return;
    }
    if (ctx->dev != nullptr && !bluetooth_is_scanning(ctx->dev)) {
        ensureBluetoothOn(ctx->dev);
        ctx->scanRunning = true;
        ctx->uiDirty = true;
        ctx->listDirty = true;
        if (bluetooth::getRadioState() == bluetooth::RadioState::On) {
            bluetooth_scan_start(ctx->dev);
        }
    }
#endif
}

static void showScanScreen(Context* ctx) {
    auto* label = lv_label_create(ctx->body);
    lv_label_set_text(label, "Nearby BLE devices.");
    ctx->statusLabel = lv_label_create(ctx->body);
    lv_label_set_text(ctx->statusLabel, "Stopped");

    ctx->list = lv_table_create(ctx->body);
    lv_table_set_column_count(ctx->list, 3);
    lv_table_set_column_width(ctx->list, 0, 150);
    lv_table_set_column_width(ctx->list, 1, 180);
    lv_table_set_column_width(ctx->list, 2, 90);
    // Compact rows: shrink vertical cell padding so more records fit on the small display.
    lv_obj_set_style_pad_ver(ctx->list, 2, LV_PART_ITEMS);
    lv_obj_set_style_pad_left(ctx->list, 4, LV_PART_ITEMS);
    lv_obj_set_style_pad_right(ctx->list, 4, LV_PART_ITEMS);
    lv_obj_set_style_pad_top(ctx->list, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ctx->list, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ctx->list, LV_PCT(100));
    lv_obj_set_flex_grow(ctx->list, 1);
    lv_obj_set_scroll_dir(ctx->list, LV_DIR_VER);
    lv_obj_set_style_pad_all(ctx->list, 0, LV_STATE_DEFAULT);

    auto* button = lv_button_create(ctx->body);
    lv_obj_set_width(button, LV_PCT(100));
    ctx->startButtonLabel = lv_label_create(button);
    lv_label_set_text(ctx->startButtonLabel, "Start Scan");
    lv_obj_center(ctx->startButtonLabel);
    lv_obj_add_event_cb(button, onStartScan, LV_EVENT_SHORT_CLICKED, ctx);

    ctx->countLabel = lv_label_create(ctx->body);
    lv_label_set_text(ctx->countLabel, "0 devices");
}

// ---- Spam ----

static void onSpamStartStop(lv_event_t* event) {
    auto* ctx = static_cast<Context*>(lv_event_get_user_data(event));
#if defined(CONFIG_BT_NIMBLE_ENABLED)
    ctx->spamRunning = !ctx->spamRunning;
    if (ctx->spamRunning) {
        ensureBluetoothOn(ctx->dev);
        ctx->spamTimer->start();
    } else {
        ctx->spamTimer->stop();
        bluetooth::stopAdvertising();
    }
#endif
    ctx->uiDirty = true;
}

static void onRandomizeSwitch(lv_event_t* event) {
    auto* sw = static_cast<lv_obj_t*>(lv_event_get_target(event));
    auto* ctx = static_cast<Context*>(lv_event_get_user_data(event));
    ctx->randomizeAddress = lv_obj_has_state(sw, LV_STATE_CHECKED);
}

static void showSpamScreen(Context* ctx) {
    auto* label = lv_label_create(ctx->body);
    lv_label_set_text(label, "Spoofs BLE advertisements.");
    ctx->spamLabel = lv_label_create(ctx->body);
    lv_label_set_text(ctx->spamLabel, kPayloads[ctx->currentPayload].label);

    // Per-family enable toggles (Apple / Android / Windows / Samsung).
    for (size_t i = 0; i < kFamilyCount; i++) {
        auto* btn = lv_button_create(ctx->body);
        lv_obj_set_width(btn, LV_PCT(100));
        auto* btnLabel = lv_label_create(btn);
        lv_label_set_text(btnLabel, kFamilies[i].label);
        lv_obj_center(btnLabel);
        auto* sw = lv_switch_create(btn);
        lv_obj_align(sw, LV_ALIGN_RIGHT_MID, -8, 0);
        lv_obj_set_user_data(sw, reinterpret_cast<void*>(static_cast<uintptr_t>(i)));
        lv_obj_add_event_cb(sw, onFamilySwitchChanged, LV_EVENT_VALUE_CHANGED, ctx);
        if (ctx->enabled[i]) {
            lv_obj_add_state(sw, LV_STATE_CHECKED);
        }
    }

    // Randomize address toggle.
    auto* btn = lv_button_create(ctx->body);
    lv_obj_set_width(btn, LV_PCT(100));
    auto* btnLabel = lv_label_create(btn);
    lv_label_set_text(btnLabel, "Randomize address");
    lv_obj_center(btnLabel);
    auto* sw = lv_switch_create(btn);
        lv_obj_align(sw, LV_ALIGN_RIGHT_MID, -8, 0);
    if (ctx->randomizeAddress) {
        lv_obj_add_state(sw, LV_STATE_CHECKED);
    }
    lv_obj_add_event_cb(sw, onRandomizeSwitch, LV_EVENT_VALUE_CHANGED, ctx);

    auto* button = lv_button_create(ctx->body);
    lv_obj_set_width(button, LV_PCT(100));
    ctx->startButtonLabel = lv_label_create(button);
    lv_label_set_text(ctx->startButtonLabel, "Start Spam");
    lv_obj_center(ctx->startButtonLabel);
    lv_obj_add_event_cb(button, onSpamStartStop, LV_EVENT_SHORT_CLICKED, ctx);

    ctx->statusLabel = lv_label_create(ctx->body);
    lv_label_set_text(ctx->statusLabel, "Stopped");
}

// ---- AirTag monitor ----

static void onStartAirtag(lv_event_t* event) {
    auto* ctx = static_cast<Context*>(lv_event_get_user_data(event));
#if defined(CONFIG_BT_NIMBLE_ENABLED)
    if (ctx->airtagRunning) {
        ctx->airtagRunning = false;
        Device* dev = ctx->dev;
        if (dev != nullptr && bluetooth_is_scanning(dev)) {
            bluetooth_scan_stop(dev);
        }
        ctx->uiDirty = true;
        ctx->listDirty = true;
        return;
    }
    if (ctx->dev != nullptr) {
        ensureBluetoothOn(ctx->dev);
        ctx->airtagRunning = true;
        ctx->uiDirty = true;
        ctx->listDirty = true;
        if (bluetooth::getRadioState() == bluetooth::RadioState::On && !bluetooth_is_scanning(ctx->dev)) {
            bluetooth_scan_start(ctx->dev);
        }
    }
#endif
}

static void showAirtagScreen(Context* ctx) {
    auto* label = lv_label_create(ctx->body);
    lv_label_set_text(label, "Detects Apple Find My trackers.");
    ctx->statusLabel = lv_label_create(ctx->body);
    lv_label_set_text(ctx->statusLabel, "Stopped");

    ctx->list = lv_table_create(ctx->body);
    lv_table_set_column_count(ctx->list, 3);
    lv_table_set_column_width(ctx->list, 0, 150);
    lv_table_set_column_width(ctx->list, 1, 180);
    lv_table_set_column_width(ctx->list, 2, 90);
    // Compact rows: shrink vertical cell padding so more records fit on the small display.
    lv_obj_set_style_pad_ver(ctx->list, 2, LV_PART_ITEMS);
    lv_obj_set_style_pad_left(ctx->list, 4, LV_PART_ITEMS);
    lv_obj_set_style_pad_right(ctx->list, 4, LV_PART_ITEMS);
    lv_obj_set_style_pad_top(ctx->list, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ctx->list, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ctx->list, LV_PCT(100));
    lv_obj_set_flex_grow(ctx->list, 1);
    lv_obj_set_scroll_dir(ctx->list, LV_DIR_VER);
    lv_obj_set_style_pad_all(ctx->list, 0, LV_STATE_DEFAULT);

    auto* button = lv_button_create(ctx->body);
    lv_obj_set_width(button, LV_PCT(100));
    ctx->startButtonLabel = lv_label_create(button);
    lv_label_set_text(ctx->startButtonLabel, "Start Monitor");
    lv_obj_center(ctx->startButtonLabel);
    lv_obj_add_event_cb(button, onStartAirtag, LV_EVENT_SHORT_CLICKED, ctx);

    ctx->countLabel = lv_label_create(ctx->body);
    lv_label_set_text(ctx->countLabel, "0 AirTag(s)");
}

// ---- Polling (timer task) ----

static void onPollTick(Context* ctx) {
    // Runs on the FreeRTOS timer daemon task, which is NOT the LVGL task. Mutating LVGL widgets
    // here (labels) requires the LVGL lock — exactly as BtManage's requestViewUpdate() does.
    // Without it the LVGL refresh task can race the invalidation and hang the app (task watchdog).
    lvgl_lock();
    if (ctx->statusLabel != nullptr) {
        const char* text;
        // No BLE device to drive: tell the user why the buttons appear to do nothing.
        if (ctx->dev == nullptr) {
            text = "No BLE device";
        } else if (ctx->spamRunning) {
            text = "Spamming";
        } else if (ctx->airtagRunning) {
            text = "Monitoring";
        } else if (ctx->scanRunning) {
            text = "Scanning";
        } else {
            text = "Stopped";
        }
        lv_label_set_text(ctx->statusLabel, text);
    }
    if (ctx->startButtonLabel != nullptr) {
        const char* text = ctx->scanRunning ? "Stop Scan" : "Start Scan";
        if (ctx->screen == Screen::Spam) {
            text = ctx->spamRunning ? "Stop Spam" : "Start Spam";
        } else if (ctx->screen == Screen::Airtag) {
            text = ctx->airtagRunning ? "Stop Monitor" : "Start Monitor";
        }
        lv_label_set_text(ctx->startButtonLabel, text);
    }
    if (ctx->screen == Screen::Airtag && ctx->countLabel != nullptr) {
        lv_label_set_text(ctx->countLabel, std::format("{} AirTag(s)", (unsigned)ctx->airtagPeers.size()).c_str());
    }
    if (ctx->screen == Screen::Scan && ctx->countLabel != nullptr) {
        lv_label_set_text(ctx->countLabel, std::format("{} devices", (unsigned)ctx->scanPeers.size()).c_str());
    }
    if (ctx->screen == Screen::Spam && ctx->spamLabel != nullptr) {
        lv_label_set_text(ctx->spamLabel, kPayloads[ctx->currentPayload].label);
    }
    lvgl_unlock();
}

// ---- App entry ----

static void createWidgets(lv_obj_t* parent, void* userData) {
    auto* ctx = static_cast<Context*>(userData);
    lv_obj_set_flex_flow(parent, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_row(parent, 0, LV_STATE_DEFAULT);

    auto* toolbar = lvgl_toolbar_create(parent, "BLE Toolbox");
    lvgl_toolbar_set_nav_action(toolbar, LV_SYMBOL_CLOSE, onBackPressed, ctx);

    ctx->body = lv_obj_create(parent);
    lv_obj_set_width(ctx->body, LV_PCT(100));
    lv_obj_set_flex_grow(ctx->body, 1);
    lv_obj_set_flex_flow(ctx->body, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_all(ctx->body, 8, LV_STATE_DEFAULT);
    lv_obj_set_style_pad_column(ctx->body, 6, LV_STATE_DEFAULT);
    lv_obj_set_scroll_dir(ctx->body, LV_DIR_VER);
    lv_obj_scroll_to_y(ctx->body, 0, LV_ANIM_OFF);

    showMainScreen(ctx);
}

int32_t appMain(int argc, char* argv[]) {
    uint32_t appInstanceId = app_scheduler_current_app_id();
    Context ctx;
    ctx.appInstanceId = appInstanceId;

    TaskEventGroup event_group;
    task_event_group_construct(&event_group);
    AppEventSubscription sub{};
    app_event_subscribe(&sub, &event_group);

    // Resolve the BLE device and subscribe to scan/radio events. The devicetree marks ble0
    // "disabled" (constructed but not started), so get_first_active_by_type() finds nothing and
    // every button would silently no-op (or crash on the null device in ensureBluetoothOn). Use
    // get_first_by_type() to find the not-yet-started device, then start it so the radio can be
    // driven and the driver is ready to log events.
#if defined(CONFIG_BT_NIMBLE_ENABLED)
    Device* dev = nullptr;
    if (device_get_first_by_type(&BLUETOOTH_TYPE, &dev) == ERROR_NONE && dev != nullptr) {
        ctx.dev = dev;
        bluetooth::start(dev);
        if (bluetooth_event_subscribe(dev, &ctx.btSub, &event_group) == ERROR_NONE) {
            LOG_I(TAG, "Subscribed to BLE events");
        } else {
            LOG_W(TAG, "Failed to subscribe to BLE events");
        }
    } else {
        LOG_W(TAG, "No Bluetooth device found");
        ctx.dev = nullptr;
    }
#endif

    ctx.pollTimer = std::make_unique<Timer>(Timer::Type::Periodic, millis_to_ticks(POLL_INTERVAL_MS), [&ctx] {
        onPollTick(&ctx);
    });
    ctx.spamTimer = std::make_unique<Timer>(Timer::Type::Periodic, millis_to_ticks(SPAM_INTERVAL_MS), [&ctx] {
        onSpamTick(&ctx);
    });
    ctx.pollTimer->start();

    WindowId window = window_manager_create(appInstanceId, createWidgets, &ctx);

    bool shouldClose = false;
    while (!shouldClose) {
        task_event_group_wait_any(&event_group, nullptr, portMAX_DELAY);

        AppEvent event{};
        while (app_event_poll(&sub, &event) == ERROR_NONE) {
            if (event.type == APP_EVENT_CLOSE) {
                shouldClose = true;
                break;
            }
        }

#if defined(CONFIG_BT_NIMBLE_ENABLED)
        if (ctx.dev != nullptr) {
            BtEvent bt_event{};
            while (bluetooth_event_poll(&ctx.btSub, &bt_event) == ERROR_NONE) {
                onBtEvent(&ctx, bt_event);
            }
        }
#endif

        // Rebuild the device list on the app task. This task is NOT the LVGL task, so the LVGL
        // lock must be held around any widget mutation first — the exact bug BtManage documents.
        // The peer-list rebuild creates/cleans many widgets, so it also needs the larger stack set
        // in the manifest below.
        if (ctx.listDirty && ctx.list != nullptr) {
            ctx.listDirty = false;
            lvgl_lock();
            rebuildList(&ctx);
            lvgl_unlock();
        }
    }

    stopAllActive(&ctx);
    ctx.pollTimer->stop();
    ctx.spamTimer->stop();

#if defined(CONFIG_BT_NIMBLE_ENABLED)
    if (ctx.dev != nullptr) {
        bluetooth_event_unsubscribe(ctx.dev, &ctx.btSub);
        if (bluetooth_is_scanning(ctx.dev)) {
            bluetooth_scan_stop(ctx.dev);
        }
        device_put(ctx.dev);
        ctx.dev = nullptr;
    }
#endif

    window_manager_remove(window);
    app_event_unsubscribe(&sub);
    task_event_group_destruct(&event_group);
    return 0;
}

} // namespace

uint32_t start() {
    uint32_t instanceId = 0;
    app_start(manifest.id, 0, nullptr, &instanceId);
    return instanceId;
}

extern const ::AppManifest manifest = {
    .id = "BleToolbox",
    .name = "BLE Toolbox",
    .category = APP_CATEGORY_USER,
    .location = { APP_LOCATION_MEMORY, reinterpret_cast<void*>(appMain) },
    .flags = 0,
    // The scan/AirTag peer-list rebuild does heavy LVGL work (creating/cleaning many widgets,
    // forcing layout, object-tree redraw recursion) on this app's own task. The default 8 KB
    // stack overflows under that redraw recursion, so give the app more headroom (as BtManage
    // does); the LVGL lock must also be held around those widget mutations.
    .stack = { .depth = 4096 }, // 16 KB
};

} // namespace tt::app::bletoolbox
