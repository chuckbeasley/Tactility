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

#ifdef __cplusplus
}
#endif
