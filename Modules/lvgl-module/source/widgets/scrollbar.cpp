// SPDX-License-Identifier: Apache-2.0
#include <lvgl/widgets/scrollbar.h>

extern "C" {

void lvgl_apply_readable_scrollbar(lv_obj_t* obj) {
    if (obj == nullptr) {
        return;
    }

    lv_obj_set_style_width(obj, LVGL_SCROLLBAR_WIDTH_PX, LV_PART_SCROLLBAR);
    lv_obj_set_style_radius(obj, LV_RADIUS_CIRCLE, LV_PART_SCROLLBAR);
    // Themed at 40% opacity, which disappears against a dark screen.
    lv_obj_set_style_bg_opa(obj, LV_OPA_70, LV_PART_SCROLLBAR);
    lv_obj_set_style_bg_color(obj, lv_palette_main(LV_PALETTE_GREY), LV_PART_SCROLLBAR);
    // The theme insets the bar 7 px from the object's edge and the bar is drawn inward from there, so
    // a wide bar would reach far enough across the content to sit under right-aligned text. 3 px
    // keeps a 15 px band at the edge instead of a 21 px one.
    lv_obj_set_style_pad_right(obj, 3, LV_PART_SCROLLBAR);
    lv_obj_set_style_pad_left(obj, 3, LV_PART_SCROLLBAR);

    // LV_SCROLLBAR_MODE_AUTO is the one that means "show it while there is anything to scroll": in
    // this LVGL, ACTIVE means "only while a scroll gesture is in progress" and ON means "always, even
    // with nothing to scroll". Getting that wrong is why the first attempt drew nothing.
    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_AUTO);
}

} // extern "C"
