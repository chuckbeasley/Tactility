// SPDX-License-Identifier: Apache-2.0
//
// Motion: the QMI8658's accelerometer and gyroscope, live, with a bubble-level tilt indicator.
//
// This exists because the driver was otherwise unused: nothing in the tree consumed the IMU, so
// there was no way to see whether it worked without a serial probe. The dial is the point of it -
// numbers tell you the chip answers, but a dot that follows gravity tells you at a glance which way
// the board is tilted, and a level marker makes "is this thing flat" a one-second check for anyone
// calibrating the touch panel or mounting the board.
//
// The whole file is conditional because the app calls the QMI8658 driver directly (as the touch
// calibration app calls the XPT2046 one), so it can only be built for boards that list that driver
// module - CONFIG_TT_IMU_SUPPORTED is set from the device's hardware.imu property, and the
// registration in Tactility.cpp carries the same guard.
#ifdef ESP_PLATFORM
#include <sdkconfig.h>
#endif

#if defined(CONFIG_TT_IMU_SUPPORTED)

#include <Tactility/Tactility.h>

#include <app/event.h>
#include <app/manager.h>
#include <app/manifest.h>
#include <app/scheduler.h>

#include <lvgl_window_manager/window_manager.h>

#include <drivers/qmi8658.h>

#include <lvgl/lvgl.h>
#include <lvgl/widgets/toolbar.h>
#include <tactility/check.h>
#include <tactility/concurrent/task_event_group.h>
#include <tactility/device.h>
#include <tactility/log.h>

#include <lvgl.h>

#include <cmath>
#include <cstdio>

namespace tt::app::motion {

constexpr auto* TAG = "Motion";

// 10 Hz. Fast enough that the dial follows a hand tilting the board, slow enough that the labels are
// readable and the app costs nothing while it is open.
constexpr TickType_t UPDATE_INTERVAL_TICKS = pdMS_TO_TICKS(100);

constexpr int32_t DIAL_SIZE = 150;
constexpr int32_t DIAL_RADIUS = DIAL_SIZE / 2;
constexpr int32_t DOT_SIZE = 20;

// Within this many degrees of flat, the indicator says so and turns green.
constexpr float LEVEL_TOLERANCE_DEGREES = 3.0f;

struct Context {
    Device* device = nullptr;
    // Set by the toolbar's close button, which arrives as APP_EVENT_CLOSE.
    bool closeRequested = false;
    TaskEventGroup eventGroup {};

    lv_obj_t* dot = nullptr;
    lv_obj_t* tiltLabel = nullptr;
    // One label per group rather than one per value: LVGL object creation costs this board the better
    // part of a kilobyte of stack each, and three rows of three objects is nine of them for what two
    // multi-line labels say just as clearly.
    lv_obj_t* accelValues = nullptr;
    lv_obj_t* gyroValues = nullptr;
    lv_obj_t* totalValue = nullptr;
};

void onBackPressed(lv_event_t* event) {
    static_cast<Context*>(lv_event_get_user_data(event))->closeRequested = true;
}

/** Formats a value in thousandths as e.g. "-0.998" or "12.5", without relying on float printf. */
void formatThousandths(char* out, size_t size, int32_t thousandths, const char* suffix) {
    const int32_t magnitude = thousandths < 0 ? -thousandths : thousandths;
    snprintf(out, size, "%s%d.%03d%s", thousandths < 0 ? "-" : "", (int)(magnitude / 1000),
             (int)(magnitude % 1000), suffix);
}

/** Formats a value in hundredths, for the two-decimal figures. */
void formatHundredths(char* out, size_t size, int32_t hundredths, const char* suffix) {
    const int32_t magnitude = hundredths < 0 ? -hundredths : hundredths;
    snprintf(out, size, "%s%d.%02d%s", hundredths < 0 ? "-" : "", (int)(magnitude / 100),
             (int)(magnitude % 100), suffix);
}

/** A thin line, used for the dial's crosshair. */
void createCrosshair(lv_obj_t* parent, bool horizontal) {
    auto* line = lv_obj_create(parent);
    lv_obj_set_size(line, horizontal ? DIAL_SIZE - 16 : 1, horizontal ? 1 : DIAL_SIZE - 16);
    lv_obj_set_style_bg_color(line, lv_palette_main(LV_PALETTE_GREY), LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(line, LV_OPA_40, LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(line, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_radius(line, 0, LV_STATE_DEFAULT);
    lv_obj_center(line);
}

void createTiltDial(lv_obj_t* parent, Context* ctx) {
    auto* dial = lv_obj_create(parent);
    lv_obj_set_size(dial, DIAL_SIZE, DIAL_SIZE);
    lv_obj_set_style_radius(dial, LV_RADIUS_CIRCLE, LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(dial, lv_color_hex(0x202020), LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(dial, lv_palette_main(LV_PALETTE_GREY), LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(dial, 2, LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(dial, 0, LV_STATE_DEFAULT);
    lv_obj_remove_flag(dial, LV_OBJ_FLAG_SCROLLABLE);

    createCrosshair(dial, true);
    createCrosshair(dial, false);

    ctx->dot = lv_obj_create(dial);
    lv_obj_set_size(ctx->dot, DOT_SIZE, DOT_SIZE);
    lv_obj_set_style_radius(ctx->dot, LV_RADIUS_CIRCLE, LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ctx->dot, lv_palette_main(LV_PALETTE_BLUE), LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ctx->dot, 0, LV_STATE_DEFAULT);
    lv_obj_align(ctx->dot, LV_ALIGN_CENTER, 0, 0);
}

/** A left column of names and a right column of values, as one row of two multi-line labels. */
lv_obj_t* createValueBlock(lv_obj_t* parent, const char* names) {
    auto* row = lv_obj_create(parent);
    lv_obj_set_width(row, LV_PCT(100));
    lv_obj_set_height(row, LV_SIZE_CONTENT);
    lv_obj_set_style_pad_all(row, 2, LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(row, 6, LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(row, 0, LV_STATE_DEFAULT);

    auto* nameLabel = lv_label_create(row);
    lv_label_set_text(nameLabel, names);
    lv_obj_align(nameLabel, LV_ALIGN_TOP_LEFT, 0, 0);

    auto* valueLabel = lv_label_create(row);
    lv_label_set_text(valueLabel, "-");
    lv_obj_align(valueLabel, LV_ALIGN_TOP_RIGHT, 0, 0);
    return valueLabel;
}

void createWidgets(lv_obj_t* parent, void* userData) {
    auto* ctx = static_cast<Context*>(userData);

    lv_obj_set_flex_flow(parent, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_row(parent, 4, LV_STATE_DEFAULT);

    auto* toolbar = lvgl_toolbar_create(parent, "Motion");
    lvgl_toolbar_set_nav_action(toolbar, LV_SYMBOL_CLOSE, onBackPressed, ctx);

    if (ctx->device == nullptr) {
        auto* message = lv_label_create(parent);
        lv_label_set_long_mode(message, LV_LABEL_LONG_MODE_WRAP);
        lv_obj_set_width(message, LV_PCT(100));
        lv_label_set_text(message, "No QMI8658 IMU was found on this board.");
        return;
    }

    auto* dialRow = lv_obj_create(parent);
    lv_obj_set_width(dialRow, LV_PCT(100));
    lv_obj_set_height(dialRow, LV_SIZE_CONTENT);
    lv_obj_set_style_border_width(dialRow, 0, LV_STATE_DEFAULT);
    createTiltDial(dialRow, ctx);

    ctx->tiltLabel = lv_label_create(parent);
    lv_obj_set_width(ctx->tiltLabel, LV_PCT(100));
    lv_obj_set_style_text_align(ctx->tiltLabel, LV_TEXT_ALIGN_CENTER, LV_STATE_DEFAULT);
    lv_label_set_text(ctx->tiltLabel, "reading...");

    auto* accelHeader = lv_label_create(parent);
    lv_label_set_text(accelHeader, "Accelerometer");
    lv_obj_set_style_text_color(accelHeader, lv_palette_main(LV_PALETTE_GREY), LV_STATE_DEFAULT);
    ctx->accelValues = createValueBlock(parent, "X\nY\nZ");
    ctx->totalValue = createValueBlock(parent, "Total");

    auto* gyroHeader = lv_label_create(parent);
    lv_label_set_text(gyroHeader, "Gyroscope");
    lv_obj_set_style_text_color(gyroHeader, lv_palette_main(LV_PALETTE_GREY), LV_STATE_DEFAULT);
    ctx->gyroValues = createValueBlock(parent, "X\nY\nZ");
}

void destroyWidgets(void* userData) {
    auto* ctx = static_cast<Context*>(userData);
    ctx->dot = nullptr;
    ctx->tiltLabel = nullptr;
    ctx->accelValues = nullptr;
    ctx->gyroValues = nullptr;
    ctx->totalValue = nullptr;
}

/** Turns one device read into the dial position and the labels. */
void refresh(Context* ctx) {
    Qmi8658Data data = {};
    if (qmi8658_read(ctx->device, &data) != ERROR_NONE) {
        lv_label_set_text(ctx->tiltLabel, "read failed");
        return;
    }

    // The dot follows the gravity vector's projection onto the board: at rest, ax and ay are the
    // components in the board's own plane, so they map straight onto the dial's axes (screen y grows
    // downwards, and so does +ay, so no sign flip is needed on y).
    const float scale = (float)(DIAL_RADIUS - DOT_SIZE / 2);
    const float clampedX = data.ax < -1.0f ? -1.0f : (data.ax > 1.0f ? 1.0f : data.ax);
    const float clampedY = data.ay < -1.0f ? -1.0f : (data.ay > 1.0f ? 1.0f : data.ay);
    lv_obj_align(ctx->dot, LV_ALIGN_CENTER, (int32_t)(clampedX * scale), (int32_t)(clampedY * scale));

    // Pitch and roll from gravity, in degrees: the same convention a level app uses.
    const float pitch = atan2f(-data.ax, sqrtf(data.ay * data.ay + data.az * data.az)) * 57.29578f;
    const float roll = atan2f(data.ay, data.az) * 57.29578f;
    const bool level = fabsf(pitch) <= LEVEL_TOLERANCE_DEGREES && fabsf(roll) <= LEVEL_TOLERANCE_DEGREES;
    lv_obj_set_style_bg_color(ctx->dot,
                              lv_palette_main(level ? LV_PALETTE_GREEN : LV_PALETTE_BLUE), LV_STATE_DEFAULT);

    char pitchText[24] = {};
    char rollText[24] = {};
    // Two decimals: a tenth of a degree is finer than the sensor's own noise, and three made the line
    // look more precise than it is.
    formatHundredths(pitchText, sizeof(pitchText), (int32_t)(pitch * 100.0f), "");
    formatHundredths(rollText, sizeof(rollText), (int32_t)(roll * 100.0f), "");
    char tiltText[80] = {};
    snprintf(tiltText, sizeof(tiltText), "Pitch %s   Roll %s%s", pitchText, rollText, level ? "   (level)" : "");
    lv_label_set_text(ctx->tiltLabel, tiltText);

    char axText[24] = {};
    char ayText[24] = {};
    char azText[24] = {};
    formatThousandths(axText, sizeof(axText), (int32_t)(data.ax * 1000.0f), " g");
    formatThousandths(ayText, sizeof(ayText), (int32_t)(data.ay * 1000.0f), " g");
    formatThousandths(azText, sizeof(azText), (int32_t)(data.az * 1000.0f), " g");
    char stacked[80] = {};
    snprintf(stacked, sizeof(stacked), "%s\n%s\n%s", axText, ayText, azText);
    lv_label_set_text(ctx->accelValues, stacked);

    char totalText[24] = {};
    const float magnitude = sqrtf(data.ax * data.ax + data.ay * data.ay + data.az * data.az);
    formatThousandths(totalText, sizeof(totalText), (int32_t)(magnitude * 1000.0f), " g");
    lv_label_set_text(ctx->totalValue, totalText);

    char gxText[24] = {};
    char gyText[24] = {};
    char gzText[24] = {};
    formatHundredths(gxText, sizeof(gxText), (int32_t)(data.gx * 100.0f), " deg/s");
    formatHundredths(gyText, sizeof(gyText), (int32_t)(data.gy * 100.0f), " deg/s");
    formatHundredths(gzText, sizeof(gzText), (int32_t)(data.gz * 100.0f), " deg/s");
    char gyroStacked[80] = {};
    snprintf(gyroStacked, sizeof(gyroStacked), "%s\n%s\n%s", gxText, gyText, gzText);
    lv_label_set_text(ctx->gyroValues, gyroStacked);
}

int32_t appMain(int argc, char* argv[]) {
    Context ctx {};
    device_get_by_name("imu0", &ctx.device);
    if (ctx.device == nullptr) {
        LOG_W(TAG, "no imu0 device");
    }

    task_event_group_construct(&ctx.eventGroup);
    AppEventSubscription subscription {};
    check(app_event_subscribe(&subscription, &ctx.eventGroup) == ERROR_NONE);

    WindowId window = window_manager_create_ext(
        app_scheduler_current_app_id(), createWidgets, destroyWidgets, &ctx);

    while (!ctx.closeRequested) {
        task_event_group_wait_any(&ctx.eventGroup, nullptr, UPDATE_INTERVAL_TICKS);

        AppEvent event {};
        while (app_event_poll(&subscription, &event) == ERROR_NONE) {
            if (event.type == APP_EVENT_CLOSE) {
                ctx.closeRequested = true;
            }
        }

        // Read and render on this task's own schedule: the labels only change on the LVGL task, under
        // its lock, and the sensor read itself takes no lock at all.
        if (!ctx.closeRequested && ctx.device != nullptr && ctx.dot != nullptr) {
            if (lvgl_try_lock(pdMS_TO_TICKS(50))) {
                refresh(&ctx);
                lvgl_unlock();
            }
        }
    }

    window_manager_remove(window);
    check(app_event_unsubscribe(&subscription) == ERROR_NONE);
    task_event_group_destruct(&ctx.eventGroup);
    return 0;
}

extern const ::AppManifest manifest = {
    .id = "tactility.motion",
    .name = "Motion",
    .category = APP_CATEGORY_SETTINGS,
    .location = { .type = APP_LOCATION_MEMORY, .location = reinterpret_cast<void*>(appMain) },
    // 4 KB (stack depths here are bytes, not words - see APP_STACK_SIZE_MAX in app/manifest.h). The
    // honest state of the evidence: 1,536 bytes crashed on launch with a stack protection fault
    // inside lv_obj_get_style_prop(), and 4 KB has run stably since, including three concurrent
    // instances. The exact peak is NOT known: uxTaskGetStackHighWaterMark() is not usable on this
    // build - it reports 5,664 bytes free on a fresh 24 KB stack before the task has run a line of
    // its own code, so its scan is not finding FreeRTOS' fill pattern and every "headroom" number it
    // produces is meaningless.
    .stack = { .depth = 4096, .desired_memory_capability = 0 },
};

} // namespace tt::app::motion

#endif // defined(CONFIG_TT_IMU_SUPPORTED)
