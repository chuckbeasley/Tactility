#include <Tactility/app/touchcalibration/TouchCalibration.h>

#if defined(CONFIG_TT_TOUCH_CALIBRATION_SUPPORTED)

#include <Tactility/Tactility.h>
#include <Tactility/settings/TouchCalibrationSettings.h>

#include <app/event.h>
#include <app/manager.h>
#include <app/manifest.h>

#include <lvgl_window_manager/window_manager.h>

#include <tactility/log.h>
#include <tactility/device.h>
#include <tactility/drivers/pointer.h>
#include <drivers/xpt2046_softspi.h>
#include <lvgl/lvgl.h>
#include <lvgl/devices/pointer.h>

#include <algorithm>
#include <lvgl.h>
#include <tactility/error.h>

namespace tt::app::touchcalibration {

constexpr auto* TAG = "TouchCalibration";

extern const ::AppManifest manifest;

namespace {

// Touch_Calibrate assumes corner samples are taken near the true display corners.
// Keep this tight to avoid shrinking the measured span (which biases bottom/right inward).
constexpr int32_t TARGET_MARGIN = 6;
constexpr int32_t TARGET_SIZE = 28;
constexpr int32_t TARGET_HALF = TARGET_SIZE / 2;
constexpr int32_t TARGET_SAFE_INSET = 36;
constexpr uint8_t RAW_SAMPLE_COUNT = 8;
constexpr TickType_t RAW_SAMPLE_ATTEMPT_TIMEOUT = pdMS_TO_TICKS(12);
constexpr TickType_t RAW_SAMPLE_CAPTURE_WINDOW = pdMS_TO_TICKS(350);
constexpr uint16_t CALIBRATION_TOUCH_THRESHOLD = 175; // Touch_Calibrate uses Z_THRESHOLD/2

struct Sample {
    uint16_t x;
    uint16_t y;
};

struct Context {
    uint32_t appInstanceId;

    Sample samples[4] = {};
    lv_point_t targetScreenPoints[4] = {};
    uint8_t sampleCount = 0;
    bool calibrationApplied = false;

    lv_obj_t* root = nullptr;
    lv_obj_t* target = nullptr;
    lv_obj_t* titleLabel = nullptr;
    lv_obj_t* hintLabel = nullptr;
    bool touchLatch = false;
};

lv_point_t getTargetPoint(uint8_t index, lv_coord_t width, lv_coord_t height);

static bool readRawTouchPoint(lv_point_t* outPoint) {
    Device* pointerDevice = nullptr;
    if (device_get_first_by_type(&POINTER_TYPE, &pointerDevice) != ERROR_NONE || pointerDevice == nullptr) {
        return false;
    }

    bool touched = false;
    if (pointer_read_data(pointerDevice, RAW_SAMPLE_ATTEMPT_TIMEOUT) == ERROR_NONE) {
        uint16_t x = 0;
        uint16_t y = 0;
        uint8_t pointCount = 0;
        touched = pointer_get_touched_points(pointerDevice, &x, &y, nullptr, &pointCount, 1) && pointCount > 0;
        if (touched) {
            outPoint->x = static_cast<lv_coord_t>(x);
            outPoint->y = static_cast<lv_coord_t>(y);
        }
    }

    device_put(pointerDevice);
    return touched;
}

static bool captureStableRawSample(lv_point_t* outPoint) {
    Device* pointerDevice = nullptr;
    if (device_get_first_by_type(&POINTER_TYPE, &pointerDevice) != ERROR_NONE || pointerDevice == nullptr) {
        return false;
    }

    int32_t sumX = 0;
    int32_t sumY = 0;
    uint8_t captured = 0;
    const uint16_t thresholds[] = { CALIBRATION_TOUCH_THRESHOLD, 100, 60, 20 };
    const TickType_t startTick = xTaskGetTickCount();

    while (captured < RAW_SAMPLE_COUNT && (xTaskGetTickCount() - startTick) < RAW_SAMPLE_CAPTURE_WINDOW) {
        bool accepted = false;
        uint16_t x = 0;
        uint16_t y = 0;
        for (uint16_t threshold : thresholds) {
            if (xpt2046_softspi_read_valid_touch(pointerDevice, &x, &y, threshold, RAW_SAMPLE_ATTEMPT_TIMEOUT) == ERROR_NONE) {
                accepted = true;
                break;
            }
        }
        if (!accepted) {
            if (pointer_read_data(pointerDevice, RAW_SAMPLE_ATTEMPT_TIMEOUT) == ERROR_NONE) {
                uint8_t pointCount = 0;
                accepted = pointer_get_touched_points(pointerDevice, &x, &y, nullptr, &pointCount, 1) && pointCount > 0;
            }
        }
        if (!accepted) break;

        sumX += x;
        sumY += y;
        captured++;
    }

    device_put(pointerDevice);

    if (captured < 1) {
        return false;
    }

    outPoint->x = static_cast<lv_coord_t>(sumX / captured);
    outPoint->y = static_cast<lv_coord_t>(sumY / captured);
    return true;
}


lv_point_t getTargetPoint(uint8_t index, lv_coord_t width, lv_coord_t height) {
    const lv_coord_t left = std::max<lv_coord_t>(TARGET_MARGIN, TARGET_SAFE_INSET);
    const lv_coord_t top = std::max<lv_coord_t>(TARGET_MARGIN, TARGET_SAFE_INSET);
    const lv_coord_t right = std::max<lv_coord_t>(left, width - 1 - left);
    const lv_coord_t bottom = std::max<lv_coord_t>(top, height - 1 - top);

    switch (index) {
        case 0:
            return {.x = left, .y = top};
        case 1:
            return {.x = left, .y = bottom};
        case 2:
            return {.x = right, .y = top};
        default:
            return {.x = right, .y = bottom};
    }
}

static bool solveAxisFromTargetPositions(
    int32_t rawMin,
    int32_t rawMax,
    int32_t screenMin,
    int32_t screenMax,
    int32_t targetMax,
    int32_t* outCalMin,
    int32_t* outCalMax
) {
    if (outCalMin == nullptr || outCalMax == nullptr) {
        return false;
    }
    if (rawMax <= rawMin || screenMax <= screenMin || targetMax <= 0) {
        return false;
    }

    // Solve calibration-space min/max so:
    // map(rawMin) == screenMin and map(rawMax) == screenMax
    // for map(raw) = (raw - calMin) * targetMax / (calMax - calMin).
    const int64_t rawSpan = (int64_t)rawMax - rawMin;
    const int64_t screenSpan = (int64_t)screenMax - screenMin;
    const int64_t calSpan = (rawSpan * targetMax) / screenSpan;
    if (calSpan <= 0) {
        return false;
    }

    const int64_t calMin = (int64_t)rawMin - ((int64_t)screenMin * calSpan) / targetMax;
    const int64_t calMax = calMin + calSpan;
    if (calMax <= calMin) {
        return false;
    }

    *outCalMin = (int32_t)calMin;
    *outCalMax = (int32_t)calMax;
    return true;
}

void updateUi(Context* ctx) {
    if (ctx->target == nullptr || ctx->root == nullptr || ctx->titleLabel == nullptr || ctx->hintLabel == nullptr) {
        return;
    }

    const auto width = lv_obj_get_content_width(ctx->root);
    const auto height = lv_obj_get_content_height(ctx->root);

    if (ctx->sampleCount < 4) {
        const auto point = getTargetPoint(ctx->sampleCount, width, height);
        lv_obj_set_pos(ctx->target, point.x - TARGET_HALF, point.y - TARGET_HALF);
        lv_label_set_text(ctx->titleLabel, "Touchscreen Calibration");
        lv_label_set_text_fmt(ctx->hintLabel, "Tap target %u/4", static_cast<unsigned>(ctx->sampleCount + 1));
    }
}

// Drives the on-screen outcome text/state; the actual result (Ok/Error) is reported to the
// caller from onPress() below, via ctx->calibrationApplied, once the user taps to dismiss.
void finishCalibration(Context* ctx) {
    // Full TFT_eSPI Touch_calibrate math port:
    // sample order: up-left, bottom-left, up-right, bottom-right.
    const int32_t values[8] = {
        static_cast<int32_t>(ctx->samples[0].x), static_cast<int32_t>(ctx->samples[0].y),
        static_cast<int32_t>(ctx->samples[1].x), static_cast<int32_t>(ctx->samples[1].y),
        static_cast<int32_t>(ctx->samples[2].x), static_cast<int32_t>(ctx->samples[2].y),
        static_cast<int32_t>(ctx->samples[3].x), static_cast<int32_t>(ctx->samples[3].y),
    };

    bool rotate_xy = false;
    bool invert_x = false;
    bool invert_y = false;
    int32_t rawXMin = 0;
    int32_t rawXMax = 0;
    int32_t rawYMin = 0;
    int32_t rawYMax = 0;

    if (std::abs(values[0] - values[2]) > std::abs(values[1] - values[3])) {
        rotate_xy = true;
        rawXMin = (values[1] + values[3]) / 2;
        rawXMax = (values[5] + values[7]) / 2;
        rawYMin = (values[0] + values[4]) / 2;
        rawYMax = (values[2] + values[6]) / 2;
    } else {
        rotate_xy = false;
        rawXMin = (values[0] + values[2]) / 2;
        rawXMax = (values[4] + values[6]) / 2;
        rawYMin = (values[1] + values[5]) / 2;
        rawYMax = (values[3] + values[7]) / 2;
    }

    if (rawXMin > rawXMax) {
        std::swap(rawXMin, rawXMax);
        invert_x = true;
    }
    if (rawYMin > rawYMax) {
        std::swap(rawYMin, rawYMax);
        invert_y = true;
    }

    auto* display = lv_display_get_default();
    const int32_t targetXMax = display != nullptr ? lv_display_get_horizontal_resolution(display) - 1 : 0;
    const int32_t targetYMax = display != nullptr ? lv_display_get_vertical_resolution(display) - 1 : 0;

    const int32_t screenXMin = (ctx->targetScreenPoints[0].x + ctx->targetScreenPoints[1].x) / 2;
    const int32_t screenXMax = (ctx->targetScreenPoints[2].x + ctx->targetScreenPoints[3].x) / 2;
    const int32_t screenYMin = (ctx->targetScreenPoints[0].y + ctx->targetScreenPoints[2].y) / 2;
    const int32_t screenYMax = (ctx->targetScreenPoints[1].y + ctx->targetScreenPoints[3].y) / 2;

    int32_t xSolveAtRawMin = invert_x ? screenXMax : screenXMin;
    int32_t xSolveAtRawMax = invert_x ? screenXMin : screenXMax;
    int32_t ySolveAtRawMin = invert_y ? screenYMax : screenYMin;
    int32_t ySolveAtRawMax = invert_y ? screenYMin : screenYMax;
    if (invert_x) {
        xSolveAtRawMin = targetXMax - xSolveAtRawMin;
        xSolveAtRawMax = targetXMax - xSolveAtRawMax;
    }
    if (invert_y) {
        ySolveAtRawMin = targetYMax - ySolveAtRawMin;
        ySolveAtRawMax = targetYMax - ySolveAtRawMax;
    }

    int32_t xMin = rawXMin;
    int32_t xMax = rawXMax;
    int32_t yMin = rawYMin;
    int32_t yMax = rawYMax;
    const bool solvedX = solveAxisFromTargetPositions(rawXMin, rawXMax, xSolveAtRawMin, xSolveAtRawMax, targetXMax, &xMin, &xMax);
    const bool solvedY = solveAxisFromTargetPositions(rawYMin, rawYMax, ySolveAtRawMin, ySolveAtRawMax, targetYMax, &yMin, &yMax);
    if (!solvedX || !solvedY) {
        lv_label_set_text(ctx->titleLabel, "Calibration Failed");
        lv_label_set_text(ctx->hintLabel, "Target solve failed. Tap to close.");
        lv_obj_add_flag(ctx->target, LV_OBJ_FLAG_HIDDEN);
        return;
    }

    settings::touch::TouchCalibrationSettings settings = settings::touch::getDefault();
    settings.enabled = true;
    settings.xMin = xMin;
    settings.xMax = xMax;
    settings.yMin = yMin;
    settings.yMax = yMax;
    settings.rotateXy = rotate_xy;
    settings.invertX = invert_x;
    settings.invertY = invert_y;

    if (!settings::touch::isValid(settings)) {
        lv_label_set_text(ctx->titleLabel, "Calibration Failed");
        lv_label_set_text(ctx->hintLabel, "Range invalid. Tap to close.");
        lv_obj_add_flag(ctx->target, LV_OBJ_FLAG_HIDDEN);
        return;
    }

    if (!settings::touch::save(settings)) {
        lv_label_set_text(ctx->titleLabel, "Calibration Failed");
        lv_label_set_text(ctx->hintLabel, "Unable to save settings. Tap to close.");
        lv_obj_add_flag(ctx->target, LV_OBJ_FLAG_HIDDEN);
        return;
    }

    LvglPointerCalibration calibration = {
        .x_min = xMin,
        .x_max = xMax,
        .y_min = yMin,
        .y_max = yMax,
        .rotate_xy = rotate_xy,
        .invert_x = invert_x,
        .invert_y = invert_y,
    };
    auto* indev = lvgl_pointer_get_default();
    if (indev != nullptr) {
        lvgl_pointer_set_calibration(indev, &calibration);
    }
    ctx->calibrationApplied = true;

    LOG_I(TAG, "Saved calibration x=[%d, %d] y=[%d, %d] flags rotate=%d invertX=%d invertY=%d",
        xMin, xMax, yMin, yMax, (int)rotate_xy, (int)invert_x, (int)invert_y);
    lv_label_set_text(ctx->titleLabel, "Calibration Complete");
    lv_label_set_text(ctx->hintLabel, "Touch anywhere to continue.");
    lv_obj_add_flag(ctx->target, LV_OBJ_FLAG_HIDDEN);
}

static void recordSample(Context* ctx, const lv_point_t& sampledRaw) {
    if (ctx->sampleCount >= 4) {
        return;
    }
    const uint8_t sampleIndex = ctx->sampleCount;
    if (ctx->root != nullptr) {
        lv_area_t rootArea {};
        lv_obj_get_coords(ctx->root, &rootArea);
        const auto localPoint = getTargetPoint(sampleIndex, lv_obj_get_content_width(ctx->root), lv_obj_get_content_height(ctx->root));
        ctx->targetScreenPoints[sampleIndex] = {
            .x = static_cast<lv_coord_t>(rootArea.x1 + localPoint.x),
            .y = static_cast<lv_coord_t>(rootArea.y1 + localPoint.y),
        };
    }
    ctx->samples[sampleIndex] = {
        .x = static_cast<uint16_t>(sampledRaw.x),
        .y = static_cast<uint16_t>(sampledRaw.y),
    };
    ctx->sampleCount++;

    lvgl_lock();
    if (ctx->sampleCount < 4) {
        updateUi(ctx);
    } else {
        finishCalibration(ctx);
    }
    lvgl_unlock();
}

void onPress(lv_event_t* event) {
    auto* ctx = static_cast<Context*>(lv_event_get_user_data(event));

    if (ctx->sampleCount < 4) {
        return;
    }

    // Async, non-blocking - must NOT call app_manager_stop()/app_manager_finish() directly
    // here: this callback runs ON the LVGL task, and app-lifecycle transitions must happen on
    // this app's own thread (woken up via app_event_await() below). The result (Ok/Error) is
    // reported by appMain() itself when it returns, based on ctx.calibrationApplied.
    AppEvent closeEvent { .type = APP_EVENT_CLOSE, .timestamp = 0, .result = {} };
    app_event_emit(ctx->appInstanceId, &closeEvent);
}

void createWidgets(lv_obj_t* parent, void* userData) {
    auto* ctx = static_cast<Context*>(userData);

    lv_obj_set_style_bg_color(parent, lv_color_black(), LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(parent, LV_OPA_COVER, LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(parent, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_radius(parent, 0, LV_STATE_DEFAULT);

    ctx->root = lv_obj_create(parent);
    lv_obj_set_size(ctx->root, LV_PCT(100), LV_PCT(100));
    lv_obj_set_style_bg_opa(ctx->root, LV_OPA_TRANSP, LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ctx->root, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(ctx->root, 0, LV_STATE_DEFAULT);

    ctx->titleLabel = lv_label_create(ctx->root);
    lv_obj_align(ctx->titleLabel, LV_ALIGN_TOP_MID, 0, 14);
    lv_obj_set_style_text_color(ctx->titleLabel, lv_color_white(), LV_STATE_DEFAULT);
    lv_label_set_text(ctx->titleLabel, "Touchscreen Calibration");

    ctx->hintLabel = lv_label_create(ctx->root);
    lv_obj_align(ctx->hintLabel, LV_ALIGN_BOTTOM_MID, 0, -14);
    lv_obj_set_style_text_color(ctx->hintLabel, lv_color_white(), LV_STATE_DEFAULT);
    lv_label_set_text(ctx->hintLabel, "Tap target 1/4");

    ctx->target = lv_button_create(ctx->root);
    lv_obj_set_size(ctx->target, TARGET_SIZE, TARGET_SIZE);
    lv_obj_set_style_radius(ctx->target, LV_RADIUS_CIRCLE, LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ctx->target, lv_palette_main(LV_PALETTE_RED), LV_STATE_DEFAULT);
    // Ensure root receives all presses for sampling.
    lv_obj_remove_flag(ctx->target, LV_OBJ_FLAG_CLICKABLE);

    auto* targetLabel = lv_label_create(ctx->target);
    lv_label_set_text(targetLabel, "+");
    lv_obj_center(targetLabel);

    lv_obj_add_flag(ctx->root, LV_OBJ_FLAG_CLICKABLE);
    // Use initial press point; release tends to drift toward the center on resistive panels,
    // which biases bottom/right targets upward/leftward.
    lv_obj_add_event_cb(ctx->root, onPress, LV_EVENT_PRESSED, ctx);

    updateUi(ctx);
}

int32_t appMain(uint32_t appInstanceId, int argc, char* argv[]) {
    Context ctx {};
    ctx.appInstanceId = appInstanceId;

    // Clear any active calibration so the taps sampled below are raw, uncalibrated coordinates.
    lvgl_lock();
    auto* startIndev = lvgl_pointer_get_default();
    if (startIndev != nullptr) {
        lvgl_pointer_set_calibration(startIndev, nullptr);
    }
    lvgl_unlock();

    AppEventSubscription sub {};
    sub.app_instance_id = appInstanceId;
    app_event_subscribe(&sub);

    WindowId window = window_manager_create(appInstanceId, createWidgets, &ctx);

    bool shouldClose = false;
    while (!shouldClose) {
        AppEvent event {};
        error_t eventError = app_event_await(&sub, &event, pdMS_TO_TICKS(20));
        if (eventError == ERROR_NONE) {
            switch (event.type) {
                case APP_EVENT_CLOSE:
                    app_manager_finish(appInstanceId);
                    shouldClose = true;
                    break;
                default:
                    break;
            }
        } else if (eventError != ERROR_TIMEOUT) {
            break;
        }

        if (!shouldClose && ctx.sampleCount < 4) {
            lv_point_t currentPoint = {0, 0};
            const bool touched = readRawTouchPoint(&currentPoint);
            if (touched && !ctx.touchLatch) {
                lv_point_t sampledRaw = {0, 0};
                if (captureStableRawSample(&sampledRaw)) {
                    recordSample(&ctx, sampledRaw);
                    ctx.touchLatch = true;
                }
            } else if (!touched) {
                ctx.touchLatch = false;
            }
        }
    }

    window_manager_remove(window);
    app_event_unsubscribe(&sub);

    // finishCalibration() already applied a new calibration on success. On cancel/failure,
    // restore whatever calibration was on disk before the block above cleared it.
    if (!ctx.calibrationApplied) {
        settings::touch::TouchCalibrationSettings settings;
        lvgl_lock();
        auto* endIndev = lvgl_pointer_get_default();
        if (endIndev != nullptr && settings::touch::load(settings) && settings.enabled && settings::touch::isValid(settings)) {
            LvglPointerCalibration calibration = {
                .x_min = settings.xMin,
                .x_max = settings.xMax,
                .y_min = settings.yMin,
                .y_max = settings.yMax,
                .rotate_xy = settings.rotateXy,
                .invert_x = settings.invertX,
                .invert_y = settings.invertY,
            };
            lvgl_pointer_set_calibration(endIndev, &calibration);
        }
        lvgl_unlock();
    }

    return ctx.calibrationApplied ? 0 : 2; // Ok : Error
}

} // namespace

uint32_t start(uint32_t callerAppInstanceId) {
    uint32_t instanceId = 0;
    app_manager_start_for_result(manifest.id, callerAppInstanceId, 0, nullptr, &instanceId);
    return instanceId;
}

extern const ::AppManifest manifest = {
    .id = "TouchCalibration",
    .name = "Touch Calibration",
    .category = APP_CATEGORY_SETTINGS,
    .location = { APP_LOCATION_MEMORY, reinterpret_cast<void*>(appMain) }
};

} // namespace tt::app::touchcalibration

#endif // defined(CONFIG_TT_TOUCH_CALIBRATION_SUPPORTED)
