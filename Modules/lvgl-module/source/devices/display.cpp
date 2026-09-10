// SPDX-License-Identifier: Apache-2.0
#include <lvgl/devices/display.h>

#include <lvgl/ppa.h>

#include <tactility/device.h>
#include <tactility/driver.h>
#include <tactility/drivers/display.h>
#include <tactility/log.h>

#include <lvgl/devices/device_context.h>

#include <stdlib.h>
#include <atomic>
#include <cstring>

#ifdef ESP_PLATFORM
#include <esp_heap_caps.h>
#include <esp_timer.h>
#endif

constexpr auto* TAG = "lvgl_display";

// Microsecond monotonic clock for the dirty rectangle's timestamp. LVGL's own tick is only
// millisecond resolution, which is too coarse to order a capture against a change that may have
// happened in the same millisecond.
static int64_t lvgl_now_us() {
#ifdef ESP_PLATFORM
    return esp_timer_get_time();
#else
    return (int64_t)lv_tick_get() * 1000;
#endif
}

struct LvglDisplayCtx {
    void* buf1;
    void* buf2;
    bool owns_buffers; // false when buf1/buf2 point at the device's own frame buffer(s)
    // Mirrors what lvgl_display_add() passed to lv_display_set_render_mode() - there's no
    // lv_display_get_render_mode() to query it back from LVGL, so it's cached here instead.
    lv_display_render_mode_t render_mode;
    // The device's swap_xy/mirror_x/mirror_y at bind time, queried once and treated as the LV_DISPLAY_ROTATION_0 baseline.
    bool base_swap_xy;
    bool base_mirror_x;
    bool base_mirror_y;
    // The device's configured gap at bind time, in the same LV_DISPLAY_ROTATION_0 baseline frame
    // as base_swap_xy above. set_gap() is a raw (x,y) offset applied to whatever coordinates the
    // panel is currently being drawn with - it has no idea about swap_xy, so a rotation that flips
    // swap_xy relative to this baseline must swap gap_x/gap_y too (see lvgl_display_apply_rotation()).
    int32_t base_gap_x;
    int32_t base_gap_y;
    bool has_set_gap_cap;
    // When true, rotation is done in software in the flush callback instead of via display_swap_xy()/
    // display_mirror(); rotate_buf holds the rotated pixels and is sized like buf1.
    bool sw_rotate;
    void* rotate_buf;
    // Lazily created on the first sw_rotate flush that needs it (see lvgl_display_rotate_tile()).
    // Stays NULL - and every rotate falls back to rotate_buf/lv_draw_sw_rotate() - when the target
    // has no PPA (lvgl_ppa_is_supported()), the color format has no PPA color mode
    // (lvgl_ppa_supports_color_format()), or creating the PPA client/buffer failed once already.
    void* ppa_handle;
    bool ppa_unavailable;
    bool ppa_eligible;
    // Size of buf1/buf2 (each) - used by lvgl_display_fb_base() to range-check which real buffer
    // (for the fb-direct case) a given color_map pointer falls into.
    size_t buf_size_bytes;
    // Cached DISPLAY_CAPABILITY_CAP_SWAP_XY/CAP_MIRROR: swap_xy()/mirror() and their getters are
    // null on drivers that don't support them, so rotation handling must not call through blindly.
    bool has_swap_xy_cap;
    bool has_mirror_cap;
    // Mirrors LvglDisplayConfig::swap_bytes: the panel is big endian while the OS is little endian,
    // so we fix it in software. In the future, the driver should probably expose endianness requirements instead.
    bool byte_swap;
    // Shadow frame: a complete copy of the logical screen, kept up to date by copying each region
    // LVGL flushes into it (see lvgl_display_shadow_update()). It exists so a consumer - the
    // remote-screen mirror - can read a whole frame cheaply instead of re-rendering the entire
    // widget tree with lv_snapshot_take() (~350 ms at 480x320).
    //
    // The alternative, putting the display into full-frame render mode, also makes the buffer a
    // complete frame, but it costs the UI a full redraw on every update: the JPEG encode alone
    // nearly doubled (128 ms -> 247 ms) even on a completely static screen. Keeping the shadow
    // instead leaves the display in partial mode: a static screen copies nothing, and a small
    // update copies only its own rectangle.
    //
    // Allocated lazily on the first RGB565 flush. shadow_rows marks which rows have been written at
    // least once, so a consumer can tell whether the frame is complete yet.
    uint8_t* shadow_frame;
    size_t shadow_stride;
    uint8_t* shadow_rows;
    // Set once every row has been written at least once. atomic so a consumer can test it without
    // taking the LVGL lock.
    std::atomic<bool> shadow_complete {false};
    // Bumped twice around every write into the shadow pixels. A consumer copies those pixels without
    // any lock, so this is how it finds out that its copy overlapped a flush and may be torn - and
    // can simply copy again. It makes no attempt to serialise the two: an occasional torn frame that
    // the next one corrects is a far better trade than a mirror that waits for the UI to stop
    // rendering (measured: up to 754 ms waiting for the LVGL lock while the screen was busy).
    std::atomic<uint32_t> shadow_seq {0};
    // Guards the dirty rectangle and the completeness flag below, which must not lose an update.
    // Held for a handful of instructions, never across a pixel copy.
    std::atomic_flag dirty_lock = ATOMIC_FLAG_INIT;
    // Union of the regions changed since a consumer last took it (lvgl_display_take_dirty_area).
    // This is what lets the mirror send only what moved instead of a whole frame: for a typical UI
    // update (a keystroke, a clock tick, a highlight) the rectangle is tiny.
    bool dirty_empty;
    int32_t dirty_x1;
    int32_t dirty_y1;
    int32_t dirty_x2;
    int32_t dirty_y2;
    // When the rectangle was last widened. A consumer that serves a frame captured *before* this
    // must not consume the rectangle, or a change made after that capture would be reported to
    // nobody. See lvgl_display_peek_dirty_area().
    int64_t dirty_last_update_us;
};

// A spinlock, not a mutex, because the sections it guards are a few comparisons: the cost of a
// syscall or a scheduler round trip would dwarf the work. Only these two functions use it.
static void lvgl_shadow_lock(struct LvglDisplayCtx* ctx) {
    while (ctx->dirty_lock.test_and_set(std::memory_order_acquire)) {
        // Spin. The holder is the LVGL task doing a few compares, and it cannot be preempted
        // inside the section on the single-core targets this runs on.
    }
}

static void lvgl_shadow_unlock(struct LvglDisplayCtx* ctx) {
    ctx->dirty_lock.clear(std::memory_order_release);
}

static void* lvgl_display_alloc_buffer(size_t size_bytes, bool prefer_external_ram) {
#ifdef ESP_PLATFORM
    // Must match LV_DRAW_BUF_ALIGN (can be > 4 - e.g. 64, tied to the cache line size for
    // DMA2D/PPA coherency on some targets - see sdkconfig's CONFIG_LV_DRAW_BUF_ALIGN). A buffer
    // allocated less strictly than that fails lv_display_set_buffers()'s alignment assert, which
    // is configured to LV_ASSERT_HANDLER (while(1);) rather than a clean abort - i.e. a silent hang.
    // MALLOC_CAP_DMA is scarce internal RAM - skip it for displays that don't DMA directly from
    // this buffer (see prefer_external_ram_buffer). Dropping MALLOC_CAP_DMA alone isn't enough to
    // land in PSRAM though: MALLOC_CAP_8BIT alone is still satisfied by internal RAM, so
    // MALLOC_CAP_SPIRAM must be requested explicitly (confirmed on real hardware).
    uint32_t caps = prefer_external_ram ? (MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT) : (MALLOC_CAP_DMA | MALLOC_CAP_8BIT);
    void* buf = heap_caps_aligned_alloc(LV_DRAW_BUF_ALIGN, size_bytes, caps);
    if (buf == NULL) {
        buf = heap_caps_aligned_alloc(LV_DRAW_BUF_ALIGN, size_bytes, MALLOC_CAP_DEFAULT);
    }
    return buf;
#else
    return malloc(size_bytes);
#endif
}

static void lvgl_display_free_buffer(void* buf) {
#ifdef ESP_PLATFORM
    heap_caps_free(buf);
#else
    free(buf);
#endif
}

// Resolves the kernel-reported color format to an LVGL color format. RGB565 and BGR565 (and
// their _SWAPPED variants) all render identically as far as LVGL is concerned - it has no native
// concept of channel order, only byte order (see LvglDisplayConfig::swap_bytes for that axis).
static bool lvgl_display_map_color_format(enum DisplayColorFormat in, lv_color_format_t* out) {
    switch (in) {
        case DISPLAY_COLOR_FORMAT_RGB565:
        case DISPLAY_COLOR_FORMAT_RGB565_SWAPPED:
        case DISPLAY_COLOR_FORMAT_BGR565:
        case DISPLAY_COLOR_FORMAT_BGR565_SWAPPED:
            *out = LV_COLOR_FORMAT_RGB565;
            return true;
        case DISPLAY_COLOR_FORMAT_RGB888:
            *out = LV_COLOR_FORMAT_RGB888;
            return true;
        case DISPLAY_COLOR_FORMAT_MONOCHROME:
            // Row-major, MSB-first 1bpp (matches LV_COLOR_FORMAT_I1's raw layout once the
            // palette header is stripped, see lvgl_display_flush_cb()) - any page/tile
            // reformatting a specific panel's GDDRAM needs is that driver's own concern
            // (e.g. ssd1306_draw_bitmap()'s row-to-page transpose).
            *out = LV_COLOR_FORMAT_I1;
            return true;
        case DISPLAY_COLOR_FORMAT_GRAYSCALE8:
            // Row-major, 1 byte/pixel luminance (0x00=black, 0xFF=white) - matches LV_COLOR_FORMAT_L8
            // directly, no repacking needed. Deliberately NOT routed through the I1 branch below in
            // lvgl_display_add(): I1 is hardcoded to LV_DISPLAY_RENDER_MODE_FULL there, which is what
            // this format exists to avoid for panels that want real partial/tile updates.
            *out = LV_COLOR_FORMAT_L8;
            return true;
        default:
            return false;
    }
}

static void lvgl_display_apply_rotation(struct LvglDeviceContext* wrapper, lv_display_rotation_t rotation) {
    struct LvglDisplayCtx* ctx = (struct LvglDisplayCtx*)wrapper->context;

    // SW-rotated displays stay in their base orientation; rotation is applied per-flush instead.
    if (ctx->sw_rotate) {
        return;
    }

    bool swap_xy = ctx->base_swap_xy;
    bool mirror_x = ctx->base_mirror_x;
    bool mirror_y = ctx->base_mirror_y;

    switch (rotation) {
        case LV_DISPLAY_ROTATION_0:
            break;
        case LV_DISPLAY_ROTATION_90:
            swap_xy = !ctx->base_swap_xy;
            if (ctx->base_swap_xy) {
                mirror_x = !ctx->base_mirror_x;
            } else {
                mirror_y = !ctx->base_mirror_y;
            }
            break;
        case LV_DISPLAY_ROTATION_180:
            mirror_x = !ctx->base_mirror_x;
            mirror_y = !ctx->base_mirror_y;
            break;
        case LV_DISPLAY_ROTATION_270:
            swap_xy = !ctx->base_swap_xy;
            if (ctx->base_swap_xy) {
                mirror_y = !ctx->base_mirror_y;
            } else {
                mirror_x = !ctx->base_mirror_x;
            }
            break;
    }

    if (ctx->has_swap_xy_cap) {
        display_swap_xy(wrapper->device, swap_xy);
    }
    if (ctx->has_mirror_cap) {
        display_mirror(wrapper->device, mirror_x, mirror_y);
    }
    if (ctx->has_set_gap_cap) {
        // set_gap() takes its (x,y) in whatever axes the panel is currently drawn with, not the
        // baseline's - swap gap_x/gap_y whenever this rotation's swap_xy differs from the baseline.
        bool gap_axes_swapped = swap_xy != ctx->base_swap_xy;
        int32_t gap_x = gap_axes_swapped ? ctx->base_gap_y : ctx->base_gap_x;
        int32_t gap_y = gap_axes_swapped ? ctx->base_gap_x : ctx->base_gap_y;
        display_set_gap(wrapper->device, gap_x, gap_y);
    }
}

static void lvgl_display_rotation_event_cb(lv_event_t* e) {
    struct LvglDeviceContext* wrapper = (struct LvglDeviceContext*)lv_event_get_user_data(e);
    lv_display_t* disp = (lv_display_t*)lv_event_get_current_target(e);
    lvgl_display_apply_rotation(wrapper, lv_display_get_rotation(disp));
}

// Returns which of buf1/buf2 (the real frame buffers, when !owns_buffers) color_map falls inside.
// Defaults to buf1, which also covers the single-frame-buffer (buf2 == NULL) case.
static void* lvgl_display_fb_base(struct LvglDisplayCtx* ctx, const uint8_t* color_map) {
    if (ctx->buf2 != NULL && color_map >= (uint8_t*)ctx->buf2 &&
        color_map < (uint8_t*)ctx->buf2 + ctx->buf_size_bytes) {
        return ctx->buf2;
    }
    return ctx->buf1;
}

// Tries to rotate the tightly-packed w x h block at in_buff via PPA, returning the PPA output
// buffer on success or NULL if this tile/format/target can't use it - in which case the caller
// must fall back to lv_draw_sw_rotate() into ctx->rotate_buf. Lazily creates the PPA client on the
// first eligible call, sized to ctx->buf_size_bytes (the largest tile or full-frame buffer this
// display will ever flush - see lvgl_display_add()); once creation fails once, ppa_unavailable
// latches so later tiles don't retry it. Never asks PPA to byte-swap: lvgl_display_flush_cb()
// applies ctx->byte_swap itself, once, at a single point regardless of which rotation path ran -
// simpler than tracking whether it was already done by PPA (PARTIAL mode) vs. already done
// per-tile before FULL mode's whole-frame rotate (see the two call sites).
static void* lvgl_display_try_ppa_rotate(struct LvglDisplayCtx* ctx, const uint8_t* in_buff, int32_t w, int32_t h,
                                          lv_display_rotation_t rotation, lv_color_format_t color_format) {
    if (!ctx->ppa_eligible || ctx->ppa_unavailable) {
        return NULL;
    }
    // PPA reads pic_w/pic_h in pixels with no separate stride - only safe when the tile has no
    // row padding beyond w * bytes-per-pixel (see lvgl_ppa.h).
    uint32_t bpp = lv_color_format_get_size(color_format);
    if (lv_draw_buf_width_to_stride(w, color_format) != (uint32_t)w * bpp) {
        return NULL;
    }
    if (ctx->ppa_handle == NULL) {
        ctx->ppa_handle = lvgl_ppa_get_or_create(ctx->buf_size_bytes);
        if (ctx->ppa_handle == NULL) {
            ctx->ppa_unavailable = true;
            return NULL;
        }
    }
    return lvgl_ppa_rotate(ctx->ppa_handle, in_buff, w, h, rotation, color_format, false);
}

// Copies a freshly rendered region into the shadow frame (see LvglDisplayCtx::shadow_frame).
// Called from the flush callback, so it runs on the LVGL task with the LVGL lock held, and works in
// logical screen coordinates - before the rotation/byte-swap handling below rewrites them.
static void lvgl_display_shadow_update(struct LvglDisplayCtx* ctx, lv_display_t* disp, const lv_area_t* area, const uint8_t* color_map) {
    if (lv_display_get_color_format(disp) != LV_COLOR_FORMAT_RGB565) {
        return; // Only RGB565 has a layout that can be copied straight through.
    }

    const uint32_t hres = (uint32_t)lv_display_get_horizontal_resolution(disp);
    const uint32_t vres = (uint32_t)lv_display_get_vertical_resolution(disp);
    if (hres == 0 || vres == 0) {
        return;
    }

    const int32_t x1 = area->x1;
    const int32_t y1 = area->y1;
    const int32_t x2 = area->x2;
    const int32_t y2 = area->y2;
    if (x1 < 0 || y1 < 0 || x2 < x1 || y2 < y1 || (uint32_t)x2 >= hres || (uint32_t)y2 >= vres) {
        return;
    }

    const size_t row_bitmap_bytes = ((size_t)vres + 7) / 8;

    if (ctx->shadow_frame == NULL) {
        ctx->shadow_frame = (uint8_t*)lvgl_display_alloc_buffer((size_t)hres * vres * 2, true);
        ctx->shadow_rows = (uint8_t*)lvgl_display_alloc_buffer(row_bitmap_bytes, true);
        if (ctx->shadow_frame == NULL || ctx->shadow_rows == NULL) {
            if (ctx->shadow_frame != NULL) { lvgl_display_free_buffer(ctx->shadow_frame); ctx->shadow_frame = NULL; }
            if (ctx->shadow_rows != NULL) { lvgl_display_free_buffer(ctx->shadow_rows); ctx->shadow_rows = NULL; }
            return;
        }
        memset(ctx->shadow_rows, 0, row_bitmap_bytes);
        ctx->shadow_stride = (size_t)hres * 2;
        ctx->shadow_complete = false;
        ctx->dirty_empty = true;
    }

    // The area's pixels are tightly packed, with a stride derived from the area's width.
    const size_t row_bytes = (size_t)(x2 - x1 + 1) * 2;
    const uint32_t src_stride = lv_draw_buf_width_to_stride(x2 - x1 + 1, LV_COLOR_FORMAT_RGB565);
    // Bracket the copy with the sequence counter, so a reader copying at the same time can see that
    // its copy overlapped a write and is not trustworthy. The copy itself is deliberately not under
    // any lock: it is the long part (tens of ms for a large region), and holding anything across it
    // is exactly what used to make the mirror wait for the UI to finish rendering.
    ctx->shadow_seq.fetch_add(1, std::memory_order_acq_rel);
    for (int32_t y = y1; y <= y2; ++y) {
        memcpy(ctx->shadow_frame + (size_t)y * ctx->shadow_stride + (size_t)x1 * 2,
               color_map + (size_t)(y - y1) * src_stride,
               row_bytes);
        ctx->shadow_rows[(size_t)y >> 3] |= (uint8_t)(1u << ((size_t)y & 7u));
    }
    ctx->shadow_seq.fetch_add(1, std::memory_order_acq_rel);

    // The dirty rectangle and the completeness flag are bookkeeping rather than pixels, and losing
    // an update to a race would mean a change the mirror never hears about - it would show a stale
    // patch until something else changed. They are therefore updated inside a spinlock, but a very
    // short one: this is a handful of comparisons, nothing like the LVGL lock.
    lvgl_shadow_lock(ctx);

    // Stamp the widening, so a consumer that serves a frame captured before this moment knows the
    // rectangle is newer than its frame.
    ctx->dirty_last_update_us = lvgl_now_us();

    // Widen the dirty rectangle to cover this region.
    if (ctx->dirty_empty) {
        ctx->dirty_x1 = x1;
        ctx->dirty_y1 = y1;
        ctx->dirty_x2 = x2;
        ctx->dirty_y2 = y2;
        ctx->dirty_empty = false;
    } else {
        if (x1 < ctx->dirty_x1) ctx->dirty_x1 = x1;
        if (y1 < ctx->dirty_y1) ctx->dirty_y1 = y1;
        if (x2 > ctx->dirty_x2) ctx->dirty_x2 = x2;
        if (y2 > ctx->dirty_y2) ctx->dirty_y2 = y2;
    }

    // Once every row has been written at least once the frame is complete - which is the case after
    // the first full refresh, since a fresh screen invalidates everything.
    if (!ctx->shadow_complete) {
        const uint8_t last_mask = (vres % 8 == 0) ? 0xFF : (uint8_t)((1u << (vres % 8)) - 1u);
        bool all = true;
        for (size_t i = 0; i < row_bitmap_bytes; ++i) {
            const uint8_t mask = (i + 1 == row_bitmap_bytes) ? last_mask : 0xFF;
            if ((ctx->shadow_rows[i] & mask) != mask) {
                all = false;
                break;
            }
        }
        ctx->shadow_complete = all;
    }

    lvgl_shadow_unlock(ctx);
}

static void lvgl_display_flush_cb(lv_display_t* disp, const lv_area_t* area, uint8_t* color_map) {
    struct LvglDeviceContext* wrapper = (struct LvglDeviceContext*)lv_display_get_driver_data(disp);
    struct LvglDisplayCtx* ctx = (struct LvglDisplayCtx*)wrapper->context;
    bool is_i1 = lv_display_get_color_format(disp) == LV_COLOR_FORMAT_I1;

    // Keep the shadow frame current, from the logical (unrotated, unswapped) pixels that LVGL just
    // rendered for this region.
    lvgl_display_shadow_update(ctx, disp, area, color_map);

    int32_t x1 = area->x1;
    int32_t y1 = area->y1;
    int32_t x2 = area->x2;
    int32_t y2 = area->y2;
    uint32_t area_size_px = (uint32_t)(x2 - x1 + 1) * (uint32_t)(y2 - y1 + 1);

    lv_display_rotation_t rotation = lv_display_get_rotation(disp);
    bool rotating = ctx->sw_rotate && rotation != LV_DISPLAY_ROTATION_0;

    // In FULL mode, a refresh cycle can call this once per still-unjoined invalidated area (see
    // the comment below) before the frame is complete - rotating per-tile here would only ever
    // reflect the last tile written, not the accumulated whole frame. Rotate the whole buffer in
    // one shot instead, right before presenting (see the FULL-mode branch below).
    if (rotating && ctx->render_mode != LV_DISPLAY_RENDER_MODE_FULL) {
        // sw_rotate is only ever requested for displays lacking real HW mirror/swap_xy capability
        // (see lvgl_devices.c), and lvgl_display_add() only binds fb-direct (owns_buffers == false)
        // when that capability is present - so this is always the owns_buffers == true case.
        lv_color_format_t color_format = lv_display_get_color_format(disp);
        int32_t w = x2 - x1 + 1;
        int32_t h = y2 - y1 + 1;

        void* ppa_out = lvgl_display_try_ppa_rotate(ctx, color_map, w, h, rotation, color_format);
        if (ppa_out != NULL) {
            color_map = (uint8_t*)ppa_out;
        } else {
            uint32_t w_stride = lv_draw_buf_width_to_stride(w, color_format);
            uint32_t h_stride = lv_draw_buf_width_to_stride(h, color_format);
            if (rotation == LV_DISPLAY_ROTATION_180) {
                lv_draw_sw_rotate(color_map, ctx->rotate_buf, w, h, w_stride, w_stride, rotation, color_format);
            } else {
                lv_draw_sw_rotate(color_map, ctx->rotate_buf, w, h, w_stride, h_stride, rotation, color_format);
            }
            color_map = (uint8_t*)ctx->rotate_buf;
        }
        lv_area_t rotated_area = { x1, y1, x2, y2 };
        lv_display_rotate_area(disp, &rotated_area);
        x1 = rotated_area.x1;
        y1 = rotated_area.y1;
        x2 = rotated_area.x2;
        y2 = rotated_area.y2;
    }

    if (ctx->byte_swap) {
        lv_draw_sw_rgb565_swap(color_map, area_size_px);
    }

    if (ctx->render_mode == LV_DISPLAY_RENDER_MODE_FULL) {
        // FULL mode always redraws (and flushes) the whole display, but a refresh cycle can still
        // call this flush_cb once per still-unjoined invalidated area (lv_refr.c's
        // refr_invalid_areas()), each writing its own tile into the *same* shared buffer - only
        // the last call actually holds the complete frame. This applies equally whether that
        // buffer is one we own (owns_buffers, e.g. an I1 e-paper/OLED panel) or a real hardware
        // frame buffer (fb-direct, where display_draw_bitmap() - see rgb_display_draw_bitmap() -
        // additionally blocks for a full scan-out period whenever frame_buffer_count > 0).
        // Presenting on every call would send partially-rendered frames, and for fb-direct would
        // also pay that scan-out wait N times per refresh instead of once; defer to the last
        // flush and present the whole buffer in one call, mirroring esp_lvgl_port_disp.c's own
        // lv_disp_flush_is_last() gate for its direct/full render mode.
        if (lv_display_flush_is_last(disp)) {
            uint8_t* fb_base;
            if (ctx->owns_buffers) {
                fb_base = (uint8_t*)ctx->buf1;
                if (is_i1) {
                    // LVGL reserves an 8-byte palette (2 x lv_color32_t) at the front of every I1
                    // draw buffer; it's on the caller to skip it before treating the rest as
                    // pixel data.
                    fb_base += 8;
                }
            } else {
                fb_base = (uint8_t*)lvgl_display_fb_base(ctx, color_map);
            }
            uint16_t hres = display_get_resolution_x(wrapper->device);
            uint16_t vres = display_get_resolution_y(wrapper->device);

            if (rotating) {
                // fb_base now holds the whole completed frame, but still in LVGL's *logical*
                // (rotated) w/h - rotate it as one block into rotate_buf, matching the panel's
                // fixed physical w/h, before presenting.
                lv_color_format_t color_format = lv_display_get_color_format(disp);
                bool swapped_wh = rotation == LV_DISPLAY_ROTATION_90 || rotation == LV_DISPLAY_ROTATION_270;
                int32_t logical_w = swapped_wh ? (int32_t)vres : (int32_t)hres;
                int32_t logical_h = swapped_wh ? (int32_t)hres : (int32_t)vres;

                void* ppa_out = lvgl_display_try_ppa_rotate(ctx, fb_base, logical_w, logical_h, rotation, color_format);
                if (ppa_out != NULL) {
                    fb_base = (uint8_t*)ppa_out;
                } else {
                    uint32_t src_stride = lv_draw_buf_width_to_stride(logical_w, color_format);
                    uint32_t dest_stride = lv_draw_buf_width_to_stride(hres, color_format);
                    lv_draw_sw_rotate(fb_base, ctx->rotate_buf, logical_w, logical_h, src_stride, dest_stride, rotation, color_format);
                    fb_base = (uint8_t*)ctx->rotate_buf;
                }
            }

            display_draw_bitmap(wrapper->device, 0, 0, hres, vres, fb_base);
        }
    } else if (ctx->owns_buffers) {
        // PARTIAL mode: each flush_cb call is one independent, complete tile into a buffer that
        // gets reused for the next tile, so present it immediately rather than waiting.
        // LVGL's area is inclusive; DisplayApi's draw_bitmap wants an exclusive end.
        display_draw_bitmap(wrapper->device, x1, y1, x2 + 1, y2 + 1, color_map);
    }
    // DisplayApi has no async completion callback, so draw_bitmap is synchronous.
    lv_display_flush_ready(disp);
}

error_t lvgl_display_add(struct Device* device, const struct LvglDisplayConfig* config, lv_display_t** out_display) {
    if (device == NULL || config == NULL || out_display == NULL) {
        return ERROR_INVALID_ARGUMENT;
    }
    if (device_get_type(device) != &DISPLAY_TYPE) {
        return ERROR_INVALID_ARGUMENT;
    }

    lv_color_format_t lv_color_format;
    enum DisplayColorFormat kernel_color_format = display_get_color_format(device);
    if (!lvgl_display_map_color_format(kernel_color_format, &lv_color_format)) {
        LOG_E(TAG, "Unsupported color format %d (no LVGL equivalent)", (int)kernel_color_format);
        return ERROR_NOT_SUPPORTED;
    }

    uint16_t hres = display_get_resolution_x(device);
    uint16_t vres = display_get_resolution_y(device);
    uint8_t fb_count = display_get_frame_buffer_count(device);
    uint8_t bpp = lv_color_format_get_size(lv_color_format);

    struct LvglDisplayCtx* ctx = new(std::nothrow) LvglDisplayCtx();
    if (ctx == NULL) {
        return ERROR_OUT_OF_MEMORY;
    }
    struct LvglDeviceContext* wrapper = new(std::nothrow) LvglDeviceContext(ctx);
    if (wrapper == NULL) {
        delete ctx;
        return ERROR_OUT_OF_MEMORY;
    }
    wrapper->device = device;

    ctx->byte_swap = config->swap_bytes;
    ctx->sw_rotate = config->sw_rotate;
    // Only relevant when sw_rotate is set - lvgl_display_try_ppa_rotate() also checks
    // ctx->ppa_eligible directly, so this is safe to compute unconditionally.
    ctx->ppa_eligible = lvgl_ppa_is_supported() && lvgl_ppa_supports_color_format(lv_color_format);
    if (config->sw_rotate && !ctx->ppa_eligible) {
        LOG_I(TAG, "PPA not available for this display (supported=%d, color_format=%d) - using lv_draw_sw_rotate()",
              (int)lvgl_ppa_is_supported(), (int)lv_color_format);
    }
    ctx->has_swap_xy_cap = display_has_capability(device, DISPLAY_CAPABILITY_CAP_SWAP_XY);
    ctx->has_mirror_cap = display_has_capability(device, DISPLAY_CAPABILITY_CAP_MIRROR);
    ctx->has_set_gap_cap = display_has_capability(device, DISPLAY_CAPABILITY_CAP_SET_GAP);
    // sw_rotate is excluded from fb-direct binding below: it writes rotated pixels into
    // ctx->rotate_buf, which lvgl_display_fb_base() doesn't recognize, so fb-direct must stay
    // off in that case as well.
    bool would_bind_fb_direct = fb_count > 0 && ctx->has_swap_xy_cap && ctx->has_mirror_cap && !ctx->sw_rotate;
    if (fb_count > 0 && !would_bind_fb_direct) {
        // Only re-enable capabilities the driver reported off above if it has a *dynamic*
        // has_capability() (DisplayApi.has_capability non-null, e.g. rgb_display_has_capability()/
        // st7701_has_capability()): that's specifically what a driver implements when a capability's
        // availability is state-dependent - here, off only because binding fb-direct would defeat
        // it (see rgb_display_has_capability()) - so once we're falling back to an owned buffer
        // instead (right below), the concern no longer applies. A driver with no dynamic
        // has_capability() reports fixed, hardware-level capabilities instead: if the bit's off,
        // swap_xy()/mirror() don't exist at all (null function pointers - see the DisplayApi
        // contract), so forcing them back on here would call through a null pointer.
        const struct DisplayApi* api = (const struct DisplayApi*)device_get_driver(device)->api;
        if (api->has_capability != NULL) {
            ctx->has_swap_xy_cap = true;
            ctx->has_mirror_cap = true;
        }
    }
    ctx->base_swap_xy = ctx->has_swap_xy_cap ? display_get_swap_xy(device) : false;
    ctx->base_mirror_x = ctx->has_mirror_cap ? display_get_mirror_x(device) : false;
    ctx->base_mirror_y = ctx->has_mirror_cap ? display_get_mirror_y(device) : false;
    ctx->base_gap_x = ctx->has_set_gap_cap ? display_get_gap_x(device) : 0;
    ctx->base_gap_y = ctx->has_set_gap_cap ? display_get_gap_y(device) : 0;

    lv_display_render_mode_t render_mode;
    size_t buf_size_bytes;

    if (lv_color_format == LV_COLOR_FORMAT_I1) {
        // I1 packs 8 pixels/byte row-wise and LVGL reserves an 8-byte palette header at the
        // buffer's start (see lvgl_display_flush_cb()). Always redraw the whole frame in one
        // owned buffer instead of computing partial-region byte offsets against that packing.
        buf_size_bytes = (size_t)((hres + 7) / 8) * vres + 8;
        ctx->buf1 = lvgl_display_alloc_buffer(buf_size_bytes, config->prefer_external_ram);
        if (ctx->buf1 == NULL) {
            delete wrapper;
            return ERROR_OUT_OF_MEMORY;
        }
        ctx->owns_buffers = true;
        render_mode = LV_DISPLAY_RENDER_MODE_FULL;
    } else if (would_bind_fb_direct) {
        display_get_frame_buffer(device, 0, &ctx->buf1);
        if (fb_count > 1) {
            display_get_frame_buffer(device, 1, &ctx->buf2);
        }
        ctx->owns_buffers = false;
        render_mode = LV_DISPLAY_RENDER_MODE_FULL;
        buf_size_bytes = (size_t)hres * vres * bpp;
    } else {
        uint16_t buf_height = config->force_full_frame || config->buffer_height == 0
            ? vres : config->buffer_height;
        buf_size_bytes = (size_t)hres * buf_height * bpp;

        ctx->buf1 = lvgl_display_alloc_buffer(buf_size_bytes, config->prefer_external_ram);
        if (ctx->buf1 == NULL) {
            delete wrapper;
            return ERROR_OUT_OF_MEMORY;
        }
        if (config->double_buffer) {
            ctx->buf2 = lvgl_display_alloc_buffer(buf_size_bytes, config->prefer_external_ram);
            if (ctx->buf2 == NULL) {
                lvgl_display_free_buffer(ctx->buf1);
                delete wrapper;
                return ERROR_OUT_OF_MEMORY;
            }
        }
        ctx->owns_buffers = true;
        render_mode = config->force_full_frame ? LV_DISPLAY_RENDER_MODE_FULL : LV_DISPLAY_RENDER_MODE_PARTIAL;
    }

    ctx->buf_size_bytes = buf_size_bytes;

    if (ctx->sw_rotate) {
        ctx->rotate_buf = lvgl_display_alloc_buffer(buf_size_bytes, config->prefer_external_ram);
        if (ctx->rotate_buf == NULL) {
            if (ctx->owns_buffers) {
                lvgl_display_free_buffer(ctx->buf1);
                lvgl_display_free_buffer(ctx->buf2);
            }
            delete wrapper;
            return ERROR_OUT_OF_MEMORY;
        }
    }

    lv_display_t* disp = lv_display_create(hres, vres);
    if (disp == NULL) {
        if (ctx->owns_buffers) {
            lvgl_display_free_buffer(ctx->buf1);
            lvgl_display_free_buffer(ctx->buf2);
        }
        lvgl_display_free_buffer(ctx->rotate_buf);
        delete wrapper;
        return ERROR_OUT_OF_MEMORY;
    }

    ctx->render_mode = render_mode;
    lv_display_set_color_format(disp, lv_color_format);
    lv_display_set_buffers(disp, ctx->buf1, ctx->buf2, buf_size_bytes, render_mode);
    lv_display_set_flush_cb(disp, lvgl_display_flush_cb);
    lv_display_set_driver_data(disp, wrapper);
    lv_display_add_event_cb(disp, lvgl_display_rotation_event_cb, LV_EVENT_RESOLUTION_CHANGED, wrapper);

    // Apply once explicitly, independent of whether LV_EVENT_RESOLUTION_CHANGED fires on creation.
    lvgl_display_apply_rotation(wrapper, lv_display_get_rotation(disp));

    *out_display = disp;
    return ERROR_NONE;
}

bool lvgl_display_get_shadow_frame(lv_display_t* display, uint8_t** out_data, uint32_t* out_width, uint32_t* out_height, size_t* out_stride) {
    if (display == NULL) {
        return false;
    }
    struct LvglDeviceContext* wrapper = (struct LvglDeviceContext*)lv_display_get_driver_data(display);
    if (wrapper == NULL || wrapper->context == NULL) {
        return false;
    }
    struct LvglDisplayCtx* ctx = (struct LvglDisplayCtx*)wrapper->context;
    // Safe without the LVGL lock: shadow_frame and shadow_stride are written once, on the first
    // flush, and shadow_complete only ever goes from false to true.
    if (ctx->shadow_frame == NULL || !ctx->shadow_complete.load(std::memory_order_acquire)) {
        return false;
    }

    if (out_data != NULL) *out_data = ctx->shadow_frame;
    if (out_width != NULL) *out_width = (uint32_t)lv_display_get_horizontal_resolution(display);
    if (out_height != NULL) *out_height = (uint32_t)lv_display_get_vertical_resolution(display);
    if (out_stride != NULL) *out_stride = ctx->shadow_stride;
    return true;
}

uint32_t lvgl_display_shadow_sequence(lv_display_t* display) {
    if (display == NULL) {
        return 0;
    }
    struct LvglDeviceContext* wrapper = (struct LvglDeviceContext*)lv_display_get_driver_data(display);
    if (wrapper == NULL || wrapper->context == NULL) {
        return 0;
    }
    struct LvglDisplayCtx* ctx = (struct LvglDisplayCtx*)wrapper->context;
    return ctx->shadow_seq.load(std::memory_order_acquire);
}

bool lvgl_display_peek_dirty_area(lv_display_t* display, lv_area_t* out_area, int64_t* out_last_update_us) {
    if (display == NULL) {
        return false;
    }
    struct LvglDeviceContext* wrapper = (struct LvglDeviceContext*)lv_display_get_driver_data(display);
    if (wrapper == NULL || wrapper->context == NULL) {
        return false;
    }
    struct LvglDisplayCtx* ctx = (struct LvglDisplayCtx*)wrapper->context;
    if (ctx->shadow_frame == NULL) {
        return false;
    }

    lvgl_shadow_lock(ctx);
    const bool have_area = !ctx->dirty_empty;
    if (have_area) {
        if (out_area != NULL) {
            out_area->x1 = ctx->dirty_x1;
            out_area->y1 = ctx->dirty_y1;
            out_area->x2 = ctx->dirty_x2;
            out_area->y2 = ctx->dirty_y2;
        }
        if (out_last_update_us != NULL) {
            *out_last_update_us = ctx->dirty_last_update_us;
        }
    }
    lvgl_shadow_unlock(ctx);

    return have_area;
}

bool lvgl_display_take_dirty_area(lv_display_t* display, lv_area_t* out_area) {
    if (display == NULL) {
        return false;
    }
    struct LvglDeviceContext* wrapper = (struct LvglDeviceContext*)lv_display_get_driver_data(display);
    if (wrapper == NULL || wrapper->context == NULL) {
        return false;
    }
    struct LvglDisplayCtx* ctx = (struct LvglDisplayCtx*)wrapper->context;
    if (ctx->shadow_frame == NULL) {
        return false;
    }

    lvgl_shadow_lock(ctx);
    const bool have_area = !ctx->dirty_empty;
    if (have_area) {
        if (out_area != NULL) {
            out_area->x1 = ctx->dirty_x1;
            out_area->y1 = ctx->dirty_y1;
            out_area->x2 = ctx->dirty_x2;
            out_area->y2 = ctx->dirty_y2;
        }
        ctx->dirty_empty = true;
    }
    lvgl_shadow_unlock(ctx);

    return have_area;
}

void lvgl_display_remove(lv_display_t* display) {
    if (display == NULL) {
        return;
    }

    struct LvglDeviceContext* wrapper = (struct LvglDeviceContext*)lv_display_get_driver_data(display);
    lv_display_delete(display);

    if (wrapper != NULL) {
        struct LvglDisplayCtx* ctx = (struct LvglDisplayCtx*)wrapper->context;
        if (ctx->owns_buffers) {
            if (ctx->buf1 != NULL) {
                lvgl_display_free_buffer(ctx->buf1);
            }
            if (ctx->buf2 != NULL) {
                lvgl_display_free_buffer(ctx->buf2);
            }
        }
        if (ctx->rotate_buf != NULL) {
            lvgl_display_free_buffer(ctx->rotate_buf);
        }
        if (ctx->shadow_frame != NULL) {
            lvgl_display_free_buffer(ctx->shadow_frame);
        }
        if (ctx->shadow_rows != NULL) {
            lvgl_display_free_buffer(ctx->shadow_rows);
        }
        if (ctx->ppa_handle != NULL) {
            lvgl_ppa_delete(ctx->ppa_handle);
        }
        delete wrapper;
    }
}
