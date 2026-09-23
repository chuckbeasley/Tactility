#pragma once

#include <cstdint>

#include <lvgl/devices/pointer.h>

namespace tt::settings::touch {

/**
 * @brief Persisted touch calibration coefficients.
 *
 * Shape mirrors struct LvglPointerCalibration (lvgl/devices/pointer.h): xMin/xMax/yMin/yMax are
 * the raw touch coordinate range, and rotateXy/invertX/invertY mirror TFT_eSPI Touch_calibrate
 * flags. This struct only concerns itself with persistence - applying it to a live pointer indev
 * is the caller's responsibility (see toPointerCalibration() and lvgl_pointer_set_calibration()).
 */
struct TouchCalibrationSettings {
    bool enabled = false;
    int32_t schemaVersion = 0;
    int32_t xMin = 0;
    int32_t xMax = 0;
    int32_t yMin = 0;
    int32_t yMax = 0;
    bool rotateXy = false;
    bool invertX = false;
    bool invertY = false;
};

TouchCalibrationSettings getDefault();

bool isValid(const TouchCalibrationSettings& settings);

/**
 * @brief Converts persisted settings into the calibration the pointer layer applies.
 *
 * The single conversion from one representation to the other: it used to be written out at every
 * call site (the app's preview and its apply-on-start, and Tactility's apply-on-boot), which is
 * three chances for the field order or the flag sense to drift apart.
 *
 * @return a calibration equivalent to @a settings; check validity with lvgl_pointer_calibration_is_valid()
 */
LvglPointerCalibration toPointerCalibration(const TouchCalibrationSettings& settings);

bool load(TouchCalibrationSettings& settings);

TouchCalibrationSettings loadOrGetDefault();

bool shouldRunCalibration();

bool save(const TouchCalibrationSettings& settings);

} // namespace tt::settings::touch
