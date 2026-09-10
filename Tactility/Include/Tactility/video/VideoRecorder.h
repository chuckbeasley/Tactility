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
 * @param[out] out_data   receives the JPEG bytes (not null-terminated). May be null.
 * @param[out] out_size   receives the JPEG length in bytes. May be null.
 * @param[out] out_width  receives the captured width in pixels. May be null.
 * @param[out] out_height receives the captured height in pixels. May be null.
 * @return true on success; false if the LVGL lock couldn't be taken or capture/encode failed.
 */
bool tt_video_grab_jpeg(const uint8_t** out_data, size_t* out_size, uint32_t* out_width, uint32_t* out_height);

/** Diagnostics for the most recent tt_video_grab_jpeg() call. All times are milliseconds. */
struct TtVideoGrabStats {
    uint32_t capture_ms;      /**< Frame acquisition: LVGL lock wait plus copy out of the source. */
    uint32_t swap_ms;         /**< BGR/RGB channel swap. */
    uint32_t encode_ms;       /**< JPEG encode. */
    uint32_t frames;          /**< Number of successful grabs since boot. */
    bool used_display_buffer; /**< false when it had to fall back to lv_snapshot_take(). */
    uint32_t active_w;        /**< lv_display_get_buf_active() width, 0 when it returned null. */
    uint32_t active_h;        /**< lv_display_get_buf_active() height, 0 when it returned null. */
    uint32_t resolution_w;    /**< Display resolution at capture time. */
    uint32_t resolution_h;    /**< Display resolution at capture time. */
};

/** Copies the timings of the most recent tt_video_grab_jpeg() call into @a out. */
void tt_video_get_grab_stats(struct TtVideoGrabStats* out);

#ifdef __cplusplus
}
#endif
