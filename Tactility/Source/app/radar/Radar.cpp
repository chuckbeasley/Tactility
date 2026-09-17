// SPDX-License-Identifier: Apache-2.0
//
// The National Weather Service's radar imagery for one location, shown on the device.
//
// The imagery is a GIF: 600x550, refreshed every few minutes, with the map, the coverage, the
// legend and the observation time all baked into the pixels. There is no PNG or JPEG variant of the
// same product - radar.weather.gov answers 404 for those - so this screen is the reason LV_USE_GIF
// is enabled in lv_conf.h.
//
// Two products exist per radar site, and both are fetched from the same directory:
//
//   {SITE}_loop.gif  the last ten frames, animated, played by LVGL as the GIF says (200ms a frame)
//   {SITE}_0.gif     the most recent frame alone
//
// The loop is what this screen shows, and the single frame is the fallback for when it cannot be
// fetched: the loop is what makes a radar view worth looking at, because the echoes move in it.
//
// It is not free. Each frame is a full 600x550 canvas render plus a scaled blit while LVGL's lock is
// held, five times a second for as long as the screen is open; with the loop on screen a screenshot
// request measured nine seconds end to end against five for the still frame, most of it waiting for
// that lock. That is a diagnostic tool's cost, not a user's, so it is accepted here.
#include <Tactility/file/File.h>
#include <Tactility/lvgl/Lvgl.h>
#include <Tactility/network/HttpClient.h>

#include <app/event.h>
#include <app/manifest.h>
#include <app/paths.h>
#include <app/scheduler.h>

#include <lvgl/lvgl.h>
#include <lvgl/widgets/toolbar.h>
#include <lvgl_window_manager/window_manager.h>

#include <tactility/check.h>
#include <tactility/log.h>

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <format>
#include <string>

namespace tt::app::radar {

constexpr auto* TAG = "Radar";

extern const ::AppManifest manifest;

namespace {

constexpr auto* RADAR_BASE_URL = "https://radar.weather.gov/ridge/standard";
constexpr auto* IMAGE_FILE_NAME = "radar.gif";

/** The GIF header puts the logical screen size in little-endian at byte 6, which is read back so
 *  the image can be scaled from what it actually is rather than from a guess at the product size. */
constexpr size_t GIF_WIDTH_OFFSET = 6;
constexpr size_t GIF_HEIGHT_OFFSET = 8;
constexpr size_t GIF_HEADER_SIZE = 10;

struct Context {
    uint32_t appInstanceId = 0;
    std::string place;
    std::string station;

    // Widgets; null whenever this window has no live widget, which happens on suspend as well as on
    // close, so every update checks first.
    lv_obj_t* statusLabel = nullptr;
    lv_obj_t* imageHolder = nullptr;
    lv_obj_t* image = nullptr;

    bool hasWidgets() const {
        return statusLabel != nullptr;
    }

    void clearWidgets() {
        statusLabel = nullptr;
        imageHolder = nullptr;
        image = nullptr;
    }
};

std::string getImageFilePath() {
    char path[256] = {};
    if (app_paths_get_user_data_path(manifest.id, IMAGE_FILE_NAME, path, sizeof(path)) != ERROR_NONE) {
        return {};
    }
    return path;
}

void setStatus(Context* ctx, const std::string& text) {
    lvgl_lock();
    if (ctx->hasWidgets()) {
        lv_label_set_text(ctx->statusLabel, text.c_str());
    }
    lvgl_unlock();
}

/**
 * Downloads the site's animated loop into @a outData, falling back to the single most recent frame
 * when the loop cannot be fetched - the loop carries the history, so it is the one worth having.
 * @return true when an image arrived
 */
bool fetchImage(const std::string& station, std::string& outData, std::string& outError) {
    const std::string loopUrl = std::format("{}/{}_loop.gif", RADAR_BASE_URL, station);
    if (tt::network::httpGet(loopUrl, outData, outError)) {
        LOG_I(TAG, "Fetched %u bytes of radar loop", static_cast<unsigned>(outData.size()));
        return true;
    }
    LOG_W(TAG, "Radar loop failed: %s", outError.c_str());

    const std::string frameUrl = std::format("{}/{}_0.gif", RADAR_BASE_URL, station);
    outData.clear();
    if (tt::network::httpGet(frameUrl, outData, outError)) {
        LOG_I(TAG, "Fetched %u bytes of radar frame", static_cast<unsigned>(outData.size()));
        return true;
    }
    LOG_W(TAG, "Radar frame failed: %s", outError.c_str());
    return false;
}

/** Scales the image to fit the space it has, in whole 1/256ths as LVGL expects. */
void showImage(Context* ctx, const std::string& data) {
    if (data.size() < GIF_HEADER_SIZE) {
        setStatus(ctx, "The radar image was truncated");
        return;
    }

    const auto width = static_cast<int32_t>(static_cast<uint8_t>(data[GIF_WIDTH_OFFSET]) | (static_cast<uint8_t>(data[GIF_WIDTH_OFFSET + 1]) << 8));
    const auto height = static_cast<int32_t>(static_cast<uint8_t>(data[GIF_HEIGHT_OFFSET]) | (static_cast<uint8_t>(data[GIF_HEIGHT_OFFSET + 1]) << 8));
    if (width <= 0 || height <= 0) {
        setStatus(ctx, "The radar image has no size");
        return;
    }

    const std::string path = getImageFilePath();
    if (path.empty()) {
        setStatus(ctx, "Could not resolve where to store the radar image");
        return;
    }
    // The app's own directory does not exist until something creates it, and file::writeString()
    // does not create it either - without this the write fails on a freshly formatted /data.
    if (!file::findOrCreateParentDirectory(path, 0755)) {
        LOG_E(TAG, "Failed to create the directory for %s", path.c_str());
        setStatus(ctx, "Could not create the radar image directory");
        return;
    }
    if (!file::writeString(path, data)) {
        LOG_E(TAG, "Failed to write the radar image to %s", path.c_str());
        setStatus(ctx, "Could not store the radar image");
        return;
    }

    lvgl_lock();
    if (ctx->hasWidgets()) {
        // The caption is set first because it is one of the two things that decide how much space is
        // left: a caption that wraps to a second line takes a line away from the image, and measuring
        // before it is set is what made the image overflow the bottom of the screen.
        //
        // The observation time is part of the image itself, so there is nothing to report here beyond
        // which place and which site are on screen.
        const std::string caption = ctx->place.empty() ? ctx->station : std::format("{} - {}", ctx->place, ctx->station);
        lv_label_set_text(ctx->statusLabel, caption.c_str());

        // Measured on the holder rather than on the parent: the holder is the flex child that grows
        // into whatever the toolbar and the caption leave, so its size is the real remaining space.
        // A layout pass has to happen first - a flex container that has not been laid out yet reports
        // zero, which is how the first attempt came out with a scale of zero and no image.
        lv_obj_update_layout(ctx->imageHolder);
        const int32_t available_width = lv_obj_get_content_width(ctx->imageHolder);
        const int32_t available_height = lv_obj_get_content_height(ctx->imageHolder);

        // LVGL loads through its own filesystem layer, which addresses files by driver letter - the
        // same file needs "A:" in front of it for LVGL and no prefix for the C library. Written to
        // disk rather than kept in memory because the decoder reads it back incrementally, and
        // because the file is what makes a repeat visit instant.
        const std::string lvgl_path = std::string(tt::lvgl::PATH_PREFIX) + path;
        int32_t scale = std::min(
            (available_width * LV_SCALE_NONE) / width,
            (available_height * LV_SCALE_NONE) / height
        );
        scale = std::clamp<int32_t>(scale, 1, LV_SCALE_NONE);

        lv_gif_set_src(ctx->image, lvgl_path.c_str());
        lv_image_set_scale(ctx->image, scale);
        lv_obj_set_size(ctx->image, (width * scale) / LV_SCALE_NONE, (height * scale) / LV_SCALE_NONE);
        LOG_I(TAG, "Radar image %dx%d scaled to %d/256 in %dx%d", width, height, scale, available_width, available_height);
    }
    lvgl_unlock();
}

void loadRadar(Context* ctx) {
    if (ctx->station.empty()) {
        setStatus(ctx, "No radar site is published for this location");
        return;
    }

    setStatus(ctx, "Loading radar...");
    LOG_I(TAG, "Radar station %s", ctx->station.c_str());

    std::string data;
    std::string error;
    if (!fetchImage(ctx->station, data, error)) {
        setStatus(ctx, std::format("Could not load the radar: {}", error));
        return;
    }

    showImage(ctx, data);
}

void onBackPressed(lv_event_t* event) {
    auto* ctx = static_cast<Context*>(lv_event_get_user_data(event));
    app_event_emit_close(ctx->appInstanceId);
}

void createWidgets(lv_obj_t* parent, void* userData) {
    auto* ctx = static_cast<Context*>(userData);

    lv_obj_set_flex_flow(parent, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_row(parent, 0, LV_STATE_DEFAULT);

    const uint8_t margin = (lvgl_get_ui_density() == LVGL_UI_DENSITY_COMPACT) ? 2 : 8;

    auto* toolbar = lvgl_toolbar_create(parent, "Radar");
    lvgl_toolbar_set_nav_action(toolbar, LV_SYMBOL_CLOSE, onBackPressed, ctx);
    lv_obj_set_style_margin_bottom(toolbar, margin, LV_STATE_DEFAULT);

    // One line for the place and the site, then the image, which is centred in whatever is left.
    ctx->statusLabel = lv_label_create(parent);
    lv_obj_set_width(ctx->statusLabel, LV_PCT(100));
    lv_label_set_long_mode(ctx->statusLabel, LV_LABEL_LONG_WRAP);
    lv_label_set_text(ctx->statusLabel, ctx->place.empty() ? "Loading radar..." : ctx->place.c_str());

    // lv_gif, not lv_image: a GIF needs the GIF widget's own decoder, and a plain image widget has
    // no way to read one - which is how the first attempt produced a blank screen with no error.
    // It is an image widget underneath, so scaling it works the same way.
    //
    // The holder is what makes the image fit rather than roughly fit: it is the flex child that
    // absorbs the space the toolbar and the caption leave (flex_grow), and centring is left to it
    // because a flex parent positions its own children - an align call on the image is ignored.
    ctx->imageHolder = lv_obj_create(parent);
    lv_obj_set_width(ctx->imageHolder, LV_PCT(100));
    lv_obj_set_flex_grow(ctx->imageHolder, 1);
    lv_obj_set_style_pad_all(ctx->imageHolder, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ctx->imageHolder, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ctx->imageHolder, LV_OPA_TRANSP, LV_STATE_DEFAULT);
    lv_obj_set_flex_flow(ctx->imageHolder, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(ctx->imageHolder, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    ctx->image = lv_gif_create(ctx->imageHolder);
}

/**
 * Called when this window's widget is deleted, which happens on suspend as well as on close. The
 * pointers above go stale at that moment and the fetch is still running on this app's task, so they
 * have to be dropped here or the first update after a suspend walks freed widgets.
 */
void destroyWidgets(void* userData) {
    auto* ctx = static_cast<Context*>(userData);
    ctx->clearWidgets();
}

int32_t appMain(int argc, char* argv[]) {
    const uint32_t appInstanceId = app_scheduler_current_app_id();

    Context ctx {};
    ctx.appInstanceId = appInstanceId;

    // argv: [0] place name (may be empty), [1] latitude, [2] longitude, [3] radar site. Read
    // defensively: this app can also be started with no arguments at all - from the app list, or
    // through the web API's app runner - and reading past the end there is a null dereference.
    if (argc > 0 && argv[0] != nullptr) {
        ctx.place = argv[0];
    }
    if (argc > 3 && argv[3] != nullptr) {
        ctx.station = argv[3];
    }

    TaskEventGroup event_group {};
    task_event_group_construct(&event_group);

    AppEventSubscription sub {};
    check(app_event_subscribe(&sub, &event_group) == ERROR_NONE);

    // create_ext, not create: the destroy hook is what keeps the fetch's result from being written
    // into widgets that a suspend has already deleted. See destroyWidgets().
    const WindowId window = window_manager_create_ext(appInstanceId, createWidgets, destroyWidgets, &ctx);

    // On this task, not a worker: LVGL renders on its own task, so the window stays live while this
    // blocks on the network, and the fetch is a single request with its own timeout.
    loadRadar(&ctx);

    bool shouldClose = false;
    while (!shouldClose) {
        task_event_group_wait_any(&event_group, nullptr, portMAX_DELAY);

        AppEvent event {};
        while (app_event_poll(&sub, &event) == ERROR_NONE) {
            if (event.type == APP_EVENT_CLOSE) {
                shouldClose = true;
                break;
            }
        }
    }

    window_manager_remove(window);
    check(app_event_unsubscribe(&sub) == ERROR_NONE);
    task_event_group_destruct(&event_group);

    return 0;
}

} // namespace

extern const ::AppManifest manifest = {
    .id = "tactility.radar",
    .name = "Radar",
    .category = APP_CATEGORY_SYSTEM,
    .location = { APP_LOCATION_MEMORY, reinterpret_cast<void*>(appMain) },
    // Hidden: it is a detail view that needs a location to be useful, and it is reached from the
    // weather app rather than from a list.
    .flags = APP_MANIFEST_FLAG_HIDDEN,
};

} // namespace
