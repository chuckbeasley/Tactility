// SPDX-License-Identifier: Apache-2.0
//
// A QR code for the National Weather Service's radar map, pointed at one location.
//
// There is no browser on this device, so "view the radar" means handing the link to the device that
// has one: this screen shows the link as a QR code, and a phone camera opens the interactive map.
//
// The link is the radar site's own settings scheme. Its map state is a JSON document, base64
// encoded, carried in the `settings` query parameter behind a version prefix:
//
//     https://radar.weather.gov/?settings=v1_<base64 of the JSON>
//
// The document is the map's entire state, so the fields below are what the site's own links carry.
// The one that matters most is agenda.center, and it is [longitude, latitude] - the map is
// GeoJSON-based. Swapping them centres the map on the wrong part of the world without any error.
#include <Tactility/lvgl/QrCode.h>

#include <app/event.h>
#include <app/manifest.h>
#include <app/scheduler.h>

#include <lvgl/fonts.h>
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

/**
 * Zoom that shows roughly one radar site's coverage. The site's own links use 7 for a regional
 * view; 8 is close enough to read the local weather without the whole state in frame.
 */
constexpr int DEFAULT_ZOOM = 8;

constexpr auto* BASE64_ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

std::string base64Encode(const std::string& input) {
    std::string output;
    output.reserve(((input.size() + 2) / 3) * 4);

    size_t index = 0;
    while (index + 3 <= input.size()) {
        const uint32_t chunk =
            (static_cast<uint32_t>(static_cast<uint8_t>(input[index])) << 16) |
            (static_cast<uint32_t>(static_cast<uint8_t>(input[index + 1])) << 8) |
            static_cast<uint32_t>(static_cast<uint8_t>(input[index + 2]));
        output += BASE64_ALPHABET[(chunk >> 18) & 0x3F];
        output += BASE64_ALPHABET[(chunk >> 12) & 0x3F];
        output += BASE64_ALPHABET[(chunk >> 6) & 0x3F];
        output += BASE64_ALPHABET[chunk & 0x3F];
        index += 3;
    }

    if (index + 1 == input.size()) {
        const uint32_t chunk = static_cast<uint32_t>(static_cast<uint8_t>(input[index])) << 16;
        output += BASE64_ALPHABET[(chunk >> 18) & 0x3F];
        output += BASE64_ALPHABET[(chunk >> 12) & 0x3F];
        output += "==";
    } else if (index + 2 == input.size()) {
        const uint32_t chunk =
            (static_cast<uint32_t>(static_cast<uint8_t>(input[index])) << 16) |
            (static_cast<uint32_t>(static_cast<uint8_t>(input[index + 1])) << 8);
        output += BASE64_ALPHABET[(chunk >> 18) & 0x3F];
        output += BASE64_ALPHABET[(chunk >> 12) & 0x3F];
        output += BASE64_ALPHABET[(chunk >> 6) & 0x3F];
        output += '=';
    }

    return output;
}

bool isUnreserved(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') ||
        c == '-' || c == '_' || c == '.' || c == '~';
}

/**
 * Percent-encode everything outside the unreserved set. base64's own alphabet contains '+', '/' and
 * '=', and every one of them means something else in a query string: '+' decodes to a space, and
 * '/' and '=' are sub-delimiters. The link only survives the round trip if they are escaped.
 */
std::string percentEncode(const std::string& input) {
    std::string output;
    output.reserve(input.size() * 2);
    for (const char raw : input) {
        const auto c = static_cast<unsigned char>(raw);
        if (isUnreserved(static_cast<char>(c))) {
            output += static_cast<char>(c);
        } else {
            output += std::format("%{:02X}", c);
        }
    }
    return output;
}

/**
 * The map's settings document, field for field as the site's own links write it - including the
 * nulls, because a JSON reader cannot tell a null it wrote from a key it never wrote, and the map
 * treats "no agenda id" differently from "no agenda".
 */
std::string buildSettingsJson(double latitude, double longitude) {
    return std::format(
        "{{\"agenda\":{{\"id\":null,\"center\":[{:.4f},{:.4f}],\"location\":null,\"zoom\":{},\"layer\":null}},"
        "\"animating\":false,\"base\":\"standard\",\"artcc\":false,\"county\":false,\"cwa\":false,"
        "\"rfc\":false,\"state\":false,\"menu\":true,\"shortFusedOnly\":false,"
        "\"opacity\":{{\"alerts\":0.8,\"local\":0.6,\"localStations\":0.8,\"national\":0.6}}}}",
        longitude,
        latitude,
        DEFAULT_ZOOM
    );
}

std::string buildRadarUrl(double latitude, double longitude) {
    return std::format(
        "https://radar.weather.gov/?settings=v1_{}",
        percentEncode(base64Encode(buildSettingsJson(latitude, longitude)))
    );
}

struct Context {
    uint32_t appInstanceId = 0;
    std::string place;
    double latitude = NAN;
    double longitude = NAN;
    std::string url;
};

void onBackPressed(lv_event_t* event) {
    auto* ctx = static_cast<Context*>(lv_event_get_user_data(event));
    app_event_emit_close(ctx->appInstanceId);
}

void createWidgets(lv_obj_t* parent, void* userData) {
    auto* ctx = static_cast<Context*>(userData);

    lv_obj_set_flex_flow(parent, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_row(parent, 0, LV_STATE_DEFAULT);
    // The URL is several hundred characters, so the content is taller than the screen and has to
    // scroll. The window manager clears this flag on every window, so it is set again here.
    lv_obj_add_flag(parent, LV_OBJ_FLAG_SCROLLABLE);

    const uint8_t margin = (lvgl_get_ui_density() == LVGL_UI_DENSITY_COMPACT) ? 2 : 8;

    auto* toolbar = lvgl_toolbar_create(parent, "Radar");
    lvgl_toolbar_set_nav_action(toolbar, LV_SYMBOL_CLOSE, onBackPressed, ctx);
    lv_obj_set_style_margin_bottom(toolbar, margin, LV_STATE_DEFAULT);

    if (!std::isfinite(ctx->latitude) || !std::isfinite(ctx->longitude)) {
        // Started on its own rather than from the weather app, so there is no location to point the
        // map at. Saying so beats showing a map centred on nowhere.
        auto* label = lv_label_create(parent);
        lv_obj_set_width(label, LV_PCT(100));
        lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP);
        lv_label_set_text(label, "Open this from the Weather app - the radar needs a location to point at.");
        return;
    }

    // The code takes the square left over once the text around it has been laid out. The sizes are
    // read back after a layout pass rather than predicted from the font metrics, so this stays
    // correct if the font or the UI density changes.
    auto* place_label = lv_label_create(parent);
    lv_obj_set_width(place_label, LV_PCT(100));
    lv_label_set_text(place_label, ctx->place.empty() ? "Local radar" : ctx->place.c_str());

    auto* note_label = lv_label_create(parent);
    lv_obj_set_width(note_label, LV_PCT(100));
    lv_obj_set_style_margin_ver(note_label, margin, LV_STATE_DEFAULT);
    lv_label_set_long_mode(note_label, LV_LABEL_LONG_WRAP);
    lv_label_set_text(note_label, "Scan to open the National Weather Service radar for this location.");

    lv_obj_update_layout(parent);
    // The toolbar is inside the parent's content box, so it has to be reserved for as well -
    // leaving it out is what pushed the code off the bottom of the screen the first time.
    const int32_t reserved = lv_obj_get_height(toolbar) + lv_obj_get_height(place_label) +
        lv_obj_get_height(note_label) + (6 * margin);
    const int32_t qr_max = std::min(lv_obj_get_content_width(parent), lv_obj_get_content_height(parent) - reserved);
    LOG_I(
        TAG,
        "QR area: %d x %d, reserving %d",
        static_cast<int>(lv_obj_get_content_width(parent)),
        static_cast<int>(lv_obj_get_content_height(parent)),
        static_cast<int>(reserved)
    );

    int32_t qr_size = 0;
    // Dark on light: the QR standard's orientation, and the one every scanner expects. The crash
    // reporter draws the inverse and gets away with it, but a link meant for a phone camera should
    // not depend on that.
    lv_obj_t* qr_code = tt::lvgl::qr_code_create(parent, ctx->url, qr_max, true, &qr_size);
    if (qr_code == nullptr) {
        auto* error_label = lv_label_create(parent);
        lv_obj_set_width(error_label, LV_PCT(100));
        lv_label_set_long_mode(error_label, LV_LABEL_LONG_WRAP);
        lv_label_set_text(error_label, "Could not turn this link into a QR code.");
    } else {
        lv_obj_set_style_margin_ver(qr_code, margin, LV_STATE_DEFAULT);
    }

    // Below the code, so it is out of the way of the camera but still there for anyone who would
    // rather type it. It is long, and the screen scrolls to reach the rest of it.
    auto* url_label = lv_label_create(parent);
    lv_obj_set_width(url_label, LV_PCT(100));
    lv_label_set_long_mode(url_label, LV_LABEL_LONG_WRAP);
    lv_label_set_text(url_label, ctx->url.c_str());
}

int32_t appMain(int argc, char* argv[]) {
    const uint32_t appInstanceId = app_scheduler_current_app_id();

    Context ctx {};
    ctx.appInstanceId = appInstanceId;

    // argv: [0] place name (may be empty), [1] latitude, [2] longitude. Read defensively, because
    // this app can also be started with no arguments at all - from the app list, or through the web
    // API's app runner - and reading past the end there is a null dereference.
    if (argc > 0 && argv[0] != nullptr) {
        ctx.place = argv[0];
    }
    if (argc > 1 && argv[1] != nullptr) {
        ctx.latitude = strtod(argv[1], nullptr);
    }
    if (argc > 2 && argv[2] != nullptr) {
        ctx.longitude = strtod(argv[2], nullptr);
    }

    if (std::isfinite(ctx.latitude) && std::isfinite(ctx.longitude)) {
        ctx.url = buildRadarUrl(ctx.latitude, ctx.longitude);
        // Logged in full: it is the whole point of the screen, and it is the only way to check the
        // encoding without pointing a camera at it.
        LOG_I(TAG, "%s", ctx.url.c_str());
    }

    TaskEventGroup event_group {};
    task_event_group_construct(&event_group);

    AppEventSubscription sub {};
    check(app_event_subscribe(&sub, &event_group) == ERROR_NONE);

    const WindowId window = window_manager_create(appInstanceId, createWidgets, &ctx);

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
