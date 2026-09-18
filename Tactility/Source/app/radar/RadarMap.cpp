// SPDX-License-Identifier: Apache-2.0
#include "RadarMap.h"

#include <tactility/concurrent/thread.h>
#include <Tactility/network/HttpClient.h>

#include <tactility/log.h>

// LVGL's image decoder, which is the PNG decoder this build already has (lodepng, vendored). It is
// reached through the decoder API rather than by calling lodepng directly because that vendored
// header is C-only in practice: its extern "C" block runs past the C++ overloads, so including it
// from C++ fails to compile.
//
// The descriptor type the open call takes is defined in LVGL's private decoder header while the call
// itself is public - the public header only names it by pointer - so to hold one at all this has to
// come along.
#include <lvgl/lvgl.h>
#include <draw/lv_image_decoder_private.h>

#include <algorithm>
#include <atomic>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <format>
#include <memory>
#include <string>
#include <vector>

namespace tt::app::radar {

namespace {

constexpr auto* TAG = "RadarMap";

constexpr auto* BASEMAP_URL = "https://basemap.nationalmap.gov/arcgis/rest/services/USGSTopo/MapServer/export";
constexpr auto* WMS_URL = "https://opengeo.ncep.noaa.gov/geoserver/ows";

/**
 * The radar layer on its own, and the boundaries on their own, rather than all three in one request.
 *
 * The boundaries are the same picture in all six frames, so asking for them six times is asking the
 * server to rasterise the same counties over and over and then sending the result again: measured, one
 * frame of the combined request was 16,400 bytes against 2,457 for the radar alone. Splitting them out
 * means the boundaries are fetched once, blended into the base map once, and each frame carries only
 * what changed - which also makes the per-frame request a fraction of the server's work.
 */
constexpr auto* WMS_RADAR_LAYERS = "conus:conus_bref_qcd";
constexpr auto* WMS_BOUNDARY_LAYERS = "nws:us_counties,nws:state_boundary";

/** The radar layer publishes two-minute steps; this is how far apart the frames are asked for. */
constexpr int32_t FRAME_INTERVAL_SECONDS = 120;

/** Minutes to look back for the newest frame: the mosaic is published a few minutes behind the clock,
 *  and the layer snaps to the nearest step it has (`nearestValue`), so being slightly early is fine. */
constexpr int32_t NEWEST_FRAME_LAG_SECONDS = 180;

/** A frame is around 30 KB and the base map around 190 KB, but a busy radar mosaic is bigger. */
constexpr size_t MAX_BASEMAP_BYTES = 4 * 1024 * 1024;
constexpr size_t MAX_FRAME_BYTES = 4 * 1024 * 1024;

/** How long one request may take before it is given up on. Short, because a series is many requests
 *  and one that is never going to finish must not hold the screen up behind it. */
constexpr int32_t FETCH_TIMEOUT_MS = 15000;

/**
 * How long the whole series may take. Whatever has arrived by then is what gets played: six frames
 * that are all here beat a seventh that is not, and a minute of "Loading map..." is worse than an
 * animation that starts missing a frame.
 */
constexpr int32_t SERIES_BUDGET_MS = 30000;

/**
 * How many frames are fetched at once, counting this task.
 *
 * The frames are independent requests - six separate WMS renders - so fetching them one after another
 * spends the whole time waiting: measured at about four seconds a frame, of which almost none was
 * transfer. Two means one worker thread beside this task, which is the useful number rather than six:
 * each request in flight holds its own TLS session out of internal RAM, and every extra worker needs
 * a task stack from the same place, of which this board has less than a hundred kilobytes to spare.
 */
constexpr int32_t FRAME_WORKERS = 2;

/**
 * Stack for the frame worker.
 *
 * 8 KB is not a guess: this app's own task runs on exactly that and does the same request, decode and
 * blend one after the other. Asking for more is what broke this the first time - a second large stack
 * had to be allocated contiguously out of internal RAM, and could not be.
 */
constexpr size_t FRAME_WORKER_STACK = 8192;

/** Web mercator, the projection both services are asked for, so a bbox means the same to each. */
constexpr double EARTH_RADIUS = 6378137.0;
constexpr double MAX_MERCATOR = 20037508.342789244;
constexpr double METRES_PER_PIXEL_AT_ZOOM_0 = 156543.033928041;

double longitudeToMercatorX(double longitude) {
    return longitude * MAX_MERCATOR / 180.0;
}

double latitudeToMercatorY(double latitude) {
    const double clamped = std::clamp(latitude, -85.05112878, 85.05112878);
    return std::log(std::tan((90.0 + clamped) * M_PI / 360.0)) / (M_PI / 180.0) * MAX_MERCATOR / 180.0;
}

/** Metres per pixel for a zoom level at a latitude, which is what makes the bbox the right size. */
double resolutionFor(int32_t zoom, double latitude) {
    return METRES_PER_PIXEL_AT_ZOOM_0 * std::cos(latitude * M_PI / 180.0) / std::pow(2.0, zoom);
}

std::string formatBoundingBox(const MapView& view) {
    const double centreX = longitudeToMercatorX(view.longitude);
    const double centreY = latitudeToMercatorY(view.latitude);
    const double resolution = resolutionFor(view.zoom, view.latitude);
    const double halfWidth = view.width / 2.0 * resolution;
    const double halfHeight = view.height / 2.0 * resolution;

    return std::format(
        "{:.0f},{:.0f},{:.0f},{:.0f}",
        centreX - halfWidth,
        centreY - halfHeight,
        centreX + halfWidth,
        centreY + halfHeight
    );
}

std::string formatTime(long long unixSeconds) {
    const auto seconds = static_cast<time_t>(unixSeconds);
    std::tm utc {};
    gmtime_r(&seconds, &utc);
    char buffer[32] = {};
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%dT%H:%M:%SZ", &utc);
    return buffer;
}

std::string basemapUrl(const MapView& view) {
    // png8 rather than png32: a topographic map has few enough colours that 256 of them look the
    // same on a 480x231 panel, and it is 87 KB instead of 296 KB - the single largest thing this
    // screen downloads, and the one download it cannot overlap with anything.
    return std::format(
        "{}?bbox={}&bboxSR=3857&imageSR=3857&size={},{}&format=png8&f=image",
        BASEMAP_URL,
        formatBoundingBox(view),
        view.width,
        view.height
    );
}

/** The radar layer for one moment, transparent where there is no echo. */
std::string radarUrl(const MapView& view, const std::string& time) {
    // png8 here too: this layer is a palette of about fifteen reflectivity colours drawn on nothing,
    // so an indexed PNG is a fraction of the size - measured, 238 bytes against 2,457 for one frame.
    return std::format(
        "{}?service=WMS&version=1.3.0&request=GetMap&layers={}&crs=EPSG:3857&bbox={}"
        "&width={}&height={}&format=image/png8&transparent=true&time={}",
        WMS_URL,
        WMS_RADAR_LAYERS,
        formatBoundingBox(view),
        view.width,
        view.height,
        time
    );
}

/**
 * The county and state lines, which do not change between frames and are therefore fetched once.
 *
 * No time parameter: these are map furniture rather than observation, and asking for them with a time
 * would be asking the server to look up a step that has nothing to do with them.
 */
std::string boundaryUrl(const MapView& view) {
    return std::format(
        "{}?service=WMS&version=1.3.0&request=GetMap&layers={}&crs=EPSG:3857&bbox={}"
        "&width={}&height={}&format=image/png8&transparent=true",
        WMS_URL,
        WMS_BOUNDARY_LAYERS,
        formatBoundingBox(view),
        view.width,
        view.height
    );
}

/**
 * A decoded image, released again when it goes out of scope. ARGB8888 with straight (not
 * premultiplied) alpha, which is the decoder's default and is what makes ordinary alpha blending
 * against the base map correct.
 */
struct DecodedImage {
    const uint8_t* pixels = nullptr;
    uint32_t width = 0;
    uint32_t height = 0;
    uint32_t stride = 0;
    lv_image_decoder_dsc_t decoder {};
    lv_image_dsc_t source {};

    ~DecodedImage() {
        if (pixels != nullptr) {
            // The decoder caches what it decoded; these frames exist to be consumed and dropped, so
            // the cache entry is dropped with them rather than held until it is evicted.
            lv_image_cache_drop(&source);
            lv_image_decoder_close(&decoder);
        }
    }

    DecodedImage() = default;
    DecodedImage(const DecodedImage&) = delete;
    DecodedImage& operator=(const DecodedImage&) = delete;
};

bool decodePng(const uint8_t* data, size_t size, DecodedImage& outImage, std::string& outError) {
    outImage.source.header.magic = LV_IMAGE_HEADER_MAGIC;
    outImage.source.data = data;
    outImage.source.data_size = static_cast<uint32_t>(size);

    // no_cache: these images are decoded once and consumed, and they are larger than the cache this
    // build configures (LV_CACHE_DEF_SIZE is 256 KB; a decoded 480x231 frame is 443 KB), so asking
    // for the cache is asking for the one thing it cannot give.
    lv_image_decoder_args_t args {};
    args.no_cache = true;

    if (lv_image_decoder_open(&outImage.decoder, &outImage.source, &args) != LV_RESULT_OK) {
        lv_image_header_t header {};
        const lv_result_t info = lv_image_decoder_get_info(&outImage.source, &header);
        outError = std::format(
            "image did not decode (header lookup {}, {} bytes)",
            static_cast<int>(info),
            static_cast<unsigned>(size)
        );
        return false;
    }

    const lv_draw_buf_t* decoded = outImage.decoder.decoded;
    if (decoded == nullptr || decoded->header.cf != LV_COLOR_FORMAT_ARGB8888) {
        outError = std::format(
            "image decoded as colour format {}",
            static_cast<int>(decoded != nullptr ? decoded->header.cf : -1)
        );
        return false;
    }

    outImage.pixels = decoded->data;
    outImage.width = decoded->header.w;
    outImage.height = decoded->header.h;
    outImage.stride = decoded->header.stride;
    return true;
}

/** One pixel of RGB565, which is what the screen wants and half the memory of RGBA. */
uint16_t toRgb565(unsigned char red, unsigned char green, unsigned char blue) {
    return static_cast<uint16_t>(
        (static_cast<uint16_t>(red >> 3) << 11) |
        (static_cast<uint16_t>(green >> 2) << 5) |
        static_cast<uint16_t>(blue >> 3)
    );
}

/**
 * Writes @a overlay over @a base into @a destination as RGB565.
 *
 * The overlay is the radar layer, which is transparent where there is no echo, so this is not a
 * copy: alpha decides how much of the base map shows through, and the result is a frame that can be
 * displayed with no blending at draw time.
 *
 * Used once per series, to blend the boundaries into the base map: the result is what every frame is
 * then drawn over, which is why the per-frame pass below does not have to read the base map's alpha.
 */
void flatten(const DecodedImage& base, const DecodedImage& overlay, uint8_t* destination) {
    auto* out = reinterpret_cast<uint16_t*>(destination);

    for (uint32_t y = 0; y < base.height; y++) {
        const uint8_t* baseRow = base.pixels + (y * base.stride);
        const uint8_t* overlayRow = overlay.pixels + (y * overlay.stride);
        uint16_t* outRow = out + (y * base.width);

        for (uint32_t x = 0; x < base.width; x++) {
            const uint32_t offset = x * 4;
            const unsigned alpha = overlayRow[offset + 3];

            if (alpha == 0) {
                outRow[x] = toRgb565(baseRow[offset], baseRow[offset + 1], baseRow[offset + 2]);
            } else if (alpha == 255) {
                outRow[x] = toRgb565(overlayRow[offset], overlayRow[offset + 1], overlayRow[offset + 2]);
            } else {
                // 8-bit alpha over an 8-bit channel; the radar styles rely on this being partial.
                const auto blend = [alpha](unsigned char under, unsigned char over) {
                    return static_cast<unsigned char>((over * alpha + under * (255 - alpha)) / 255);
                };
                outRow[x] = toRgb565(
                    blend(baseRow[offset], overlayRow[offset]),
                    blend(baseRow[offset + 1], overlayRow[offset + 1]),
                    blend(baseRow[offset + 2], overlayRow[offset + 2])
                );
            }
        }
    }
}

/** The base map on its own, for when the boundary layer could not be had. */
void flattenBase(const DecodedImage& base, uint8_t* destination) {
    auto* out = reinterpret_cast<uint16_t*>(destination);
    for (uint32_t y = 0; y < base.height; y++) {
        const uint8_t* baseRow = base.pixels + (y * base.stride);
        uint16_t* outRow = out + (y * base.width);
        for (uint32_t x = 0; x < base.width; x++) {
            outRow[x] = toRgb565(baseRow[x * 4], baseRow[x * 4 + 1], baseRow[x * 4 + 2]);
        }
    }
}

/** One pixel of the radar over one pixel of an RGB565 base. */
uint16_t blendPixel(uint16_t under, const uint8_t* over, unsigned alpha) {
    if (alpha == 255) {
        return toRgb565(over[0], over[1], over[2]);
    }

    // The base is unpacked to eight bits, blended, and packed again. That round trip costs a little
    // precision that the base map's own colours do not notice - and it is the only way to blend
    // against a frame that is stored as RGB565, which is half the memory to read back per frame.
    const auto underRed = static_cast<unsigned char>(((under >> 11) & 0x1F) * 255 / 31);
    const auto underGreen = static_cast<unsigned char>(((under >> 5) & 0x3F) * 255 / 63);
    const auto underBlue = static_cast<unsigned char>((under & 0x1F) * 255 / 31);
    const auto blend = [alpha](unsigned char below, unsigned char above) {
        return static_cast<unsigned char>((above * alpha + below * (255 - alpha)) / 255);
    };
    return toRgb565(
        blend(underRed, over[0]),
        blend(underGreen, over[1]),
        blend(underBlue, over[2])
    );
}

/**
 * Draws the radar layer over a base that is already RGB565, in place.
 *
 * This is the pass that runs once per frame, so it is the one worth being careful with. The base is
 * read as two-byte pixels rather than four, and a row the radar does not touch - which is most of a
 * calm map - is handed to memcpy, which moves it in words. What is left of the cost is the blending
 * itself, and on this board that is dominated by the PSRAM round trip rather than the arithmetic.
 */
void blendOver(uint8_t* frame, const uint8_t* base, const DecodedImage& overlay) {
    auto* out = reinterpret_cast<uint16_t*>(frame);
    const auto* under = reinterpret_cast<const uint16_t*>(base);
    const uint32_t width = overlay.width;

    for (uint32_t y = 0; y < overlay.height; y++) {
        const uint8_t* overlayRow = overlay.pixels + (y * overlay.stride);
        const uint16_t* baseRow = under + (static_cast<size_t>(y) * width);
        uint16_t* outRow = out + (static_cast<size_t>(y) * width);

        bool touched = false;
        for (uint32_t x = 0; x < width; x++) {
            if (overlayRow[x * 4 + 3] != 0) {
                touched = true;
                break;
            }
        }
        if (!touched) {
            std::memcpy(outRow, baseRow, static_cast<size_t>(width) * 2);
            continue;
        }

        for (uint32_t x = 0; x < width; x++) {
            const unsigned alpha = overlayRow[x * 4 + 3];
            outRow[x] = alpha == 0 ? baseRow[x] : blendPixel(baseRow[x], overlayRow + (x * 4), alpha);
        }
    }
}

} // namespace

/**
 * Everything one frame worker needs. The frames are independent requests, so the work is claimed
 * from a shared counter rather than handed out in advance: a worker that finishes early takes the
 * next one, and each writes into its own slot, which is what keeps the series in time order however
 * the requests interleave.
 */
struct FrameWork {
    const MapView* view = nullptr;
    /** The base map with the boundaries already blended in, as RGB565: what every frame is drawn over. */
    const uint8_t* composite = nullptr;
    std::vector<uint8_t>* pixels = nullptr;
    size_t framePixels = 0;
    long long now = 0;
    std::atomic<int32_t>* nextIndex = nullptr;
    std::atomic<int32_t>* producedCount = nullptr;
    std::atomic<bool>* succeeded = nullptr;
    /** One connection for this slot's frames, instead of a TLS handshake per frame. */
    tt::network::HttpSession* session = nullptr;
    /** The series being filled, so a finished slot can be published the moment it is ready. */
    RadarFrames* series = nullptr;
    RadarFrames::FrameReady onFrameReady = nullptr;
    void* userData = nullptr;
    /** Tick after which no new frame is started; frames already in flight are still waited for. */
    TickType_t deadline = 0;
};

/** Fetches, decodes and blends frames until there are none left to claim. */
int32_t runFrameWork(FrameWork& work) {
    // One body for the whole series, not one per frame: the client empties it in place before each
    // use, so its PSRAM buffer is allocated once and reused for every frame.
    tt::network::HttpBody frameData;

    while (true) {
        // Checked between frames rather than during one, so the wait is bounded by the budget plus a
        // single request's timeout rather than by the number of frames left.
        if (work.deadline != 0 && xTaskGetTickCount() > work.deadline) {
            LOG_W(
                TAG,
                "Time is up with %d frame(s) in hand; starting no more",
                static_cast<int>(work.producedCount->load())
            );
            break;
        }

        const int32_t index = work.nextIndex->fetch_add(1);
        if (index >= RadarFrames::FRAME_COUNT) {
            break;
        }

        // Oldest first, so playback runs forward in time like the loop it replaces.
        const int32_t ageSeconds = NEWEST_FRAME_LAG_SECONDS +
            (RadarFrames::FRAME_COUNT - 1 - index) * FRAME_INTERVAL_SECONDS;
        const std::string time = formatTime(work.now - ageSeconds);

        std::string frameError;
        if (!work.session->get(
                radarUrl(*work.view, time),
                frameData,
                frameError,
                FETCH_TIMEOUT_MS,
                MAX_FRAME_BYTES
            )) {
            LOG_W(TAG, "Frame %s failed: %s", time.c_str(), frameError.c_str());
            continue;
        }

        DecodedImage overlay;
        bool decoded = false;
        {
            // LVGL is not thread safe, so the decode happens under the graphics lock. The blending
            // below is arithmetic on two buffers and needs nothing.
            lvgl_lock();
            decoded = decodePng(frameData.data(), frameData.size(), overlay, frameError);
            lvgl_unlock();
        }
        if (!decoded) {
            LOG_W(TAG, "Frame %s did not decode: %s", time.c_str(), frameError.c_str());
            continue;
        }
        if (static_cast<int32_t>(overlay.width) != work.view->width ||
            static_cast<int32_t>(overlay.height) != work.view->height) {
            LOG_W(TAG, "Frame %s is %ux%u", time.c_str(), overlay.width, overlay.height);
            continue;
        }

        uint8_t* slot = work.pixels->data() + (static_cast<size_t>(index) * work.framePixels);
        blendOver(slot, work.composite, overlay);

        // The slot is filled *before* its flag is raised, which is what lets the screen show a frame
        // without waiting for the rest: a reader that sees the flag is guaranteed to see the pixels.
        work.series->frames[index] = MapFrame {
            .pixels = slot,
            .width = work.view->width,
            .height = work.view->height,
            .epochSeconds = work.now - ageSeconds
        };
        work.succeeded[index].store(true);
        work.producedCount->fetch_add(1);

        if (work.onFrameReady != nullptr) {
            work.onFrameReady(work.userData, *work.series, work.succeeded);
        }
        LOG_I(
            TAG,
            "Frame %d at %s: %u bytes",
            static_cast<int>(index),
            time.c_str(),
            static_cast<unsigned>(frameData.size())
        );
    }
    return 0;
}

int32_t frameWorkMain(void* context) {
    return runFrameWork(*static_cast<FrameWork*>(context));
}

void RadarFrames::clear() {
    frames.clear();
    pixels.clear();
    description.clear();
    zoom = 0;
}

/**
 * The last base map with its boundaries blended in, kept between series.
 *
 * It is kept because it is the slow half of opening this screen: the USGS base map measured 3.4
 * seconds, against a fraction of a second for each radar frame once the boundaries stopped being
 * fetched with them. It depends only on the bounding box and the pixel size, so the same entry
 * answers re-opening the radar at the same zoom and zooming back to a level already seen. The cost
 * is 222 KB of PSRAM held for as long as the app is loaded, out of eight megabytes.
 */
struct StaticLayers {
    std::string key;
    std::vector<uint8_t> composite;
};

StaticLayers& staticLayers() {
    static StaticLayers layers;
    return layers;
}

bool RadarFrames::fetch(
    const MapView& view,
    const std::string& station,
    std::string& outError,
    FrameReady onFrameReady,
    void* userData
) {
    clear();

    const size_t framePixels = static_cast<size_t>(view.width) * view.height * 2;
    pixels.assign(framePixels * FRAME_COUNT, 0);

    // One entry per time slot from the start, so a slot's position is its place in time and a frame
    // that never arrives leaves a hole rather than shifting the ones after it. The screen reads this
    // while it is being filled.
    frames.assign(FRAME_COUNT, MapFrame {});

    // The static half of every frame: the base map, with the boundaries drawn over it once. Frames
    // are then drawn over the result, which is why the boundaries are no longer part of their request.
    std::vector<uint8_t> composite(framePixels);
    const std::string staticKey = basemapUrl(view);
    if (staticLayers().key == staticKey && staticLayers().composite.size() == framePixels) {
        composite = staticLayers().composite;
        LOG_I(TAG, "Base map and boundaries kept from the last time this view was fetched");
    } else {
        // The base map is the largest single image in the system - 87 KB as png8 - so its body is the
        // one that has to come out of PSRAM rather than internal RAM.
        tt::network::HttpBody basemapData;
        if (!tt::network::httpGet(basemapUrl(view), basemapData, outError, FETCH_TIMEOUT_MS, MAX_BASEMAP_BYTES)) {
            LOG_W(TAG, "Base map failed: %s", outError.c_str());
            return false;
        }
        LOG_I(TAG, "Base map: %u bytes", static_cast<unsigned>(basemapData.size()));

        DecodedImage basemap;
        if (!decodePng(basemapData.data(), basemapData.size(), basemap, outError)) {
            LOG_W(TAG, "Base map did not decode: %s", outError.c_str());
            return false;
        }
        if (static_cast<int32_t>(basemap.width) != view.width || static_cast<int32_t>(basemap.height) != view.height) {
            outError = std::format("base map is {}x{}", basemap.width, basemap.height);
            LOG_W(TAG, "%s", outError.c_str());
            return false;
        }

        // The boundaries, once for the whole series rather than once per frame.
        tt::network::HttpBody boundaryData;
        std::string boundaryError;
        DecodedImage boundaries;
        const bool haveBoundaries =
            tt::network::httpGet(boundaryUrl(view), boundaryData, boundaryError, FETCH_TIMEOUT_MS, MAX_BASEMAP_BYTES) &&
            decodePng(boundaryData.data(), boundaryData.size(), boundaries, boundaryError) &&
            boundaries.width == basemap.width &&
            boundaries.height == basemap.height;

        if (haveBoundaries) {
            LOG_I(TAG, "Boundaries: %u bytes", static_cast<unsigned>(boundaryData.size()));
            flatten(basemap, boundaries, composite.data());
        } else {
            // Lines missing off a map are not a reason to show no map, so this is not fatal.
            LOG_W(TAG, "Boundaries unavailable (%s); drawing the base map alone", boundaryError.c_str());
            flattenBase(basemap, composite.data());
        }

        staticLayers().key = staticKey;
        staticLayers().composite = composite;
    }

    const long long now = static_cast<long long>(std::time(nullptr));

    // Frames are claimed from a shared counter rather than handed out in advance, so a worker that
    // finishes early takes the next one instead of idling, and each writes into its own slot - which
    // is what keeps the series in time order however the requests interleave.
    std::atomic<int32_t> nextIndex { 0 };
    std::atomic<int32_t> producedCount { 0 };
    std::atomic<bool> succeeded[FRAME_COUNT] {};


    // One session per slot, so two connections rather than one per frame: each worker keeps its
    // connection open and issues its frames over it, which is where the handshakes stop being paid.
    tt::network::HttpSession callerSession;
    tt::network::HttpSession workerSession;

    FrameWork work {
        .view = &view,
        .composite = composite.data(),
        .pixels = &pixels,
        .framePixels = framePixels,
        .now = now,
        .nextIndex = &nextIndex,
        .producedCount = &producedCount,
        .succeeded = succeeded,
        .session = &callerSession,
        .series = this,
        .onFrameReady = onFrameReady,
        .userData = userData,
        .deadline = xTaskGetTickCount() + pdMS_TO_TICKS(SERIES_BUDGET_MS)
    };

    FrameWork workerWork = work;
    workerWork.session = &workerSession;

    // One worker thread, and this task is the other worker: two requests in flight, one of which
    // costs no memory at all, which is what makes this affordable - a second task stack has to come
    // out of the internal RAM this board is short of.
    //
    // The kernel thread API rather than tt::Thread, deliberately: Thread::start() asserts when its
    // task cannot be created, so an out-of-memory moment there is a panic. This one returns an error,
    // and a worker that will not start just means the frames are fetched one at a time again.
    Thread* frameThread = thread_alloc_full(
        "radar-frame",
        FRAME_WORKER_STACK,
        frameWorkMain,
        &workerWork,
        tskNO_AFFINITY
    );
    if (frameThread != nullptr) {
        thread_set_priority(frameThread, THREAD_PRIORITY_NORMAL);
        if (thread_start(frameThread) != ERROR_NONE) {
            LOG_W(TAG, "The frame worker would not start; fetching on this task alone");
            thread_free(frameThread);
            frameThread = nullptr;
        }
    } else {
        LOG_W(TAG, "No room for a frame worker; fetching on this task alone");
    }

    runFrameWork(work);

    if (frameThread != nullptr) {
        thread_join(frameThread, portMAX_DELAY, 10);
        thread_free(frameThread);
    }

    const int32_t produced = producedCount.load();
    if (produced == 0) {
        outError = "no radar frames arrived";
        return false;
    }

    // The slots filled as they arrived and are left where they are: their index is their place in
    // time, and the screen has already been publishing each one as it landed.
    description = std::format(
        "{} zoom {}",
        station.empty() ? std::string("radar") : station,
        static_cast<int>(view.zoom)
    );
    zoom = view.zoom;
    LOG_I(TAG, "Map ready: %d of %d frames, %s", static_cast<int>(readyCount()), static_cast<int>(FRAME_COUNT), description.c_str());
    return true;
}

} // namespace tt::app::radar
