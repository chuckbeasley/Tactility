// SPDX-License-Identifier: Apache-2.0
#pragma once

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * How far a finger may travel between press and release and still count as a click, in pixels.
 *
 * LVGL only suppresses click events when its own scroll machinery took over the gesture
 * (`scroll_obj != NULL`), so a drag that cannot start a scroll - a list that already fits its
 * content, a screen with nothing scrollable under the finger - ends as a CLICKED/SHORT_CLICKED on
 * whatever was pressed. That is how scrolling a settings list opens the app you were dragging over.
 * This is the movement budget that separates the two intents instead.
 *
 * 12 px against LVGL's own 10 px scroll limit: a deliberate drag crosses it in the first frame or
 * two, while a tap that slides a little (fingers do) stays inside it. It cannot be zero - the panel's
 * reported coordinates jitter by a pixel or two even when the finger is still.
 */
#define LVGL_CLICK_GUARD_SLOP_PX 12

/**
 * Whether a gesture that moved from @a press to @a release should be treated as a drag rather than a
 * click. Squared comparison, so no square root and no float.
 */
bool lvgl_click_guard_is_drag(const lv_point_t* press, const lv_point_t* release, int32_t slop_px);

/**
 * Makes @a obj ignore click events that are really drags (see lvgl_click_guard_is_drag).
 *
 * Call once when the object is created. The app's own event handlers are unaffected on a real tap:
 * a suppressed event stops processing before they run, which is the point - it is exactly as if the
 * press had landed on a scrolling list.
 *
 * Only meaningful for pointer (touch) indevs; other kinds are left alone.
 */
void lvgl_click_guard_apply(lv_obj_t* obj);

#ifdef __cplusplus
}
#endif
