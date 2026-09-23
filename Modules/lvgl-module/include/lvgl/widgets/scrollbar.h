// SPDX-License-Identifier: Apache-2.0
#pragma once

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Width of the scrollbar, in pixels, on every scrollable object the UI creates.
 *
 * LVGL's default theme draws it 5 px wide at 40% opacity, which on this 320 px panel is a hairline:
 * too thin to aim at, too faint to notice, and impossible to use as a scroll target with a fingertip.
 * Widening it is not only cosmetic - the bar is a draggable part of the object, so a usable bar means
 * scrolling can be done by dragging the bar instead of dragging over the rows, which is the gesture
 * that turns into an accidental click when nothing under the finger can scroll.
 *
 * 24 px is about 3.8 mm on this 3.5" panel (roughly 6.4 px/mm), which is a fingertip target, and it
 * applies to every scroller the UI builds: containers, lists, dropdowns (including the list a
 * dropdown pops up) and text areas. See lvgl_apply_readable_scrollbar() for why it is applied from
 * the widget wrappers rather than by patching LVGL's theme.
 */
#define LVGL_SCROLLBAR_WIDTH_PX 24

/**
 * Applies the readable scrollbar styling above to @a obj, and shows the bar while the object has
 * anything to scroll (LV_SCROLLBAR_MODE_AUTO - in this LVGL, ACTIVE means "only during a scroll
 * gesture" and ON means "always, even with nothing to scroll").
 *
 * Harmless on objects that cannot scroll: the bar is simply never drawn.
 */
void lvgl_apply_readable_scrollbar(lv_obj_t* obj);

#ifdef __cplusplus
}
#endif
