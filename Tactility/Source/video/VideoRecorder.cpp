// SPDX-License-Identifier: Apache-2.0
// Screen video recorder: captures the active LVGL screen at a fixed rate into PSRAM, then on
// stop encodes the frames to a Motion-JPEG AVI (via esp_new_jpeg). Used by the /api/video/* HTTP
// endpoints.

#include <Tactility/video/VideoRecorder.h>

#include <Tactility/lvgl/Lvgl.h>
#include <lvgl/devices/display.h>
#include <lvgl/lvgl.h>

#include <esp_jpeg_enc.h>
#include <esp_heap_caps.h>
#include <esp_timer.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>

#include <algorithm>
#include <cstring>
#include <vector>

namespace {

// Timings of the most recent single-frame grab, surfaced through /api/sysinfo (see
// tt_video_get_grab_stats). Kept in RAM rather than logged per frame: a per-frame LOG_I backs the
// UART up under a slow reader, and a blocked log call stalls the very path being measured.
TtVideoGrabStats g_grab_stats = {};

// Shared by the full-frame and delta paths: both read the display's shadow frame and use reusable
// scratch buffers, so they must not run concurrently.
SemaphoreHandle_t g_grab_mutex = nullptr;
uint8_t* g_delta_buf = nullptr;
size_t g_delta_capacity = 0;
// Where a change-only region is encoded. Grown to the region's worst case, which is a whole frame's
// worth of pixels but only ever holds the whole screen when the screen really did all change.
uint8_t* g_region_jpeg_buf = nullptr;
size_t g_region_jpeg_capacity = 0;

// How much of the screen a change may cover before it is compressed rather than sent raw: 1/8 of
// the frame is about 38 KB raw at 480x320, versus a ~5 KB JPEG - past that point the encode is far
// cheaper than the pixels.
constexpr size_t DELTA_MAX_AREA_DIVISOR = 8;

// The region encoder uses the same default quality as a full frame, so a region and a whole frame
// look alike on the client.
constexpr int kRegionJpegQuality = 60;

bool ensureGrabMutex() {
    if (g_grab_mutex == nullptr) {
        g_grab_mutex = xSemaphoreCreateMutex();
    }
    return g_grab_mutex != nullptr;
}

/** Grows the region scratch buffer to hold `bytes` of raw RGB565. False when it could not. */
bool ensureDeltaCapacity(size_t bytes) {
    if (g_delta_capacity >= bytes && g_delta_buf != nullptr) {
        return true;
    }
    if (g_delta_buf != nullptr) {
        heap_caps_free(g_delta_buf);
        g_delta_buf = nullptr;
        g_delta_capacity = 0;
    }
    // 16-byte aligned for the same reason as the full-frame buffer: it is encoder input.
    g_delta_buf = static_cast<uint8_t*>(heap_caps_aligned_alloc(16, bytes, MALLOC_CAP_SPIRAM));
    g_delta_capacity = (g_delta_buf != nullptr) ? bytes : 0;
    return g_delta_buf != nullptr;
}

/** Grows the region JPEG output buffer to `need` bytes. Null when it could not. */
uint8_t* ensureRegionJpegCapacity(size_t need) {
    if (g_region_jpeg_capacity >= need && g_region_jpeg_buf != nullptr) {
        return g_region_jpeg_buf;
    }
    if (g_region_jpeg_buf != nullptr) {
        heap_caps_free(g_region_jpeg_buf);
        g_region_jpeg_buf = nullptr;
        g_region_jpeg_capacity = 0;
    }
    g_region_jpeg_buf = static_cast<uint8_t*>(heap_caps_malloc(need, MALLOC_CAP_SPIRAM));
    g_region_jpeg_capacity = (g_region_jpeg_buf != nullptr) ? need : 0;
    return g_region_jpeg_buf;
}

/** Worst-case JPEG size for a region, using the same rule of thumb as the full-frame path: a JPEG
 * never comes out larger than the raw pixels, and the encoder is always given a floor of 64 KB. */
size_t regionJpegNeed(size_t rgb_bytes) {
    return std::max<size_t>(rgb_bytes, 65536);
}

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
    // Two pixels per iteration through a single 32-bit load/store. The buffer usually lives in
    // PSRAM, where the per-pixel read-modify-write round trip - not the arithmetic - is what makes
    // this loop expensive, so halving the number of accesses matters more than the bit math.
    uint32_t* pairs = reinterpret_cast<uint32_t*>(data);
    const size_t pair_count = num_pixels / 2;
    for (size_t i = 0; i < pair_count; ++i) {
        const uint32_t p = pairs[i];
        const uint32_t lo = ((p & 0x0000001Fu) << 11) | (p & 0x000007E0u) | ((p & 0x0000F800u) >> 11);
        const uint32_t hi = ((p & 0x001F0000u) << 11) | (p & 0x07E00000u) | ((p & 0xF8000000u) >> 11);
        pairs[i] = (hi & 0xFFFF0000u) | (lo & 0x0000FFFFu);
    }
    if ((num_pixels & 1) != 0) {
        uint16_t* px = reinterpret_cast<uint16_t*>(data);
        const uint16_t p = px[num_pixels - 1];
        px[num_pixels - 1] = static_cast<uint16_t>(((p & 0x001Fu) << 11) | (p & 0x07E0u) | ((p & 0xF800u) >> 11));
    }
}

// Copies a region out of a strided source into a tight destination while exchanging red and blue
// in every pixel - one pass instead of a copy pass followed by a swap pass.
//
// The exchange cannot happen where the pixels are produced (the display's shadow frame holds
// LVGL's logical channel order, and other consumers depend on that), and as a separate pass it
// costs a full extra read and write of the frame. On this hardware that is the expensive part:
// both buffers live in PSRAM, where these loops move roughly 9 MB/s, so a 480x320 frame costs
// ~68 ms to copy and another ~46 ms to swap it in place. Fused it is a single ~68 ms pass.
//
// 0xF800 = red, 0x07E0 = green, 0x001F = blue. Two pixels per 32-bit access, because on PSRAM the
// number of accesses is what costs, not the bit math.
static void copy_swap_rgb565(uint8_t* dst, const uint8_t* src, uint32_t src_stride, uint32_t w, uint32_t h) {
    const size_t row_bytes = static_cast<size_t>(w) * 2;
    // Rows are addressed individually, so the wide loop needs every row start 4-byte aligned.
    const bool wide_ok = (((uintptr_t)src | (uintptr_t)dst | src_stride | row_bytes) & 3u) == 0 && (w & 1u) == 0;

    for (uint32_t y = 0; y < h; ++y) {
        const uint8_t* src_row = src + static_cast<size_t>(y) * src_stride;
        uint8_t* dst_row = dst + static_cast<size_t>(y) * row_bytes;

        if (!wide_ok) {
            const uint16_t* in = reinterpret_cast<const uint16_t*>(src_row);
            uint16_t* out = reinterpret_cast<uint16_t*>(dst_row);
            for (uint32_t x = 0; x < w; ++x) {
                const uint16_t p = in[x];
                out[x] = static_cast<uint16_t>(((p & 0x001Fu) << 11) | (p & 0x07E0u) | ((p & 0xF800u) >> 11));
            }
            continue;
        }

        const uint32_t* in = reinterpret_cast<const uint32_t*>(src_row);
        uint32_t* out = reinterpret_cast<uint32_t*>(dst_row);
        const uint32_t pairs = w / 2;
        for (uint32_t i = 0; i < pairs; ++i) {
            const uint32_t p = in[i];
            const uint32_t lo = ((p & 0x0000001Fu) << 11) | (p & 0x000007E0u) | ((p & 0x0000F800u) >> 11);
            const uint32_t hi = ((p & 0x001F0000u) << 11) | (p & 0x07E00000u) | ((p & 0xF8000000u) >> 11);
            out[i] = (hi & 0xFFFF0000u) | (lo & 0x0000FFFFu);
        }
    }
}

// copy_swap_rgb565, but box-averaging `scale`x`scale` blocks into one pixel on the way, so a
// downscaled frame never materialises a full-resolution intermediate to average afterwards.
// Returns the output size through out_w/out_h.
//
// Averaging is symmetric in red and blue, so the exchanged value is written directly and the
// downsample costs nothing extra.
static void copy_scale_swap_rgb565(uint8_t* dst, const uint8_t* src, uint32_t src_stride,
                                   uint32_t w, uint32_t h, uint32_t scale,
                                   uint32_t* out_w, uint32_t* out_h) {
    const uint32_t new_w = w / scale;
    const uint32_t new_h = h / scale;
    if (scale <= 1 || new_w == 0 || new_h == 0) {
        copy_swap_rgb565(dst, src, src_stride, w, h);
        *out_w = w;
        *out_h = h;
        return;
    }

    const uint32_t samples = scale * scale;
    const uint32_t half = samples / 2;
    const size_t dst_row_bytes = static_cast<size_t>(new_w) * 2;

    for (uint32_t y = 0; y < new_h; ++y) {
        uint16_t* out = reinterpret_cast<uint16_t*>(dst + static_cast<size_t>(y) * dst_row_bytes);
        for (uint32_t x = 0; x < new_w; ++x) {
            uint32_t r = 0;
            uint32_t g = 0;
            uint32_t b = 0;
            for (uint32_t sy = 0; sy < scale; ++sy) {
                const uint16_t* row = reinterpret_cast<const uint16_t*>(
                    src + static_cast<size_t>(y * scale + sy) * src_stride) + static_cast<size_t>(x) * scale;
                for (uint32_t sx = 0; sx < scale; ++sx) {
                    const uint16_t p = row[sx];
                    r += (p >> 11) & 0x1Fu;
                    g += (p >> 5) & 0x3Fu;
                    b += p & 0x1Fu;
                }
            }
            out[x] = static_cast<uint16_t>((((b + half) / samples) << 11) |
                                           (((g + half) / samples) << 5) |
                                           ((r + half) / samples));
        }
    }

    *out_w = new_w;
    *out_h = new_h;
}

// Encoder handles are meant to be reused: jpeg_enc_open() builds the working buffers and the
// quantization/Huffman tables for one image geometry and quality, and the library exposes
// jpeg_enc_set_quality() precisely so a caller can keep a handle and vary the quality between
// images. Opening one per frame pays that setup every time.
//
// Two slots, because the mirror alternates between a whole frame and a region of arbitrary size.
// A request that matches a slot reuses it; only a third distinct geometry replaces anything, so
// alternating between two shapes keeps both warm.
constexpr size_t JPEG_ENC_SLOTS = 2;

struct JpegEncoderSlot {
    jpeg_enc_handle_t handle = nullptr;
    uint32_t width = 0;
    uint32_t height = 0;
    int quality = 0;
};

JpegEncoderSlot g_enc_slots[JPEG_ENC_SLOTS];
size_t g_enc_next_victim = 0;
// Time the last encode spent in setup (zero when a cached handle was reused). Published through
// /api/sysinfo so the cost of opening is visible rather than assumed.
uint32_t g_last_encode_open_us = 0;

/** Returns an encoder for this geometry, opening one only when no slot already matches. */
jpeg_enc_handle_t acquireEncoder(uint32_t w, uint32_t h, int quality) {
    for (auto& slot : g_enc_slots) {
        if (slot.handle != nullptr && slot.width == w && slot.height == h && slot.quality == quality) {
            return slot.handle;
        }
    }

    JpegEncoderSlot& slot = g_enc_slots[g_enc_next_victim];
    g_enc_next_victim = (g_enc_next_victim + 1) % JPEG_ENC_SLOTS;

    if (slot.handle != nullptr) {
        jpeg_enc_close(slot.handle);
        slot.handle = nullptr;
    }

    jpeg_enc_config_t config = DEFAULT_JPEG_ENC_CONFIG();
    config.width = static_cast<int>(w);
    config.height = static_cast<int>(h);
    config.src_type = JPEG_PIXEL_FORMAT_RGB565_LE;
    config.subsampling = JPEG_SUBSAMPLE_420;
    config.quality = static_cast<uint8_t>(quality);
    config.rotate = JPEG_ROTATE_0D;
    config.task_enable = false;

    jpeg_enc_handle_t handle = nullptr;
    if (jpeg_enc_open(&config, &handle) != JPEG_ERR_OK || handle == nullptr) {
        return nullptr;
    }

    slot.handle = handle;
    slot.width = w;
    slot.height = h;
    slot.quality = quality;
    return handle;
}

// Encodes a standard-RGB565 buffer as JPEG. Returns the number of bytes produced, or 0 on failure.
static size_t encode_jpeg_rgb565(const uint8_t* rgb565, uint32_t w, uint32_t h, int quality,
                                 uint8_t* out, size_t out_capacity) {
    const int64_t open_start = esp_timer_get_time();
    jpeg_enc_handle_t enc = acquireEncoder(w, h, quality);
    g_last_encode_open_us = static_cast<uint32_t>(esp_timer_get_time() - open_start);
    if (enc == nullptr) {
        return 0;
    }

    int produced = 0;
    const jpeg_error_t result = jpeg_enc_process(
        enc,
        rgb565,
        static_cast<int>(static_cast<size_t>(w) * h * 2),
        out,
        static_cast<int>(out_capacity),
        &produced
    );

    return (result == JPEG_ERR_OK && produced > 0) ? static_cast<size_t>(produced) : 0;
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

bool tt_video_grab_jpeg(int quality, int scale, const uint8_t** out_data, size_t* out_size, uint32_t* out_width, uint32_t* out_height) {
    // Clamp the caller's knobs before anything else so the rest of the function can rely on them.
    if (quality < 1 || quality > 100) {
        quality = 60;
    }
    if (scale < 1 || scale > 4) {
        scale = 1;
    }

    // Scratch buffers live for the process lifetime once allocated: streaming asks for frames
    // continuously, so re-allocating ~300 KB per frame would fragment the heap for no benefit.
    static uint8_t* rgb565_buf = nullptr;
    static size_t rgb565_capacity = 0;
    static uint8_t* jpeg_buf = nullptr;
    static size_t jpeg_capacity = 0;

    if (!ensureGrabMutex()) {
        return false;
    }
    if (xSemaphoreTake(g_grab_mutex, portMAX_DELAY) != pdTRUE) {
        return false;
    }

    const int64_t t_start = esp_timer_get_time();
    int64_t t_lock_acquired = 0;
    int64_t t_copy_done = 0;
    int64_t t_capture_done = 0;
    int64_t t_swap_done = 0;
    bool ok = false;
    bool captured = false;
    bool used_shadow_frame = false;
    uint32_t w = 0;
    uint32_t h = 0;

    // Capture under the LVGL lock. The display maintains a complete shadow frame as it flushes
    // regions (see lvgl_display_get_shadow_frame), so this copies a finished frame instead of
    // re-rendering the widget tree with lv_snapshot_take() - the difference between ~70 ms and
    // ~350 ms at 480x320.
    //
    // The shadow is written by flushes on the LVGL task, so the copy happens under the lock; the
    // channel swap and JPEG encode are deliberately left for after the unlock to keep the UI's
    // stall short. The elapsed time is split into lock wait and copy (see TtVideoGrabStats) because
    // they call for completely different optimizations.
    if (lvgl_try_lock(pdMS_TO_TICKS(200))) {
        t_lock_acquired = esp_timer_get_time();
        lv_display_t* display = lv_display_get_default();
        const uint32_t display_w = (display != nullptr) ? lv_display_get_horizontal_resolution(display) : 0;
        const uint32_t display_h = (display != nullptr) ? lv_display_get_vertical_resolution(display) : 0;

        const uint8_t* source = nullptr;
        uint32_t source_stride = 0;
        lv_draw_buf_t* snapshot = nullptr;

        uint8_t* shadow = nullptr;
        uint32_t shadow_w = 0;
        uint32_t shadow_h = 0;
        size_t shadow_stride = 0;

        g_grab_stats.resolution_w = display_w;
        g_grab_stats.resolution_h = display_h;

        if (display != nullptr &&
            lvgl_display_get_shadow_frame(display, &shadow, &shadow_w, &shadow_h, &shadow_stride)) {
            // Preferred path: the display keeps a complete frame up to date as it flushes regions,
            // so this is a copy rather than a widget-tree re-render (the difference between ~70 ms
            // and ~350 ms at 480x320).
            w = shadow_w;
            h = shadow_h;
            source = shadow;
            source_stride = static_cast<uint32_t>(shadow_stride);
            used_shadow_frame = true;
        } else if ((snapshot = lv_snapshot_take(lv_scr_act(), LV_COLOR_FORMAT_RGB565)) != nullptr) {
            // Fallback until the shadow frame is complete (or on displays it isn't kept for).
            w = snapshot->header.w;
            h = snapshot->header.h;
            source = snapshot->data;
            source_stride = snapshot->header.stride;
        }

        if (source != nullptr && w != 0 && h != 0) {
            const size_t rgb_size = static_cast<size_t>(w) * h * 2;
            // Generous output capacity, matching encode_jpeg()'s rule of thumb.
            const size_t jpeg_need = std::max<size_t>(rgb_size, 65536);

            if (rgb565_capacity < rgb_size) {
                if (rgb565_buf != nullptr) heap_caps_free(rgb565_buf);
                // 16-byte aligned: the encoder documents its input buffer as needing that, and this
                // is the buffer it reads.
                rgb565_buf = static_cast<uint8_t*>(
                    heap_caps_aligned_alloc(16, rgb_size, MALLOC_CAP_SPIRAM));
                rgb565_capacity = (rgb565_buf != nullptr) ? rgb_size : 0;
            }
            if (jpeg_capacity < jpeg_need) {
                if (jpeg_buf != nullptr) heap_caps_free(jpeg_buf);
                jpeg_buf = static_cast<uint8_t*>(heap_caps_malloc(jpeg_need, MALLOC_CAP_SPIRAM));
                jpeg_capacity = (jpeg_buf != nullptr) ? jpeg_need : 0;
            }

            if (rgb565_buf != nullptr && jpeg_buf != nullptr) {
                // One pass out of the shadow frame: copy, exchange red/blue, and box-average down
                // to the requested scale. w/h become the output size the encoder will see, and the
                // buffer above is sized for the full frame, so it always fits.
                copy_scale_swap_rgb565(rgb565_buf, source, source_stride, w, h,
                                       (scale > 0) ? static_cast<uint32_t>(scale) : 1u, &w, &h);
                captured = true;
            }
        }
        t_copy_done = esp_timer_get_time();

        if (snapshot != nullptr) {
            lv_draw_buf_destroy(snapshot);
        }
        lvgl_unlock();
        t_capture_done = esp_timer_get_time();
    }

    if (captured) {
        // The capture pass already exchanged red/blue and applied any downscale, so the encoder
        // reads rgb565_buf as a finished standard-RGB565 frame at w x h.
        t_swap_done = esp_timer_get_time();

        const size_t produced = encode_jpeg_rgb565(rgb565_buf, w, h, quality, jpeg_buf, jpeg_capacity);
        if (produced > 0) {
            if (out_data != nullptr) *out_data = jpeg_buf;
            if (out_size != nullptr) *out_size = produced;
            if (out_width != nullptr) *out_width = w;
            if (out_height != nullptr) *out_height = h;
            ok = true;
        }
    }
    // Publish the breakdown for /api/sysinfo instead of logging it: a per-frame LOG_I backs the
    // UART up under a slow reader, and a blocked log call can stall the very path being measured.
    {
        const int64_t t_end = esp_timer_get_time();
        g_grab_stats.capture_ms = static_cast<uint32_t>((t_capture_done - t_start) / 1000);
        // Only meaningful when the lock was actually taken (otherwise both stamps are still 0).
        g_grab_stats.lock_wait_ms = (t_lock_acquired > 0)
            ? static_cast<uint32_t>((t_lock_acquired - t_start) / 1000) : 0;
        g_grab_stats.copy_ms = (t_lock_acquired > 0 && t_copy_done > 0)
            ? static_cast<uint32_t>((t_copy_done - t_lock_acquired) / 1000) : 0;
        // The channel exchange is folded into the copy, so it no longer has a phase of its own and
        // this stays 0 for the paths that use it. It is kept in the struct so existing readers of
        // /api/sysinfo do not have to change.
        g_grab_stats.swap_ms = 0;
        g_grab_stats.encode_ms = static_cast<uint32_t>((t_end - t_swap_done) / 1000);
        g_grab_stats.encode_open_ms = g_last_encode_open_us / 1000;
        g_grab_stats.used_shadow_frame = used_shadow_frame;
        g_grab_stats.quality = static_cast<uint32_t>(quality);
        g_grab_stats.scale = static_cast<uint32_t>(scale);
        g_grab_stats.output_w = w;
        g_grab_stats.output_h = h;
        if (ok) {
            g_grab_stats.frames++;
            g_grab_stats.full_frames++;
        }
    }

    xSemaphoreGive(g_grab_mutex);
    return ok;
}

TtVideoFrameKind tt_video_grab_delta(const uint8_t** out_data, size_t* out_size, uint32_t* out_x, uint32_t* out_y, uint32_t* out_width, uint32_t* out_height) {
    if (!ensureGrabMutex()) {
        return TT_VIDEO_FRAME_FULL;
    }
    if (xSemaphoreTake(g_grab_mutex, portMAX_DELAY) != pdTRUE) {
        return TT_VIDEO_FRAME_FULL;
    }

    TtVideoFrameKind kind = TT_VIDEO_FRAME_FULL;
    uint32_t region_x = 0;
    uint32_t region_y = 0;
    uint32_t region_w = 0;
    uint32_t region_h = 0;

    if (lvgl_try_lock(pdMS_TO_TICKS(200))) {
        lv_display_t* display = lv_display_get_default();
        uint8_t* shadow = nullptr;
        uint32_t shadow_w = 0;
        uint32_t shadow_h = 0;
        size_t shadow_stride = 0;
        lv_area_t area = {};

        if (display == nullptr ||
            !lvgl_display_get_shadow_frame(display, &shadow, &shadow_w, &shadow_h, &shadow_stride)) {
            // No complete frame to diff against: the caller sends a full JPEG.
            kind = TT_VIDEO_FRAME_FULL;
        } else if (!lvgl_display_take_dirty_area(display, &area)) {
            kind = TT_VIDEO_FRAME_NONE;
        } else {
            const uint32_t x = static_cast<uint32_t>(area.x1);
            const uint32_t y = static_cast<uint32_t>(area.y1);
            const uint32_t w = static_cast<uint32_t>(area.x2 - area.x1 + 1);
            const uint32_t h = static_cast<uint32_t>(area.y2 - area.y1 + 1);
            const size_t bytes = static_cast<size_t>(w) * h * 2;
            const size_t frame_bytes = static_cast<size_t>(shadow_w) * shadow_h * 2;

            // A change that covers the whole screen is not a region at all: the caller's full-frame
            // path already handles it, and going through a region copy first would just add a pass.
            if (bytes < frame_bytes && ensureDeltaCapacity(bytes)) {
                // Copy and exchange in one pass. The source is handed over already offset to the
                // region's first pixel, so the alignment check inside covers the x offset too. The
                // shadow holds the panel's channel order, so this also produces the standard RGB565
                // the client expects - same as the full-frame path.
                copy_swap_rgb565(g_delta_buf,
                                 shadow + static_cast<size_t>(y) * shadow_stride + static_cast<size_t>(x) * 2,
                                 shadow_stride,
                                 w,
                                 h);

                region_x = x;
                region_y = y;
                region_w = w;
                region_h = h;

                // Small regions are cheaper raw: no encode at all, and the pixels cost less than
                // the framing a JPEG would add. Anything bigger is compressed.
                kind = (bytes * DELTA_MAX_AREA_DIVISOR <= frame_bytes)
                    ? TT_VIDEO_FRAME_DELTA
                    : TT_VIDEO_FRAME_DELTA_JPEG;
            }
        }

        lvgl_unlock();
    }

    size_t payload_size = 0;
    const uint8_t* payload = g_delta_buf;

    if (kind == TT_VIDEO_FRAME_DELTA) {
        payload_size = static_cast<size_t>(region_w) * region_h * 2;
        g_grab_stats.delta_frames++;
    } else if (kind == TT_VIDEO_FRAME_DELTA_JPEG) {
        // Encoded after the LVGL lock is released: the encode is CPU-bound and takes long enough
        // (tens of ms even for a part of the screen) that holding the UI lock across it would stall
        // rendering - the same reason the full-frame path encodes outside the lock.
        const size_t need = regionJpegNeed(static_cast<size_t>(region_w) * region_h * 2);
        uint8_t* jpeg_out = ensureRegionJpegCapacity(need);
        payload_size = (jpeg_out != nullptr)
            ? encode_jpeg_rgb565(g_delta_buf, region_w, region_h, kRegionJpegQuality, jpeg_out, need)
            : 0;
        if (payload_size > 0) {
            payload = jpeg_out;
            g_grab_stats.region_jpeg_frames++;
        } else {
            // Encoding failed: let the caller fall back to a whole frame.
            kind = TT_VIDEO_FRAME_FULL;
        }
    } else if (kind == TT_VIDEO_FRAME_NONE) {
        g_grab_stats.same_frames++;
    }

    if (kind == TT_VIDEO_FRAME_DELTA || kind == TT_VIDEO_FRAME_DELTA_JPEG) {
        if (out_data != nullptr) *out_data = payload;
        if (out_size != nullptr) *out_size = payload_size;
        if (out_x != nullptr) *out_x = region_x;
        if (out_y != nullptr) *out_y = region_y;
        if (out_width != nullptr) *out_width = region_w;
        if (out_height != nullptr) *out_height = region_h;
    }

    xSemaphoreGive(g_grab_mutex);
    return kind;
}

void tt_video_get_grab_stats(struct TtVideoGrabStats* out) {
    if (out != nullptr) {
        *out = g_grab_stats;
    }
}
