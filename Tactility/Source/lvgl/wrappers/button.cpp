#ifdef ESP_PLATFORM

#include <lvgl.h>

#include <lvgl/lvgl.h>

extern "C" {

extern lv_obj_t* __real_lv_button_create(lv_obj_t* parent);

lv_obj_t* __wrap_lv_button_create(lv_obj_t* parent) {
    auto button = __real_lv_button_create(parent);

    if (lvgl_get_ui_density() == LVGL_UI_DENSITY_COMPACT) {
        lv_obj_set_style_pad_all(button, 2, LV_STATE_DEFAULT);
        lv_obj_set_style_radius(button, 3, LV_STATE_DEFAULT);
        lv_obj_set_height(button, 18);
    } else {
        // Match the compact settings-switch row height (see switch.cpp / the Wi-Fi & Bluetooth
        // on-off toggles) so buttons read as compact pills rather than tall blocks. Full-width
        // menu buttons set their own taller height and are unaffected.
        lv_obj_set_style_pad_ver(button, 4, LV_STATE_DEFAULT);
        lv_obj_set_height(button, 22);
    }

    return button;
}

}

#endif // ESP_PLATFORM