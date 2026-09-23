// SPDX-License-Identifier: Apache-2.0
#pragma once

#include <lvgl.h>

#include <tactility/device.h>
#include <tactility/error.h>

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Maximum simultaneous touch points lvgl_pointer_add() will create indev \slots for.
 *
 * \slots Each lv_indev_t carries one point per read, so per-finger widget interaction needs
 *     one indev per finger, pooled over a single device read - see pointer.cpp. Distinct from
 *     LVGL's own lv_indev_touch_data_t gesture path, which reports all points to one indev for
 *     gestures rather than per-widget press handling; unused here.
 */
#define LVGL_POINTER_MAX_SLOTS 5

/**
 * @brief Minimum span, in raw units, that a calibration axis must cover to be considered valid.
 *
 * A span narrower than this cannot be a real calibration: it would mean the raw range measured
 * during calibration covered only a few units, and applying it would amplify any noise into a jump
 * across the whole screen.
 */
#define LVGL_POINTER_CALIBRATION_MIN_RANGE 20

/**
 * @brief Linear per-axis calibration range for raw pointer coordinates.
 *
 * Values are the raw (pre-calibration) coordinates that should map to the display's
 * [0, hor_res-1] / [0, ver_res-1] range, with optional rotate/invert flags matching
 * TFT_eSPI Touch_calibrate semantics.
 */
struct LvglPointerCalibration {
    int32_t x_min;
    int32_t x_max;
    int32_t y_min;
    int32_t y_max;
    bool rotate_xy;
    bool invert_x;
    bool invert_y;
};

/**
 * @brief Whether a calibration is usable: both axes must have a positive span of at least
 *        LVGL_POINTER_CALIBRATION_MIN_RANGE raw units.
 *
 * Exported because it is the one rule for what counts as a valid calibration, and the layer that
 * persists a calibration has to agree with the layer that applies it - when the two disagreed, a
 * saved calibration could be rejected at apply time with nothing to show for it.
 */
bool lvgl_pointer_calibration_is_valid(const struct LvglPointerCalibration* calibration);

/**
 * @brief Sets (or clears, when calibration is NULL) the calibration applied to raw coordinates
 * read from the device before they are written into LVGL indev data, on an indev previously
 * created with lvgl_pointer_add(). Applies to every slot of the same physical device (one panel,
 * one calibration) - pass any one of lvgl_pointer_add()'s out_indevs, it doesn't matter which.
 *
 * @warning Caller must hold the LVGL lock (see lvgl_lock() in lvgl_module.h).
 *
 * @param[in] indev an indev previously created by lvgl_pointer_add()
 * @param[in] calibration the calibration range to apply, or NULL to clear/disable calibration
 * @retval ERROR_NONE on success
 * @retval ERROR_INVALID_ARGUMENT if indev is NULL, or calibration is non-NULL but invalid
 *         (see lvgl_pointer_calibration_is_valid())
 */
error_t lvgl_pointer_set_calibration(lv_indev_t* indev, const struct LvglPointerCalibration* calibration);

/**
 * @brief Retrieves the calibration currently active on indev's device, if any.
 * @warning Caller must hold the LVGL lock.
 * @return true when a calibration is currently set on indev (out_calibration is filled), false otherwise
 */
bool lvgl_pointer_get_calibration(lv_indev_t* indev, struct LvglPointerCalibration* out_calibration);

/**
 * @brief Returns the first indev created by lvgl_pointer_add() that hasn't been removed yet
 * (specifically, slot 0 of the first pool created).
 *
 * Unlike iterating LVGL's own indev list, this only ever returns an indev created by
 * lvgl_pointer_add() — safe to pass to lvgl_pointer_set_calibration()/lvgl_pointer_get_calibration()
 * without risking a foreign indev (e.g. one registered by the deprecated HAL layer) whose driver
 * data isn't this module's own context type.
 *
 * @warning Caller must hold the LVGL lock.
 * @return the indev, or NULL if none is currently registered.
 */
lv_indev_t* lvgl_pointer_get_default(void);

/**
 * @brief Creates up to max_touch_points lv_indev_t instances bound to the given POINTER_TYPE
 * device, each independently tracking one simultaneous touch point ("slot"). All slots share one
 * underlying read of the device per LVGL polling round (see pointer.cpp) — this does not multiply
 * bus traffic by max_touch_points.
 *
 * Each returned indev behaves like an ordinary single-touch pointer indev to LVGL: normal widget
 * press/click/drag interaction (LV_EVENT_PRESSED, dragging, etc.) works independently per-indev
 * out of the box. Event callbacks that need to know which finger fired them should use
 * lv_event_get_indev() (never lv_indev_active()/lvgl_pointer_get_default(), which only ever
 * resolve to whichever indev happens to be currently processing) and, if per-finger app state is
 * needed, lvgl_pointer_get_slot_index() to index into it.
 *
 * @warning Caller must hold the LVGL lock (see lvgl_lock() in lvgl_module.h) — call this from
 * LvglModuleConfig.on_start, or after calling lvgl_lock() explicitly.
 * @warning Do not call lv_indev_enable(false) on individual slots. The pool's shared bus read
 * only runs once every slot_count read callbacks (round_pos); disabling one slot skips its
 * callback and desyncs that cadence, staling the remaining active slots' data.
 *
 * @param[in] device a device of type POINTER_TYPE
 * @param[in] display the display this indev should be associated with, or NULL to leave it unset
 * @param[in] max_touch_points how many simultaneous touch slots to create (1 for classic
 *            single-touch behavior; clamped to [1, LVGL_POINTER_MAX_SLOTS])
 * @param[out] out_indevs array of at least max_touch_points entries; filled with the created
 *             indevs on success, valid only when ERROR_NONE is returned
 * @retval ERROR_NONE on success
 * @retval ERROR_INVALID_ARGUMENT if device or out_indevs is NULL, or device is not of type POINTER_TYPE
 * @retval ERROR_OUT_OF_MEMORY if allocation failed
 */
error_t lvgl_pointer_add(struct Device* device, lv_display_t* display, uint8_t max_touch_points, lv_indev_t** out_indevs);

/**
 * @brief Returns which slot index (0-based) of its pool the given indev is, or -1 if indev wasn't
 * created by lvgl_pointer_add() (or is NULL). Useful for indexing small per-finger app state
 * arrays from an event callback's lv_event_get_indev() result.
 * @warning Caller must hold the LVGL lock.
 */
int8_t lvgl_pointer_get_slot_index(lv_indev_t* indev);

/**
 * @brief Removes every slot indev in the same pool as the given indev (i.e. all indevs returned
 * together by one lvgl_pointer_add() call).
 * @warning Caller must hold the LVGL lock.
 */
void lvgl_pointer_remove(lv_indev_t* indev);

#ifdef __cplusplus
}
#endif
