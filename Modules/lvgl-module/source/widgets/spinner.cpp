// SPDX-License-Identifier: Apache-2.0
#include <lvgl/widgets/spinner.h>

static void anim_rotation_callback(void* var, int32_t v) {
    auto* object = static_cast<lv_obj_t*>(var);
    auto width = lv_obj_get_width(object);
    auto height = lv_obj_get_height(object);
    lv_obj_set_style_transform_pivot_x(object, width / 2, 0);
    lv_obj_set_style_transform_pivot_y(object, height / 2, 0);
    lv_obj_set_style_transform_rotation(object, v, 0);
}

lv_obj_t* lvgl_spinner_create(lv_obj_t* parent) {
    auto* spinner = lv_label_create(parent);
    lv_label_set_text(spinner, LV_SYMBOL_REFRESH);
    lv_obj_remove_flag(spinner, LV_OBJ_FLAG_CLICKABLE);

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, spinner);
    lv_anim_set_values(&a, 0, 3600);
    lv_anim_set_duration(&a, 800);
    lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
    lv_anim_set_exec_cb(&a, anim_rotation_callback);
    lv_anim_start(&a);

    return spinner;
}
