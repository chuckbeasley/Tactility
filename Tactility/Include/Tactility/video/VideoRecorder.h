// SPDX-License-Identifier: Apache-2.0
#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include <vector>

#ifdef __cplusplus
extern "C" {
#endif

/** Video output codec. */
#define TT_VIDEO_CODEC_MJPEG 0 // Motion-JPEG (small, but Windows Media Player lacks this decoder)
#define TT_VIDEO_CODEC_RGB   1 // Uncompressed 24-bit RGB (BI_RGB) - plays in Windows Media Player & everywhere

/**
 * Start recording the active LVGL screen to an in-memory buffer at @a fps frames per second.
 * Frames are captured as 16-bit RGB565 (little-endian) snapshots of the LVGL screen.
 * @param fps  frames per second (clamped to 1..30).
 * @param codec one of TT_VIDEO_CODEC_*; TT_VIDEO_CODEC_RGB (default) produces an uncompressed
 *              AVI that Windows Media Player can decode, TT_VIDEO_CODEC_MJPEG produces a small
 *              Motion-JPEG AVI.
 * @return true if recording started, false if already recording or capture failed.
 */
bool tt_video_start(uint32_t fps, uint32_t codec);

/**
 * Stop recording and encode the captured frames into an AVI file.
 * @param[out] avi_out receives the encoded AVI bytes (cleared first). May be null.
 * @param[out] out_frames receives the number of frames captured (may be null).
 * @return true on success, false if not recording or the buffer is empty.
 */
bool tt_video_stop(std::vector<uint8_t>* avi_out, uint32_t* out_frames);

/** @return true while the recorder is capturing. */
bool tt_video_is_recording(void);

/**
 * Capture the active LVGL screen and JPEG-encode it as a single frame.
 *
 * Unlike the recorder above, this is meant for streaming (e.g. one frame per client request):
 * the scratch buffers are allocated once, in PSRAM, on first use and then reused, so repeated
 * calls don't churn the heap. Calls are serialized internally and the returned pointer stays
 * valid only until the next call, so a single consumer must finish with it first.
 *
 * @param[in]  quality    JPEG quality, 1..100. Pass 0 for the default (60).
 * @param[in]  scale      Downscale factor, 1..4 (1 or 0 = full resolution). 2 halves each axis,
 *                        cutting the encode to roughly a quarter of the pixels at half the detail.
 * @param[out] out_data   receives the JPEG bytes (not null-terminated). May be null.
 * @param[out] out_size   receives the JPEG length in bytes. May be null.
 * @param[out] out_width  receives the captured width in pixels. May be null.
 * @param[out] out_height receives the captured height in pixels. May be null.
 * @return true on success; false if the LVGL lock couldn't be taken or capture/encode failed.
 */
bool tt_video_grab_jpeg(int quality, int scale, const uint8_t** out_data, size_t* out_size, uint32_t* out_width, uint32_t* out_height);

/** Which kind of frame tt_video_grab_delta() produced. */
typedef enum {
    TT_VIDEO_FRAME_NONE,       /**< Nothing changed since the last delta was taken. */
    TT_VIDEO_FRAME_DELTA,      /**< A small changed region is available, as raw RGB565. */
    TT_VIDEO_FRAME_DELTA_JPEG, /**< A changed region is available, as a JPEG of that region. */
    TT_VIDEO_FRAME_FULL,       /**< No usable delta (nothing rendered yet, or the whole screen changed). */
} TtVideoFrameKind;

/**
 * Tries to produce a change-only frame: just the region of the screen that changed since the
 * previous call, in standard channel order.
 *
 * Most UI updates touch a small part of the screen (a keystroke, a clock tick, a highlight), and
 * sending only that skips the share of a frame that dominates the pipeline - the full-frame copy
 * and the JPEG encode both scale with the amount of data, so a small change costs a small fraction
 * of a full frame. The region comes back one of two ways:
 *
 *  - TT_VIDEO_FRAME_DELTA, raw RGB565, for regions small enough that the pixels cost less than the
 *    framing a JPEG would add;
 *  - TT_VIDEO_FRAME_DELTA_JPEG, a JPEG of exactly that region, for everything larger. Raw pixels
 *    for half the screen are ~150 KB, where the same region as a JPEG is ~2 KB, so past a small
 *    size compressing the region is far cheaper than sending it - and much cheaper than the whole
 *    frame, which is what the caller got before this existed.
 *
 * Only a change to (nearly) the whole screen reports TT_VIDEO_FRAME_FULL.
 *
 * The returned pointer stays valid until the next call.
 *
 * @param[out] out_data   receives the pixels (raw RGB565) or the JPEG depending on the return value.
 * @param[out] out_size   receives the payload length in bytes. May be null.
 * @param[out] out_x      receives the region's left edge. May be null.
 * @param[out] out_y      receives the region's top edge. May be null.
 * @param[out] out_width  receives the region width. May be null.
 * @param[out] out_height receives the region height. May be null.
 */
TtVideoFrameKind tt_video_grab_delta(const uint8_t** out_data, size_t* out_size, uint32_t* out_x, uint32_t* out_y, uint32_t* out_width, uint32_t* out_height);

/** Diagnostics for the most recent tt_video_grab_jpeg() call. All times are milliseconds. */
struct TtVideoGrabStats {
    uint32_t capture_ms;       /**< Frame acquisition: LVGL lock wait plus copy out of the source. */
    uint32_t lock_wait_ms;     /**< Part of capture_ms spent waiting for the LVGL lock. */
    uint32_t copy_ms;          /**< Part of capture_ms spent copying the source (and allocating buffers).
                                    Includes the channel swap and any downscale, which happen in the
                                    same pass over the pixels rather than as separate passes. */
    uint32_t swap_ms;          /**< Always 0: kept so existing readers of /api/sysinfo do not change. */
    uint32_t encode_ms;        /**< JPEG encode. */
    uint32_t encode_open_ms;   /**< Part of encode_ms spent setting the encoder up. Zero whenever a
                                    cached encoder was reused, which is the normal case. */
    uint32_t frames;           /**< Number of successful grabs since boot. */
    bool used_shadow_frame;    /**< true when the display's shadow frame was used instead of a snapshot. */
    uint32_t resolution_w;     /**< Display resolution at capture time. */
    uint32_t resolution_h;     /**< Display resolution at capture time. */
    uint32_t quality;          /**< JPEG quality actually used. */
    uint32_t scale;            /**< Downscale factor actually used (1 = full resolution). */
    uint32_t output_w;         /**< Width of the encoded frame. */
    uint32_t output_h;         /**< Height of the encoded frame. */
    uint32_t delta_frames;     /**< Frames sent as change-only raw regions since boot. */
    uint32_t region_jpeg_frames; /**< Frames sent as change-only JPEG regions since boot. */
    uint32_t full_frames;      /**< Frames sent as full JPEGs since boot. */
    uint32_t same_frames;      /**< Frame requests answered with "nothing changed" since boot. */
};

/** Copies the timings of the most recent tt_video_grab_jpeg() call into @a out. */
void tt_video_get_grab_stats(struct TtVideoGrabStats* out);

#ifdef __cplusplus
}
#endif
