// SPDX-License-Identifier: Apache-2.0
// Screen video recorder: captures the active LVGL screen at a fixed rate into PSRAM, then on
// stop encodes the frames to a Motion-JPEG AVI (via esp_new_jpeg). Used by the /api/video/* HTTP
// endpoints.

#include <Tactility/video/VideoRecorder.h>

#include <Tactility/lvgl/Lvgl.h>
#include <lvgl/lvgl.h>

#include <esp_jpeg_enc.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>

#include <algorithm>
#include <cstring>
#include <vector>

namespace {

// Frame caps keep PSRAM bounded. MJPEG frames are small (~5 KB each, 15 frames). Uncompressed RGB
// expands to w*h*3 bytes/frame, so it is capped lower to keep peak memory under the 8 MB PSRAM.
constexpr uint32_t MAX_FRAMES_MJPEG = 15; // ~3 s at 5 fps
constexpr uint32_t MAX_FRAMES_RGB = 8;    // ~1.6 s at 5 fps; peak = 8x(307200 + 460800) ~ 6.1 MB in PSRAM

struct Recorder {
    SemaphoreHandle_t mutex = nullptr;
    bool recording = false;
    uint32_t fps = 5;
    uint32_t codec = 0;
    uint32_t max_frames = MAX_FRAMES_MJPEG;
    uint32_t width = 0;
    uint32_t height = 0;
    TaskHandle_t task = nullptr;
    std::vector<std::vector<uint8_t>> frames; // each: RGB565 (R<->B swapped), w*h*2 bytes
};

Recorder& recorder() {
    static Recorder instance;
    return instance;
}

// The ST7796 panel senses BGR; swap red/blue in each RGB565 pixel so the recording matches the
// physical panel (same as /api/screenshot). 0xF800 = red, 0x07E0 = green, 0x001F = blue.
void bgr_swap_rgb565(uint8_t* data, size_t num_pixels) {
    uint16_t* px = reinterpret_cast<uint16_t*>(data);
    for (size_t i = 0; i < num_pixels; ++i) {
        const uint16_t p = px[i];
        const uint16_t r = p & 0xF800u;
        const uint16_t g = p & 0x07E0u;
        const uint16_t b = p & 0x001Fu;
        px[i] = static_cast<uint16_t>((b << 11) | g | (r >> 11));
    }
}

void captureTask(void* arg) {
    (void)arg;
    Recorder& rec = recorder();
    const uint32_t interval_ms = 1000 / (rec.fps ? rec.fps : 5);

    while (true) {
        // A mutex-guarded "are we still recording?" check.
        xSemaphoreTake(rec.mutex, portMAX_DELAY);
        if (!rec.recording) {
            xSemaphoreGive(rec.mutex);
            break;
        }
        xSemaphoreGive(rec.mutex);

        std::vector<uint8_t> frame;
        if (lvgl_try_lock(pdMS_TO_TICKS(200))) {
            lv_draw_buf_t* draw_buf = lv_snapshot_take(lv_scr_act(), LV_COLOR_FORMAT_RGB565);
            if (draw_buf != nullptr) {
                const uint32_t w = draw_buf->header.w;
                const uint32_t h = draw_buf->header.h;
                const uint32_t stride = draw_buf->header.stride;
                if (w != 0 && h != 0) {
                    frame.resize(static_cast<size_t>(w) * h * 2);
                    // Copy row by row (stride may be padded) so frames are tightly packed.
                    uint8_t* dst = frame.data();
                    for (uint32_t y = 0; y < h; ++y) {
                        std::memcpy(dst, draw_buf->data + static_cast<size_t>(y) * stride,
                                    static_cast<size_t>(w) * 2);
                        dst += static_cast<size_t>(w) * 2;
                    }
                    bgr_swap_rgb565(frame.data(), static_cast<size_t>(w) * h);
                    rec.width = w;
                    rec.height = h;
                }
                lv_draw_buf_destroy(draw_buf);
            }
            lvgl_unlock();
        }

        if (!frame.empty()) {
            xSemaphoreTake(rec.mutex, portMAX_DELAY);
            if (rec.frames.size() < rec.max_frames) {
                rec.frames.emplace_back(std::move(frame));
            }
            xSemaphoreGive(rec.mutex);
        }

        vTaskDelay(pdMS_TO_TICKS(interval_ms));
    }

    xSemaphoreTake(rec.mutex, portMAX_DELAY);
    rec.task = nullptr;
    xSemaphoreGive(rec.mutex);
    vTaskDelete(nullptr);
}

bool encode_jpeg(const uint8_t* rgb565, uint32_t w, uint32_t h, std::vector<uint8_t>& jpeg_out) {
    jpeg_enc_config_t config = DEFAULT_JPEG_ENC_CONFIG();
    config.width = static_cast<int>(w);
    config.height = static_cast<int>(h);
    config.src_type = JPEG_PIXEL_FORMAT_RGB565_LE;
    config.subsampling = JPEG_SUBSAMPLE_420;
    config.quality = 60;
    config.rotate = JPEG_ROTATE_0D;
    config.task_enable = false;

    jpeg_enc_handle_t enc = nullptr;
    if (jpeg_enc_open(&config, &enc) != JPEG_ERR_OK || enc == nullptr) {
        return false;
    }

    // Input size = w*h*2 (RGB565). Output buffer generous enough for any JPEG size.
    const int in_size = static_cast<int>(static_cast<size_t>(w) * h * 2);
    const int out_capacity = std::max<int>(in_size, 65536);
    std::vector<uint8_t> out_buf(static_cast<size_t>(out_capacity));
    int out_size = 0;
    const jpeg_error_t res = jpeg_enc_process(enc, rgb565, in_size, out_buf.data(), out_capacity, &out_size);
    jpeg_enc_close(enc);

    if (res != JPEG_ERR_OK || out_size <= 0) {
        return false;
    }
    jpeg_out.assign(out_buf.begin(), out_buf.begin() + out_size);
    return true;
}

void write_u16_le(std::vector<uint8_t>& v, uint32_t off, uint16_t val) {
    v[off] = static_cast<uint8_t>(val & 0xFF);
    v[off + 1] = static_cast<uint8_t>((val >> 8) & 0xFF);
}

void write_u32_le(std::vector<uint8_t>& v, uint32_t off, uint32_t val) {
    v[off] = static_cast<uint8_t>(val & 0xFF);
    v[off + 1] = static_cast<uint8_t>((val >> 8) & 0xFF);
    v[off + 2] = static_cast<uint8_t>((val >> 16) & 0xFF);
    v[off + 3] = static_cast<uint8_t>((val >> 24) & 0xFF);
}

void append_u32_le(std::vector<uint8_t>& v, uint32_t val) {
    v.push_back(static_cast<uint8_t>(val & 0xFF));
    v.push_back(static_cast<uint8_t>((val >> 8) & 0xFF));
    v.push_back(static_cast<uint8_t>((val >> 16) & 0xFF));
    v.push_back(static_cast<uint8_t>((val >> 24) & 0xFF));
}

void append_fourcc(std::vector<uint8_t>& v, const char* fourcc) {
    v.insert(v.end(), fourcc, fourcc + 4);
}

// Build an AVI (RIFF) file containing the JPEG frames as a 'MJPG' stream.
bool build_avi(const std::vector<std::vector<uint8_t>>& frames, uint32_t w, uint32_t h, uint32_t fps,
               std::vector<uint8_t>& avi) {
    if (frames.empty()) {
        return false;
    }
    const uint32_t frame_count = static_cast<uint32_t>(frames.size());
    const uint32_t frame_interval_ms = fps ? (1000 / fps) : 200;

    // Total compressed bytes: used for the avih dwMaxBytesPerSec (a correct, non-zero bitrate).
    uint32_t total_jpeg_bytes = 0;
    for (const auto& jpg : frames) {
        total_jpeg_bytes += static_cast<uint32_t>(jpg.size());
    }

    avi.clear();
    append_fourcc(avi, "RIFF");
    const uint32_t riff_size_pos = static_cast<uint32_t>(avi.size());
    append_u32_le(avi, 0); // patched
    append_fourcc(avi, "AVI ");

    // --- hdrl LIST ---
    append_fourcc(avi, "LIST");
    const uint32_t hdrl_size_pos = static_cast<uint32_t>(avi.size());
    append_u32_le(avi, 0); // patched
    append_fourcc(avi, "hdrl");

    // avih
    {
        append_fourcc(avi, "avih");
        append_u32_le(avi, 56);
        const uint32_t avih_pos = static_cast<uint32_t>(avi.size());
        avi.resize(avi.size() + 56, 0);
        write_u32_le(avi, avih_pos + 0, frame_interval_ms * 1000); // dwMicroSecPerFrame (e.g. 200000 = 5 fps)
        write_u32_le(avi, avih_pos + 4, total_jpeg_bytes * fps / frame_count); // dwMaxBytesPerSec (bytes/sec)
        write_u32_le(avi, avih_pos + 8, 0);  // dwPaddingGranularity
        write_u32_le(avi, avih_pos + 12, 0x10); // AVIF_HASINDEX
        write_u32_le(avi, avih_pos + 16, frame_count); // dwTotalFrames
        write_u32_le(avi, avih_pos + 20, 0);  // dwInitialFrames
        write_u32_le(avi, avih_pos + 24, 1);  // dwStreams
        write_u32_le(avi, avih_pos + 28, w * h * 3); // dwSuggestedBufferSize (decompressed frame)
        write_u32_le(avi, avih_pos + 32, w);  // dwWidth
        write_u32_le(avi, avih_pos + 36, h);  // dwHeight
        // dwReserved[4] already 0.
    }

    // strl LIST
    {
        append_fourcc(avi, "LIST");
        const uint32_t strl_size_pos = static_cast<uint32_t>(avi.size());
        append_u32_le(avi, 0); // patched
        append_fourcc(avi, "strl");

        // strh
        {
            append_fourcc(avi, "strh");
            append_u32_le(avi, 56);
            const uint32_t sh = static_cast<uint32_t>(avi.size());
            avi.resize(avi.size() + 56, 0);
            avi[sh + 0] = 'v'; avi[sh + 1] = 'i'; avi[sh + 2] = 'd'; avi[sh + 3] = 's'; // fccType
            avi[sh + 4] = 'M'; avi[sh + 5] = 'J'; avi[sh + 6] = 'P'; avi[sh + 7] = 'G'; // fccHandler
            write_u32_le(avi, sh + 8, 0);          // dwFlags
            write_u16_le(avi, sh + 12, 0);         // wPriority
            write_u16_le(avi, sh + 14, 0);         // wLanguage
            write_u32_le(avi, sh + 16, 0);         // dwInitialFrames
            write_u32_le(avi, sh + 20, 1);         // dwScale
            write_u32_le(avi, sh + 24, fps);       // dwRate  (rate/scale = frames per second)
            write_u32_le(avi, sh + 28, 0);         // dwStart
            write_u32_le(avi, sh + 32, frame_count); // dwLength
            write_u32_le(avi, sh + 36, w * h * 3); // dwSuggestedBufferSize
            write_u32_le(avi, sh + 40, static_cast<uint32_t>(-1)); // dwQuality (best)
            write_u32_le(avi, sh + 44, 0);         // dwSampleSize
            // rcFrame[4] = 0 (offset 48..55)
        }

        // strf (BITMAPINFOHEADER, MJPEG = RGB24)
        {
            append_fourcc(avi, "strf");
            append_u32_le(avi, 40);
            const uint32_t bm = static_cast<uint32_t>(avi.size());
            avi.resize(avi.size() + 40, 0);
            auto put = [&](uint32_t off, uint32_t val) { write_u32_le(avi, bm + off, val); };
            put(0, 40);   // biSize
            put(4, w);    // biWidth
            put(8, h);    // biHeight
            write_u16_le(avi, bm + 12, 1);  // biPlanes
            write_u16_le(avi, bm + 14, 24); // biBitCount
            avi[bm + 16] = 'M'; avi[bm + 17] = 'J'; avi[bm + 18] = 'P'; avi[bm + 19] = 'G';
            put(20, static_cast<uint32_t>(w) * h * 3); // biSizeImage
            put(24, 0);   // biXPelsPerMeter
            put(28, 0);   // biYPelsPerMeter
            put(32, 0);   // biClrUsed
            put(36, 0);   // biClrImportant
        }

        const uint32_t strl_end = static_cast<uint32_t>(avi.size());
        write_u32_le(avi, strl_size_pos, strl_end - strl_size_pos - 4);
    }

    const uint32_t hdrl_end = static_cast<uint32_t>(avi.size());
    write_u32_le(avi, hdrl_size_pos, hdrl_end - hdrl_size_pos - 4);

    // --- movi LIST ---
    append_fourcc(avi, "LIST");
    const uint32_t movi_size_pos = static_cast<uint32_t>(avi.size());
    append_u32_le(avi, 0); // patched
    append_fourcc(avi, "movi");
    std::vector<uint32_t> chunk_offsets;
    std::vector<uint32_t> chunk_sizes;
    for (const auto& jpg : frames) {
        chunk_offsets.push_back(static_cast<uint32_t>(avi.size()));
        append_fourcc(avi, "00dc");
        append_u32_le(avi, static_cast<uint32_t>(jpg.size()));
        avi.insert(avi.end(), jpg.begin(), jpg.end());
        if (jpg.size() & 1) {
            avi.push_back(0); // pad to even
        }
        chunk_sizes.push_back(static_cast<uint32_t>(jpg.size()));
    }
    const uint32_t movi_end = static_cast<uint32_t>(avi.size());
    // movi chunk offsets are relative to the start of the 'movi' LIST *data* (after 'movi' fourcc).
    for (uint32_t i = 0; i < chunk_offsets.size(); ++i) {
        chunk_offsets[i] -= movi_size_pos + 8; // movi_size_pos + 8 = start of data
    }
    write_u32_le(avi, movi_size_pos, movi_end - movi_size_pos - 4);

    // --- idx1 ---
    append_fourcc(avi, "idx1");
    const uint32_t idx_size_pos = static_cast<uint32_t>(avi.size());
    // Reserve idx1 size; we don't know it yet, so put frames then patch.
    append_u32_le(avi, static_cast<uint32_t>(chunk_offsets.size() * 16)); // each index entry = 16 bytes
    for (uint32_t i = 0; i < chunk_offsets.size(); ++i) {
        append_fourcc(avi, "00dc");
        append_u32_le(avi, 0x10); // AVIIF_KEYFRAME
        append_u32_le(avi, chunk_offsets[i]);
        append_u32_le(avi, chunk_sizes[i]);
    }

    const uint32_t total = static_cast<uint32_t>(avi.size());
    write_u32_le(avi, riff_size_pos, total - 8);
    return true;
}

// Build an uncompressed 24-bit RGB AVI (BI_RGB). This is the one codec Windows Media Player
// decodes natively. Each captured RGB565 frame is expanded to RGB24 and stored bottom-up as a BGR
// DIB (matching the verified-correct screenshot), so colors match the physical panel.
// The input frame buffers are consumed (freed) as they are written, to keep PSRAM peak bounded.
bool build_avi_uncompressed(std::vector<std::vector<uint8_t>>& frames, uint32_t w, uint32_t h,
                            uint32_t fps, std::vector<uint8_t>& avi) {
    if (frames.empty()) {
        return false;
    }
    const uint32_t frame_count = static_cast<uint32_t>(frames.size());
    const uint32_t frame_interval_ms = fps ? (1000 / fps) : 200;
    const uint32_t row_bytes = w * 3;
    const uint32_t row_pad = (4 - (row_bytes % 4)) % 4; // 0 for w=480 (1440 % 4 == 0)
    const uint32_t frame_size = (row_bytes + row_pad) * h;

    avi.clear();
    append_fourcc(avi, "RIFF");
    const uint32_t riff_size_pos = static_cast<uint32_t>(avi.size());
    append_u32_le(avi, 0); // patched
    append_fourcc(avi, "AVI ");

    append_fourcc(avi, "LIST");
    const uint32_t hdrl_size_pos = static_cast<uint32_t>(avi.size());
    append_u32_le(avi, 0);
    append_fourcc(avi, "hdrl");

    // avih
    {
        append_fourcc(avi, "avih");
        append_u32_le(avi, 56);
        const uint32_t ap = static_cast<uint32_t>(avi.size());
        avi.resize(avi.size() + 56, 0);
        write_u32_le(avi, ap + 0, frame_interval_ms * 1000);       // dwMicroSecPerFrame
        write_u32_le(avi, ap + 4, frame_size * fps);               // dwMaxBytesPerSec
        write_u32_le(avi, ap + 8, 0);                              // dwPaddingGranularity
        write_u32_le(avi, ap + 12, 0x10);                          // AVIF_HASINDEX
        write_u32_le(avi, ap + 16, frame_count);                   // dwTotalFrames
        write_u32_le(avi, ap + 20, 0);                             // dwInitialFrames
        write_u32_le(avi, ap + 24, 1);                             // dwStreams
        write_u32_le(avi, ap + 28, frame_size);                    // dwSuggestedBufferSize
        write_u32_le(avi, ap + 32, w);                             // dwWidth
        write_u32_le(avi, ap + 36, h);                             // dwHeight
    }

    append_fourcc(avi, "LIST");
    const uint32_t strl_size_pos = static_cast<uint32_t>(avi.size());
    append_u32_le(avi, 0);
    append_fourcc(avi, "strl");

    // strh -- fccHandler 'DIB ' marks an uncompressed DIB video stream
    {
        append_fourcc(avi, "strh");
        append_u32_le(avi, 56);
        const uint32_t sh = static_cast<uint32_t>(avi.size());
        avi.resize(avi.size() + 56, 0);
        avi[sh + 0] = 'v'; avi[sh + 1] = 'i'; avi[sh + 2] = 'd'; avi[sh + 3] = 's';
        avi[sh + 4] = 'D'; avi[sh + 5] = 'I'; avi[sh + 6] = 'B'; avi[sh + 7] = ' ';
        write_u32_le(avi, sh + 8, 0);
        write_u16_le(avi, sh + 12, 0);
        write_u16_le(avi, sh + 14, 0);
        write_u32_le(avi, sh + 16, 0);       // dwInitialFrames
        write_u32_le(avi, sh + 20, 1);       // dwScale
        write_u32_le(avi, sh + 24, fps);     // dwRate (rate/scale = fps)
        write_u32_le(avi, sh + 28, 0);       // dwStart
        write_u32_le(avi, sh + 32, frame_count); // dwLength
        write_u32_le(avi, sh + 36, frame_size);  // dwSuggestedBufferSize
        write_u32_le(avi, sh + 40, 0);       // dwQuality
        write_u32_le(avi, sh + 44, 0);       // dwSampleSize
    }

    // strf -- BITMAPINFOHEADER, BI_RGB (24-bit, bottom-up)
    {
        append_fourcc(avi, "strf");
        append_u32_le(avi, 40);
        const uint32_t bm = static_cast<uint32_t>(avi.size());
        avi.resize(avi.size() + 40, 0);
        write_u32_le(avi, bm + 0, 40);
        write_u32_le(avi, bm + 4, w);
        write_u32_le(avi, bm + 8, h);   // positive height -> bottom-up
        write_u16_le(avi, bm + 12, 1);  // biPlanes
        write_u16_le(avi, bm + 14, 24); // biBitCount
        write_u32_le(avi, bm + 16, 0);  // biCompression = BI_RGB
        write_u32_le(avi, bm + 20, frame_size); // biSizeImage
        write_u32_le(avi, bm + 24, 0);
        write_u32_le(avi, bm + 28, 0);
        write_u32_le(avi, bm + 32, 0);
        write_u32_le(avi, bm + 36, 0);
    }

    const uint32_t strl_end = static_cast<uint32_t>(avi.size());
    write_u32_le(avi, strl_size_pos, strl_end - strl_size_pos - 4);
    const uint32_t hdrl_end = static_cast<uint32_t>(avi.size());
    write_u32_le(avi, hdrl_size_pos, hdrl_end - hdrl_size_pos - 4);

    // movi LIST
    append_fourcc(avi, "LIST");
    const uint32_t movi_size_pos = static_cast<uint32_t>(avi.size());
    append_u32_le(avi, 0);
    append_fourcc(avi, "movi");
    const uint32_t data_start = static_cast<uint32_t>(avi.size());

    // Reserve the entire output (movi frames + idx1) up front so the vector never reallocates while
    // source frames are still held. A growth realloc here would transiently need ~2x the AVI size in
    // PSRAM and OOM the 8 MB pool. Include the idx1 entries and pad so nothing reallocates.
    const size_t total_capacity = static_cast<size_t>(data_start)
            + static_cast<size_t>(frame_count) * (8 + frame_size)
            + static_cast<size_t>(frame_count) * 16   // idx1 entries
            + 8;                                       // idx1 header + slack
    avi.reserve(total_capacity);

    std::vector<uint32_t> chunk_offsets;
    std::vector<uint32_t> chunk_sizes;
    for (uint32_t i = 0; i < frame_count; ++i) {
        chunk_offsets.push_back(static_cast<uint32_t>(avi.size()) - data_start);
        append_fourcc(avi, "00dc");
        append_u32_le(avi, frame_size);
        const std::vector<uint8_t>& fr = frames[i];
        const uint8_t* src = fr.data();
        // Expand bottom-up, BGR byte order (like the verified-correct screenshot BMP).
        for (uint32_t y = h; y > 0; --y) {
            const uint8_t* srow = src + static_cast<size_t>(y - 1) * w * 2;
            for (uint32_t x = 0; x < w; ++x) {
                const uint16_t p = static_cast<uint16_t>(srow[x * 2] | (srow[x * 2 + 1] << 8));
                const uint8_t r5 = (p >> 11) & 0x1F;
                const uint8_t g6 = (p >> 5) & 0x3F;
                const uint8_t b5 = p & 0x1F;
                avi.push_back(static_cast<uint8_t>((b5 << 3) | (b5 >> 2))); // blue
                avi.push_back(static_cast<uint8_t>((g6 << 2) | (g6 >> 4))); // green
                avi.push_back(static_cast<uint8_t>((r5 << 3) | (r5 >> 2))); // red
            }
            for (uint32_t z = 0; z < row_pad; ++z) {
                avi.push_back(0);
            }
        }
        chunk_sizes.push_back(frame_size);
        // Free this source frame now to lower peak PSRAM use.
        std::vector<uint8_t>().swap(frames[i]);
        if (frame_size & 1) {
            avi.push_back(0);
        }
    }
    const uint32_t movi_end = static_cast<uint32_t>(avi.size());
    write_u32_le(avi, movi_size_pos, movi_end - movi_size_pos - 4);

    // idx1
    append_fourcc(avi, "idx1");
    append_u32_le(avi, frame_count * 16);
    for (uint32_t i = 0; i < frame_count; ++i) {
        append_fourcc(avi, "00dc");
        append_u32_le(avi, 0x10);
        append_u32_le(avi, chunk_offsets[i]);
        append_u32_le(avi, chunk_sizes[i]);
    }

    const uint32_t total = static_cast<uint32_t>(avi.size());
    write_u32_le(avi, riff_size_pos, total - 8);
    return true;
}

} // namespace

bool tt_video_start(uint32_t fps, uint32_t codec) {
    Recorder& rec = recorder();
    if (rec.mutex == nullptr) {
        rec.mutex = xSemaphoreCreateMutex();
    }
    xSemaphoreTake(rec.mutex, portMAX_DELAY);
    if (rec.recording) {
        xSemaphoreGive(rec.mutex);
        return false;
    }
    rec.frames.clear();
    rec.recording = true;
    rec.fps = fps ? fps : 5;
    rec.codec = codec;
    rec.max_frames = (codec == TT_VIDEO_CODEC_RGB) ? MAX_FRAMES_RGB : MAX_FRAMES_MJPEG;
    rec.width = 0;
    rec.height = 0;
    const BaseType_t ok = xTaskCreate(captureTask, "ttvideo", 8192, nullptr, 5, &rec.task);
    xSemaphoreGive(rec.mutex);
    return ok == pdPASS;
}

bool tt_video_stop(std::vector<uint8_t>* avi_out, uint32_t* out_frames) {
    Recorder& rec = recorder();
    if (rec.mutex == nullptr) {
        return false;
    }
    xSemaphoreTake(rec.mutex, portMAX_DELAY);
    if (!rec.recording) {
        xSemaphoreGive(rec.mutex);
        return false;
    }
    rec.recording = false;
    const TaskHandle_t task = rec.task;
    const uint32_t w = rec.width;
    const uint32_t h = rec.height;
    const uint32_t fps = rec.fps;
    const uint32_t codec = rec.codec;
    xSemaphoreGive(rec.mutex);

    if (task != nullptr) {
        // Wait (up to ~2s) for the capture task to notice recording==false and delete itself.
        uint32_t waited = 0;
        while (rec.task != nullptr && waited < 2000) {
            vTaskDelay(pdMS_TO_TICKS(10));
            waited += 10;
        }
    }

    // Snapshot frames out from under the mutex.
    std::vector<std::vector<uint8_t>> frames;
    xSemaphoreTake(rec.mutex, portMAX_DELAY);
    frames.swap(rec.frames);
    xSemaphoreGive(rec.mutex);

    if (avi_out != nullptr) {
        if (out_frames != nullptr) {
            *out_frames = static_cast<uint32_t>(frames.size());
        }
        if (frames.empty() || w == 0 || h == 0) {
            return false;
        }
        if (codec == TT_VIDEO_CODEC_RGB) {
            // Uncompressed RGB24 (BI_RGB) AVI - plays in Windows Media Player. Consumes [frames].
            return build_avi_uncompressed(frames, w, h, fps, *avi_out);
        }
        // Motion-JPEG: encode each frame to JPEG, then wrap in an AVI.
        std::vector<std::vector<uint8_t>> jpegs;
        jpegs.reserve(frames.size());
        for (const auto& frame : frames) {
            std::vector<uint8_t> jpg;
            if (!encode_jpeg(frame.data(), w, h, jpg)) {
                return false;
            }
            jpegs.emplace_back(std::move(jpg));
        }
        return build_avi(jpegs, w, h, fps, *avi_out);
    }

    return true;
}

bool tt_video_is_recording(void) {
    Recorder& rec = recorder();
    if (rec.mutex == nullptr) {
        return false;
    }
    xSemaphoreTake(rec.mutex, portMAX_DELAY);
    const bool recording = rec.recording;
    xSemaphoreGive(rec.mutex);
    return recording;
}
