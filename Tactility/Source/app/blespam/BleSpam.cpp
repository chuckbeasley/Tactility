#ifdef ESP_PLATFORM
#include <sdkconfig.h>
#endif

#include <Tactility/Tactility.h>
#include <Tactility/Timer.h>
#include <Tactility/bluetooth/Bluetooth.h>

#include "BleSpamPayloads.h"

#include <app/event.h>
#include <app/manager.h>
#include <app/manifest.h>
#include <app/start.h>
#include <app/scheduler.h>

#include <lvgl_window_manager/window_manager.h>

#include <lvgl/lvgl.h>
#include <lvgl/widgets/toolbar.h>

#include <tactility/concurrent/task_event_group.h>
#include <tactility/device.h>
#include <tactility/drivers/bluetooth.h>
#include <tactility/log.h>
#include <tactility/time.h>

#include <memory>

namespace tt::app::blespam {

constexpr auto* TAG = "BleSpam";

extern const ::AppManifest manifest;

namespace {

// How long each spoofed packet is advertised before cycling to the next.
constexpr uint32_t CYCLE_INTERVAL_MS = 200;

struct Context {
    uint32_t appInstanceId = 0;
    bool enabled[kFamilyCount] = { true, true, true, true };
    // Written by the LVGL task (button handler), read by the timer task. Plain bool loads/
    // stores are atomic on 32-bit targets, so the worst case is one stale tick — acceptable.
    bool running = false;
    // Written by the LVGL task (switch handler), read by the timer task - same benign
    // one-tick race as `running`.
    bool randomizeAddress = true;
    // Written only by the timer task.
    size_t currentPayload = 0;
    std::unique_ptr<Timer> cycleTimer;
    lv_obj_t* startButtonLabel = nullptr;
    lv_obj_t* statusLabel = nullptr;
};

void onBackPressed(lv_event_t* event) {
    auto* ctx = static_cast<Context*>(lv_event_get_user_data(event));
    // Async, non-blocking - must NOT call app_manager_stop() directly here: that bound-waits
    // (thread_join) for this app's own thread, which would deadlock against the LVGL task.
    app_event_emit_close(ctx->appInstanceId);
}

void updateStatusLabel(Context* ctx) {
    if (ctx->statusLabel != nullptr) {
        lv_label_set_text(ctx->statusLabel, ctx->running ? "Running" : "Stopped");
    }
}

void updateStartButtonLabel(Context* ctx) {
    if (ctx->startButtonLabel != nullptr) {
        lv_label_set_text(ctx->startButtonLabel, ctx->running ? "Stop" : "Start");
    }
}

#if defined(CONFIG_BT_NIMBLE_ENABLED)

void ensureBluetoothOn() {
    Device* dev = nullptr;
    if (device_get_first_by_type(&BLUETOOTH_TYPE, &dev) == ERROR_NONE) {
        if (!bluetooth::isRadioOnOrPending(dev)) {
            LOG_I(TAG, "Enabling Bluetooth radio");
            bluetooth::start(dev);
        }
        device_put(dev);
    } else {
        LOG_W(TAG, "No Bluetooth device found");
    }
}

void stopAdvertisingSafe() {
    bluetooth::stopAdvertising();
}

// Runs on the FreeRTOS timer daemon task. NimBLE's advertising API is thread-safe, and the
// radio state is read from an atomic, so no LVGL lock is needed here.
void onCycleTick(Context* ctx) {
    if (!ctx->running) {
        return;
    }
    // The radio comes on asynchronously after ensureBluetoothOn() requests it; wait for it.
    if (bluetooth::getRadioState() != bluetooth::RadioState::On) {
        return;
    }
    if (kPayloadCount == 0) {
        return;
    }

    // Advance to the next enabled payload, wrapping around.
    for (size_t i = 0; i < kPayloadCount; i++) {
        ctx->currentPayload = (ctx->currentPayload + 1) % kPayloadCount;
        const Payload& payload = kPayloads[ctx->currentPayload];
        if (ctx->enabled[payload.family]) {
            bluetooth::startAdvertising(payload.data, payload.length, false, ctx->randomizeAddress);
            return;
        }
    }
}

#else // CONFIG_BT_NIMBLE_ENABLED

void ensureBluetoothOn() {}

void stopAdvertisingSafe() {}

void onCycleTick(Context* /*ctx*/) {}

#endif // CONFIG_BT_NIMBLE_ENABLED

void onStartStopClicked(lv_event_t* event) {
    auto* ctx = static_cast<Context*>(lv_event_get_user_data(event));
    ctx->running = !ctx->running;
    if (ctx->running) {
        ensureBluetoothOn();
        ctx->cycleTimer->start();
    } else {
        ctx->cycleTimer->stop();
        stopAdvertisingSafe();
    }
    updateStatusLabel(ctx);
    updateStartButtonLabel(ctx);
}

void onFamilySwitchChanged(lv_event_t* event) {
    auto* sw = static_cast<lv_obj_t*>(lv_event_get_target(event));
    auto* ctx = static_cast<Context*>(lv_event_get_user_data(event));
    uintptr_t family = reinterpret_cast<uintptr_t>(lv_obj_get_user_data(sw));
    if (family < kFamilyCount) {
        ctx->enabled[family] = lv_obj_has_state(sw, LV_STATE_CHECKED);
    }
}

void onRandomizeSwitchChanged(lv_event_t* event) {
    auto* sw = static_cast<lv_obj_t*>(lv_event_get_target(event));
    auto* ctx = static_cast<Context*>(lv_event_get_user_data(event));
    ctx->randomizeAddress = lv_obj_has_state(sw, LV_STATE_CHECKED);
}

void addFamilyRow(lv_obj_t* list, Context* ctx, size_t familyIndex) {
    auto* btn = lv_list_add_button(list, nullptr, kFamilies[familyIndex].label);
    auto* sw = lv_switch_create(btn);
    lv_obj_align(sw, LV_ALIGN_RIGHT_MID, -8, 0);
    lv_obj_set_user_data(sw, reinterpret_cast<void*>(static_cast<uintptr_t>(familyIndex)));
    lv_obj_add_event_cb(sw, onFamilySwitchChanged, LV_EVENT_VALUE_CHANGED, ctx);
    if (ctx->enabled[familyIndex]) {
        lv_obj_add_state(sw, LV_STATE_CHECKED);
    }
}

void addRandomizeRow(lv_obj_t* list, Context* ctx) {
    auto* btn = lv_list_add_button(list, nullptr, "Randomize address");
    auto* sw = lv_switch_create(btn);
    lv_obj_align(sw, LV_ALIGN_RIGHT_MID, -8, 0);
    lv_obj_add_event_cb(sw, onRandomizeSwitchChanged, LV_EVENT_VALUE_CHANGED, ctx);
    if (ctx->randomizeAddress) {
        lv_obj_add_state(sw, LV_STATE_CHECKED);
    }
}

void createWidgets(lv_obj_t* parent, void* userData) {
    auto* ctx = static_cast<Context*>(userData);

    lv_obj_set_flex_flow(parent, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_row(parent, 0, LV_STATE_DEFAULT);

    auto* toolbar = lvgl_toolbar_create(parent, "BLE Spam");
    lvgl_toolbar_set_nav_action(toolbar, LV_SYMBOL_CLOSE, onBackPressed, ctx);

    auto* list = lv_list_create(parent);
    lv_obj_set_width(list, LV_PCT(100));
    lv_obj_set_flex_grow(list, 1);

    for (size_t i = 0; i < kFamilyCount; i++) {
        addFamilyRow(list, ctx, i);
    }

    addRandomizeRow(list, ctx);

    auto* button = lv_button_create(parent);
    lv_obj_set_width(button, LV_PCT(100));
    ctx->startButtonLabel = lv_label_create(button);
    lv_label_set_text(ctx->startButtonLabel, "Start");
    lv_obj_center(ctx->startButtonLabel);
    lv_obj_add_event_cb(button, onStartStopClicked, LV_EVENT_SHORT_CLICKED, ctx);

    ctx->statusLabel = lv_label_create(parent);
    lv_label_set_text(ctx->statusLabel, "Stopped");
}

int32_t appMain(int argc, char* argv[]) {
    uint32_t appInstanceId = app_scheduler_current_app_id();
    Context ctx{};
    ctx.appInstanceId = appInstanceId;

    ctx.cycleTimer = std::make_unique<Timer>(Timer::Type::Periodic, millis_to_ticks(CYCLE_INTERVAL_MS), [&ctx] {
        onCycleTick(&ctx);
    });

    TaskEventGroup event_group;
    task_event_group_construct(&event_group);
    AppEventSubscription sub{};
    app_event_subscribe(&sub, &event_group);

    WindowId window = window_manager_create(appInstanceId, createWidgets, &ctx);

    bool shouldClose = false;
    while (!shouldClose) {
        task_event_group_wait_any(&event_group, nullptr, portMAX_DELAY);
        AppEvent event{};
        while (app_event_poll(&sub, &event) == ERROR_NONE) {
        switch (event.type) {
            case APP_EVENT_CLOSE:
                ctx.running = false;
                ctx.cycleTimer->stop();
                stopAdvertisingSafe();
                shouldClose = true;
                break;
            default:
                break;
        }
        }
    }

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
    .id = "BleSpam",
    .name = "BLE Spam",
    .category = APP_CATEGORY_USER,
    .location = { APP_LOCATION_MEMORY, reinterpret_cast<void*>(appMain) }
};

} // namespace tt::app::blespam
