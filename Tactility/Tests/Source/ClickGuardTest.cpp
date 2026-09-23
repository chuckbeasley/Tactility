// SPDX-License-Identifier: Apache-2.0
//
// The click guard's arithmetic, which is the part of it that can be tested without a touch panel.
// The event wiring around it (press/release/click ordering) can only be exercised on hardware: it
// depends on lv_indev_active() reporting an indev, which only happens while LVGL is processing one.
#include <lvgl/widgets/click_guard.h>

#include <doctest.h>

namespace {

constexpr int32_t SLOP = LVGL_CLICK_GUARD_SLOP_PX;

} // namespace

TEST_CASE("click guard treats movement inside the slop as a click") {
    const lv_point_t press = { 100, 100 };
    const lv_point_t still = { 100, 100 };
    const lv_point_t jitter = { 105, 103 };
    const lv_point_t at_limit = { 112, 100 };

    CHECK_FALSE(lvgl_click_guard_is_drag(&press, &still, SLOP));
    CHECK_FALSE(lvgl_click_guard_is_drag(&press, &jitter, SLOP));
    // Exactly at the limit: 12 px away is still a click, since the comparison is "greater than".
    CHECK_FALSE(lvgl_click_guard_is_drag(&press, &at_limit, SLOP));
}

TEST_CASE("click guard treats movement beyond the slop as a drag") {
    const lv_point_t press = { 100, 100 };
    const lv_point_t past = { 113, 100 };
    const lv_point_t up = { 100, 88 };
    const lv_point_t diagonal = { 109, 109 };
    const lv_point_t flick = { 300, 460 };

    CHECK(lvgl_click_guard_is_drag(&press, &past, SLOP));
    CHECK(lvgl_click_guard_is_drag(&press, &up, SLOP));
    // Diagonal: 9 px each way is 12.7 px of travel, so past the limit even though neither axis is.
    CHECK(lvgl_click_guard_is_drag(&press, &diagonal, SLOP));
    // A whole-screen drag, e.g. a flick that the multi-slot pointer pool lost and re-acquired
    // mid-gesture - the case that used to arrive as a click on whatever was under the finger.
    CHECK(lvgl_click_guard_is_drag(&press, &flick, SLOP));
}

TEST_CASE("click guard ignores a missing point") {
    const lv_point_t point = { 10, 10 };
    CHECK_FALSE(lvgl_click_guard_is_drag(nullptr, &point, SLOP));
    CHECK_FALSE(lvgl_click_guard_is_drag(&point, nullptr, SLOP));
    CHECK_FALSE(lvgl_click_guard_is_drag(nullptr, nullptr, SLOP));
}
