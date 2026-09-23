// SPDX-License-Identifier: Apache-2.0
#include <lvgl/widgets/click_guard.h>

#include <cstdlib>

namespace {

/** Per-object state: where the finger went down for the press being tracked. */
struct ClickGuardState {
    lv_point_t press_point;
    bool pressed;
};

/**
 * Whether this event is part of the click family that a drag must not produce.
 *
 * LVGL sends RELEASED, then SHORT_CLICKED, then (via the short-click handler) SINGLE_CLICKED and its
 * double/triple variants, then CLICKED - each as its own processing chain, so suppressing one does
 * not stop the rest. Every one of them has to be filtered, and the decision is made per event rather
 * than once on release.
 */
bool is_click_event(lv_event_code_t code) {
    return code == LV_EVENT_SHORT_CLICKED || code == LV_EVENT_CLICKED ||
        code == LV_EVENT_SINGLE_CLICKED || code == LV_EVENT_DOUBLE_CLICKED ||
        code == LV_EVENT_TRIPLE_CLICKED;
}

void on_event(lv_event_t* event) {
    const lv_event_code_t code = lv_event_get_code(event);

    if (code == LV_EVENT_DELETE) {
        free(lv_event_get_user_data(event));
        return;
    }

    auto* state = static_cast<ClickGuardState*>(lv_event_get_user_data(event));
    if (state == nullptr) {
        return;
    }

    if (code == LV_EVENT_PRESSED) {
        // Only touch gestures are guarded: a keyboard or encoder "press" has no meaningful travel,
        // and an external pointer (a mouse) legitimately drags with its button held.
        lv_indev_t* indev = lv_indev_active();
        if (indev == nullptr || lv_indev_get_type(indev) != LV_INDEV_TYPE_POINTER) {
            state->pressed = false;
            return;
        }
        lv_indev_get_point(indev, &state->press_point);
        state->pressed = true;
        return;
    }

    if (code == LV_EVENT_PRESS_LOST) {
        state->pressed = false;
        return;
    }

    if (!is_click_event(code)) {
        return;
    }

    if (!state->pressed) {
        return;
    }

    lv_indev_t* indev = lv_indev_active();
    if (indev == nullptr || lv_indev_get_type(indev) != LV_INDEV_TYPE_POINTER) {
        return;
    }

    lv_point_t point = {};
    lv_indev_get_point(indev, &point);
    if (lvgl_click_guard_is_drag(&state->press_point, &point, LVGL_CLICK_GUARD_SLOP_PX)) {
        // Swallow it, so neither this object's own handlers nor its parents' run: as far as the app
        // is concerned the press landed on something that was being scrolled.
        lv_event_stop_processing(event);
    } else if (code == LV_EVENT_CLICKED) {
        // Last of the family for this gesture, and the point where a stale press must not survive.
        state->pressed = false;
    }
}

} // namespace

extern "C" {

bool lvgl_click_guard_is_drag(const lv_point_t* press, const lv_point_t* release, int32_t slop_px) {
    if (press == nullptr || release == nullptr) {
        return false;
    }
    const int32_t dx = (int32_t)release->x - (int32_t)press->x;
    const int32_t dy = (int32_t)release->y - (int32_t)press->y;
    return (dx * dx + dy * dy) > (slop_px * slop_px);
}

void lvgl_click_guard_apply(lv_obj_t* obj) {
    if (obj == nullptr) {
        return;
    }

    auto* state = static_cast<ClickGuardState*>(malloc(sizeof(ClickGuardState)));
    if (state == nullptr) {
        return;
    }
    state->press_point = { 0, 0 };
    state->pressed = false;

    // Registered at creation, i.e. before the app adds its own handlers, so this one runs first in
    // every click chain and can stop the rest of it. LV_EVENT_ALL rather than a mask because the
    // filter argument takes one code, not a set.
    lv_obj_add_event_cb(obj, on_event, LV_EVENT_ALL, state);
}

} // extern "C"
