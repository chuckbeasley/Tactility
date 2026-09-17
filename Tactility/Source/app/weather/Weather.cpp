// SPDX-License-Identifier: Apache-2.0
//
// A weather screen fed by the National Weather Service, with the location taken from the GPS
// receiver when it has a fix and from a ZIP code otherwise.
//
// The three data sources are used the way each one is meant to be used:
//
//   * nominatim.openstreetmap.org turns a ZIP code into coordinates - it is the only one of the
//     three that understands postal codes at all;
//   * api.weather.gov/points turns coordinates into the grid forecast and the nearest observation
//     station for that point, because the NWS does not accept coordinates on its forecast
//     endpoints;
//   * api.weather.gov/stations/.../observations/latest gives the current conditions.
//
// Network requests run on this app's own task rather than a worker thread: LVGL draws on its own
// task, so the screen stays live and scrollable while a fetch is in flight, and the fetch checks
// for a close request between requests so a slow server cannot trap the user in the app.
#include "WeatherClient.h"

#include <Tactility/app/inputdialog/InputDialog.h>
#include <Tactility/file/File.h>
#include <Tactility/file/PropertiesFile.h>
#include <Tactility/time.h>

#include <app/event.h>
#include <app/manager.h>
#include <app/manifest.h>
#include <app/paths.h>
#include <app/scheduler.h>
#include <app/start.h>
#include <app/stream.h>

#include <gps/gps.h>

#include <lvgl/lvgl.h>
#include <lvgl/widgets/toolbar.h>
#include <lvgl_window_manager/window_manager.h>

#include <tactility/check.h>
#include <tactility/device.h>
#include <tactility/log.h>

#include <atomic>
#include <cctype>
#include <cmath>
#include <cstring>
#include <ctime>
#include <format>
#include <map>
#include <string>
#include <vector>

namespace tt::app::weather {

constexpr auto* TAG = "Weather";

extern const ::AppManifest manifest;

namespace {

/** How long to wait for the GPS receiver to offer a fix before falling back to the ZIP code. The
 *  driver parses continuously, so a receiver that has a fix publishes one within a sentence or two;
 *  waiting longer than this only delays the answer for a receiver that has none. */
constexpr uint32_t GPS_FIX_WAIT_MILLIS = 1500;

constexpr auto* SETTINGS_FILE_NAME = "weather.properties";
constexpr auto* SETTINGS_KEY_POSTAL_CODE = "postalCode";

std::string trim(const std::string& value) {
    const auto first = value.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) {
        return {};
    }
    const auto last = value.find_last_not_of(" \t\r\n");
    return value.substr(first, last - first + 1);
}

/** A US ZIP code: five digits, optionally followed by the four-digit extension. */
bool isPostalCode(const std::string& value) {
    for (size_t i = 0; i < 5; i++) {
        if (i >= value.size() || std::isdigit(static_cast<unsigned char>(value[i])) == 0) {
            return false;
        }
    }
    if (value.size() == 5) {
        return true;
    }
    if (value.size() == 10 && value[5] == '-') {
        for (size_t i = 6; i < 10; i++) {
            if (std::isdigit(static_cast<unsigned char>(value[i])) == 0) {
                return false;
            }
        }
        return true;
    }
    return false;
}

// region Settings

std::string getSettingsFilePath() {
    char path[256] = {};
    if (app_paths_get_user_data_path(manifest.id, SETTINGS_FILE_NAME, path, sizeof(path)) != ERROR_NONE) {
        return {};
    }
    return path;
}

std::string loadPostalCode() {
    const std::string path = getSettingsFilePath();
    if (path.empty() || !file::isFile(path)) {
        return {};
    }

    std::map<std::string, std::string> properties;
    if (!file::loadPropertiesFile(path, properties)) {
        return {};
    }

    auto entry = properties.find(SETTINGS_KEY_POSTAL_CODE);
    if (entry == properties.end()) {
        return {};
    }

    // Anything that is not a ZIP code is worth less than nothing: sending it to the geocoder
    // produces a location error on every launch, and the value is invisible to the user. An earlier
    // version of this app saved whatever the input dialog's stream happened to contain, so a stored
    // value that fails this check is treated as "no ZIP set" and logged rather than reported.
    const std::string postalCode = trim(entry->second);
    if (!isPostalCode(postalCode)) {
        LOG_W(TAG, "Ignoring the stored location '%s': it is not a ZIP code", postalCode.c_str());
        return {};
    }
    return postalCode;
}

void savePostalCode(const std::string& postalCode) {
    const std::string path = getSettingsFilePath();
    if (path.empty()) {
        return;
    }
    if (!file::findOrCreateParentDirectory(path, 0755)) {
        LOG_E(TAG, "Failed to create the settings directory for %s", path.c_str());
        return;
    }

    std::map<std::string, std::string> properties;
    properties[SETTINGS_KEY_POSTAL_CODE] = postalCode;
    if (!file::savePropertiesFile(path, properties)) {
        LOG_E(TAG, "Failed to save the ZIP code");
    }
}

// endregion

// region Formatting

/** NWS reports observations in metric (degC, km/h, Pa). The screen shows imperial: degF, mph, inHg. */
float toFahrenheit(float celsius) {
    return (celsius * 9.0f / 5.0f) + 32.0f;
}

std::string formatTemperature(float celsius) {
    if (std::isnan(celsius)) {
        return "--";
    }
    return std::format("{:.1f}\u00b0F", toFahrenheit(celsius));
}

std::string formatWind(const CurrentConditions& current) {
    if (std::isnan(current.windSpeedKph)) {
        return {};
    }

    const float mph = current.windSpeedKph * 0.621371f;
    if (std::isnan(current.windDirectionDegrees)) {
        return std::format("Wind {:.0f} mph", mph);
    }

    // Eight compass points: this is a glanceable readout, and no one reads "WSW" off a 480px screen
    // any faster than "SW".
    static const char* points[] = { "N", "NE", "E", "SE", "S", "SW", "W", "NW" };
    const int index = static_cast<int>(((current.windDirectionDegrees / 45.0f) + 0.5f)) & 7;
    return std::format("Wind {:.0f} mph {}", mph, points[index]);
}

std::string formatHumidity(const CurrentConditions& current) {
    if (std::isnan(current.relativeHumidityPercent)) {
        return {};
    }
    return std::format("Humidity {:.0f}%", current.relativeHumidityPercent);
}

std::string formatPressure(const CurrentConditions& current) {
    if (std::isnan(current.pressurePa)) {
        return {};
    }
    return std::format("{:.2f} inHg", current.pressurePa / 3386.389f);
}

/** Joins the parts that the station actually reported, so a gap does not leave a stray separator. */
std::string joinParts(const std::vector<std::string>& parts) {
    std::string result;
    for (const auto& part : parts) {
        if (part.empty()) {
            continue;
        }
        if (!result.empty()) {
            result += "   ";
        }
        result += part;
    }
    return result;
}

std::string formatLocalTime() {
    const std::time_t now = std::time(nullptr);
    std::tm local = {};
    if (localtime_r(&now, &local) == nullptr) {
        return {};
    }
    char buffer[16] = {};
    strftime(buffer, sizeof(buffer), "%H:%M", &local);
    return buffer;
}

/**
 * Pull the ZIP code out of what the input dialog returned.
 *
 * That dialog's stdout is not a private channel. The app framework logs through the same stdout
 * from the app's own task, and the scheduler writes its "[instance N] Task started" line moments
 * after the stream is bound - so what arrives here is a mixture of framework log lines and the
 * text the dialog was asked to return. Reading the whole stream as the answer is how this app came
 * to save a log line as a postal code.
 *
 * The answer is therefore the last line that is actually a ZIP code. A log line cannot match, the
 * dialog's own output is the last thing it writes, and this holds whether noise arrives before the
 * answer, after it, or on both sides.
 */
std::string extractPostalCode(const std::string& streamText) {
    size_t end = streamText.size();
    while (end > 0) {
        const size_t newline = streamText.find_last_of('\n', end - 1);
        const size_t start = (newline == std::string::npos) ? 0 : newline + 1;
        const std::string line = trim(streamText.substr(start, end - start));
        if (isPostalCode(line)) {
            return line;
        }
        if (newline == std::string::npos) {
            break;
        }
        end = newline;
    }
    return {};
}

// endregion

enum class PositionSource {
    None,
    Gps,
    PostalCode
};

struct Position {
    Coordinates coordinates;
    PositionSource source = PositionSource::None;
};

struct Context {
    uint32_t appInstanceId = 0;

    std::string postalCode;
    Position position;
    WeatherReport report;
    /** Location, source and fetch time, or the reason the screen has no data. */
    std::string status;

    TaskEventGroup eventGroup {};
    AppStream zipDialogStream {};
    uint8_t zipDialogBuffer[128] = {};
    uint32_t zipDialogInstanceId = 0;

    // The radar screen, while it is open: it is a for-result child so that closing it returns here,
    // and a for-result child has to be reaped by its parent once its result arrives. Written from
    // the LVGL task (the button) and read on this app's task (the event loop).
    std::atomic<uint32_t> radarInstanceId { 0 };

    // Set from another task: the toolbar runs in the LVGL task, refresh() and the loop run here.
    std::atomic<bool> closing { false };
    std::atomic<bool> refreshRequested { false };

    // Widgets. Null whenever this window has no live widget, which happens on suspend as well as on
    // close - see destroyWidgets().
    lv_obj_t* statusLabel = nullptr;
    lv_obj_t* currentLabel = nullptr;
    lv_obj_t* detailLabel = nullptr;
    lv_obj_t* shortTermLabel = nullptr;
    lv_obj_t* periodContainer = nullptr;
    /** Null while the window has no live widget, and also until a report has arrived - the radar
     *  has nothing to point at before then, so the button stays hidden. */
    lv_obj_t* radarButton = nullptr;

    bool hasWidgets() const {
        return statusLabel != nullptr;
    }

    void clearWidgets() {
        statusLabel = nullptr;
        currentLabel = nullptr;
        detailLabel = nullptr;
        shortTermLabel = nullptr;
        periodContainer = nullptr;
        radarButton = nullptr;
    }
};

// region GPS

bool tryGetGpsPosition(const AbortCheck& shouldAbort, Coordinates& outCoordinates) {
    Device* device = nullptr;
    device_for_each_of_type(&GPS_TYPE, &device, [](Device* candidate, void* context) {
        auto** out = static_cast<Device**>(context);
        if (device_is_ready(candidate)) {
            *out = candidate;
            return false;
        }
        return true;
    });

    if (device == nullptr) {
        return false;
    }

    GpsSubscription subscription {};
    if (gps_event_subscribe(device, &subscription) != ERROR_NONE) {
        LOG_W(TAG, "Failed to subscribe to the GPS receiver");
        return false;
    }

    bool found = false;
    const TickType_t deadline = get_ticks() + millis_to_ticks(GPS_FIX_WAIT_MILLIS);
    while (!found && !shouldAbort() && get_ticks() < deadline) {
        if (gps_event_await(device, &subscription, millis_to_ticks(250)) != ERROR_NONE) {
            continue;
        }

        const GpsEvent& event = subscription.event;
        // Either sentence can carry a usable position; GGA also says how good it is, so it is
        // preferred when it has a fix quality at all.
        if (event.type == GPS_EVENT_MESSAGE_GGA && event.data.gga.fix_quality > 0) {
            outCoordinates.latitude = minmea_tocoord(&event.data.gga.latitude);
            outCoordinates.longitude = minmea_tocoord(&event.data.gga.longitude);
            found = outCoordinates.isValid();
        } else if (event.type == GPS_EVENT_MESSAGE_RMC && event.data.rmc.valid) {
            outCoordinates.latitude = minmea_tocoord(&event.data.rmc.latitude);
            outCoordinates.longitude = minmea_tocoord(&event.data.rmc.longitude);
            found = outCoordinates.isValid();
        }
    }

    // Only safe while the device is still running: the driver frees its data when it stops, and it
    // clears its own subscriber list on the way out anyway.
    if (device_is_ready(device)) {
        gps_event_unsubscribe(device, &subscription);
    }
    return found;
}

// endregion

// region Rendering

void render(Context* ctx) {
    lvgl_lock();

    if (ctx->hasWidgets()) {
        lv_label_set_text(ctx->statusLabel, ctx->status.c_str());

        // Revealed by the first report: before that there is no location for the radar to show.
        if (ctx->radarButton != nullptr) {
            if (ctx->report.valid) {
                lv_obj_remove_flag(ctx->radarButton, LV_OBJ_FLAG_HIDDEN);
            } else {
                lv_obj_add_flag(ctx->radarButton, LV_OBJ_FLAG_HIDDEN);
            }
        }

        const CurrentConditions& current = ctx->report.current;
        if (current.valid) {
            lv_label_set_text(
                ctx->currentLabel,
                std::format("{}   {}", formatTemperature(current.temperatureC), current.description).c_str()
            );
            lv_label_set_text(
                ctx->detailLabel,
                joinParts({ formatWind(current), formatHumidity(current), formatPressure(current) }).c_str()
            );
        } else {
            lv_label_set_text(ctx->currentLabel, "No current observation");
            lv_label_set_text(ctx->detailLabel, "");
        }

        // The short-term outlook is the first period's own prose, which is what the NWS writes for
        // "today and tonight". Everything from the second period onward is the extended list below.
        const auto& periods = ctx->report.forecast.periods;
        if (!periods.empty()) {
            lv_label_set_text(
                ctx->shortTermLabel,
                std::format("{}: {}", periods.front().name, periods.front().detailedForecast).c_str()
            );
        } else {
            lv_label_set_text(ctx->shortTermLabel, "");
        }

        lv_obj_clean(ctx->periodContainer);
        for (size_t i = 1; i < periods.size(); i++) {
            const ForecastPeriod& period = periods[i];

            // One container per period, holding both labels directly. That is deliberate: the
            // screenshot endpoint renders the widget tree recursively on the HTTP server's task
            // stack, and a deeper tree overflows it (a three-container row per period did exactly
            // that). Two levels here instead of three keeps a fourteen-period list inside it.
            auto* row = lv_obj_create(ctx->periodContainer);
            lv_obj_set_size(row, LV_PCT(100), LV_SIZE_CONTENT);
            lv_obj_set_flex_flow(row, LV_FLEX_FLOW_ROW);
            lv_obj_set_flex_align(row, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
            lv_obj_set_style_border_width(row, 0, LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(row, 0, LV_STATE_DEFAULT);
            lv_obj_set_style_pad_all(row, 0, LV_STATE_DEFAULT);
            lv_obj_set_style_pad_column(row, 8, LV_STATE_DEFAULT);

            // Name, then the forecast phrase on the next line, then the wind if the period has one:
            // all one label, so the only thing beside it is the temperature.
            std::string text = period.name;
            if (!period.shortForecast.empty()) {
                text += "\n" + period.shortForecast;
            }
            if (!period.windSpeed.empty()) {
                text += "\n";
                if (!period.windDirection.empty()) {
                    text += period.windDirection + " ";
                }
                text += period.windSpeed;
            }

            auto* textLabel = lv_label_create(row);
            lv_obj_set_flex_grow(textLabel, 1);
            lv_label_set_text(textLabel, text.c_str());

            auto* temperatureLabel = lv_label_create(row);
            // Forecast periods arrive in Fahrenheit from the NWS, but the unit is part of the
            // response rather than an assumption, so it is honoured instead of converted blindly.
            if (period.temperatureUnit == "C") {
                lv_label_set_text(
                    temperatureLabel,
                    std::format("{:.0f}\u00b0F", toFahrenheit(static_cast<float>(period.temperature))).c_str()
                );
            } else {
                lv_label_set_text(temperatureLabel, std::format("{}\u00b0{}", period.temperature, period.temperatureUnit).c_str());
            }
        }
    }

    lvgl_unlock();
}

// endregion

// region Fetching

void setStatus(Context* ctx, std::string status) {
    ctx->status = std::move(status);
}

void refresh(Context* ctx) {
    LOG_I(TAG, "Refreshing");
    setStatus(ctx, "Loading...");
    render(ctx);

    const AbortCheck shouldAbort = [ctx] { return ctx->closing.load(); };

    Position position;

    if (tryGetGpsPosition(shouldAbort, position.coordinates)) {
        position.source = PositionSource::Gps;
        LOG_I(TAG, "Using the GPS fix at %.4f, %.4f", position.coordinates.latitude, position.coordinates.longitude);
    } else if (!ctx->postalCode.empty()) {
        position.source = PositionSource::PostalCode;
        std::string error;
        if (!geocodePostalCode(ctx->postalCode, position.coordinates, error)) {
            setStatus(ctx, std::format("ZIP {}: {}", ctx->postalCode, error));
            render(ctx);
            return;
        }
        LOG_I(TAG, "ZIP %s resolved to %.4f, %.4f", ctx->postalCode.c_str(), position.coordinates.latitude, position.coordinates.longitude);
    } else {
        setStatus(ctx, "No location. Set a ZIP code, or wait for a GPS fix.");
        render(ctx);
        return;
    }

    if (shouldAbort()) {
        return;
    }

    WeatherReport report;
    std::string error;
    if (!fetchReport(position.coordinates, report, shouldAbort, error)) {
        if (shouldAbort()) {
            return;
        }
        setStatus(ctx, error);
        render(ctx);
        return;
    }

    ctx->report = std::move(report);
    ctx->position = position;

    const std::string sourceText = position.source == PositionSource::Gps
        ? std::string("GPS")
        : std::format("ZIP {}", ctx->postalCode);
    const std::string locationText = ctx->report.locationName.empty()
        ? sourceText
        : std::format("{} ({})", ctx->report.locationName, sourceText);
    setStatus(ctx, std::format("{}   {}", locationText, formatLocalTime()));

    render(ctx);
}

// endregion

// region Widgets

void onBackPressed(lv_event_t* event) {
    auto* ctx = static_cast<Context*>(lv_event_get_user_data(event));
    ctx->closing.store(true);
    app_event_emit_close(ctx->appInstanceId);
}

void onZipPressed(lv_event_t* event) {
    auto* ctx = static_cast<Context*>(lv_event_get_user_data(event));
    ctx->zipDialogInstanceId = inputdialog::start(
        ctx->appInstanceId,
        "ZIP code",
        "Enter a US ZIP code:",
        ctx->postalCode,
        ctx->zipDialogStream,
        ctx->zipDialogBuffer,
        sizeof(ctx->zipDialogBuffer),
        &ctx->eventGroup
    );
}

void onRefreshPressed(lv_event_t* event) {
    auto* ctx = static_cast<Context*>(lv_event_get_user_data(event));
    // Only flags it: the fetch belongs on the app's task, and this runs in the LVGL task.
    ctx->refreshRequested.store(true);
}

/**
 * Opens the radar screen on the location the report was fetched for, so the button means "the radar
 * for what you are looking at" rather than "a radar map you then have to navigate".
 *
 * Started for a result rather than with app_start(), which is what makes closing it come back here:
 * a for-result launch records this instance as the parent, so the window manager has somewhere to
 * return to. Fire-and-forget leaves it with none, and the launcher is what comes up instead.
 */
void onRadarPressed(lv_event_t* event) {
    auto* ctx = static_cast<Context*>(lv_event_get_user_data(event));

    const std::string latitude = std::format("{:.4f}", ctx->position.coordinates.latitude);
    const std::string longitude = std::format("{:.4f}", ctx->position.coordinates.longitude);
    // The radar site comes from the report: the imagery is published per site, not per coordinate.
    const char* arguments[] = {
        ctx->report.locationName.c_str(),
        latitude.c_str(),
        longitude.c_str(),
        ctx->report.radarStation.c_str()
    };

    uint32_t instanceId = 0;
    if (app_start_for_result("tactility.radar", 4, arguments, ctx->appInstanceId, &instanceId) != ERROR_NONE) {
        LOG_W(TAG, "Failed to open the radar");
    } else {
        ctx->radarInstanceId.store(instanceId);
    }
}

void createWidgets(lv_obj_t* parent, void* userData) {
    auto* ctx = static_cast<Context*>(userData);

    lv_obj_set_flex_flow(parent, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_row(parent, 0, LV_STATE_DEFAULT);

    const uint8_t margin = (lvgl_get_ui_density() == LVGL_UI_DENSITY_COMPACT) ? 2 : 8;

    auto* toolbar = lvgl_toolbar_create(parent, "Weather");
    // The global toolbar nav callback only knows how to stop old-model apps.
    lvgl_toolbar_set_nav_action(toolbar, LV_SYMBOL_CLOSE, onBackPressed, ctx);
    // "Map", not "Radar": the toolbar sizes its text buttons as squares, so a five-letter label is
    // clipped to "Rada". Three characters fit, as the neighbouring ZIP button shows, and the screen
    // it opens is titled Radar.
    ctx->radarButton = lvgl_toolbar_add_text_button_action(toolbar, "Map", onRadarPressed, ctx);
    // Hidden until a report arrives: the radar link is built from the coordinates the report was
    // fetched for, and offering it before there are any would open a map pointed at nowhere.
    lv_obj_add_flag(ctx->radarButton, LV_OBJ_FLAG_HIDDEN);
    lvgl_toolbar_add_text_button_action(toolbar, "ZIP", onZipPressed, ctx);
    lvgl_toolbar_add_text_button_action(toolbar, LV_SYMBOL_REFRESH, onRefreshPressed, ctx);
    lv_obj_set_style_margin_bottom(toolbar, margin, LV_STATE_DEFAULT);

    // One scrollable column for the whole report. The extended forecast is fourteen periods, which
    // cannot fit on this screen at any readable size, so scrolling is expected rather than a last
    // resort - and it is the only scroll region, so there is no ambiguity about which one moves.
    auto* wrapper = lv_obj_create(parent);
    lv_obj_set_size(wrapper, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(wrapper, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_grow(wrapper, 1);
    lv_obj_set_style_border_width(wrapper, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_pad_hor(wrapper, margin, 0);
    lv_obj_set_style_pad_top(wrapper, 0, 0);
    lv_obj_set_style_pad_bottom(wrapper, margin, 0);
    lv_obj_set_style_pad_row(wrapper, margin, 0);

    ctx->statusLabel = lv_label_create(wrapper);
    lv_obj_set_width(ctx->statusLabel, LV_PCT(100));
    lv_label_set_text(ctx->statusLabel, "Loading...");

    ctx->currentLabel = lv_label_create(wrapper);
    lv_obj_set_width(ctx->currentLabel, LV_PCT(100));
    lv_obj_set_style_text_color(ctx->currentLabel, lv_palette_main(LV_PALETTE_BLUE), LV_STATE_DEFAULT);

    ctx->detailLabel = lv_label_create(wrapper);
    lv_obj_set_width(ctx->detailLabel, LV_PCT(100));

    ctx->shortTermLabel = lv_label_create(wrapper);
    lv_obj_set_width(ctx->shortTermLabel, LV_PCT(100));

    ctx->periodContainer = lv_obj_create(wrapper);
    lv_obj_set_size(ctx->periodContainer, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(ctx->periodContainer, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_border_width(ctx->periodContainer, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ctx->periodContainer, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(ctx->periodContainer, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_pad_row(ctx->periodContainer, margin, 0);

    render(ctx);
}

/**
 * Called when this window's widget is deleted, which happens on suspend as well as on close. The
 * labels above are pointers into that tree, so they have to be dropped here or the next render
 * walks freed widgets. This runs inside the LVGL-locked section that is deleting them, and render()
 * holds that same lock for its whole body, so clearing them here is sufficient on its own.
 */
void destroyWidgets(void* userData) {
    auto* ctx = static_cast<Context*>(userData);
    ctx->clearWidgets();
}

// endregion

void handleResult(Context* ctx, const AppResultEventData& result) {
    // The radar closes like any other child and has to be reaped, but it carries nothing back: the
    // screen beside it is the point of it.
    if (result.launch_id == ctx->radarInstanceId.load() && result.launch_id != 0) {
        ctx->radarInstanceId.store(0);
        app_manager_stop(result.launch_id);
        return;
    }

    if (result.launch_id != ctx->zipDialogInstanceId || ctx->zipDialogInstanceId == 0) {
        return;
    }
    ctx->zipDialogInstanceId = 0;

    if (result.result == 0) {
        char buffer[sizeof(ctx->zipDialogBuffer) + 1] = {};
        const size_t length = app_stream_read(&ctx->zipDialogStream, buffer, sizeof(ctx->zipDialogBuffer));
        const std::string entered = extractPostalCode(std::string(buffer, length));
        if (entered.empty()) {
            // The stream carried no ZIP code at all - either nothing was typed, or the dialog's
            // output was lost behind framework logging. Saying so is better than saving the noise.
            LOG_W(TAG, "The location dialog returned no ZIP code");
            setStatus(ctx, "Enter a 5-digit US ZIP code");
            render(ctx);
        } else if (entered != ctx->postalCode) {
            ctx->postalCode = entered;
            savePostalCode(entered);
            ctx->refreshRequested.store(true);
        }
    }

    // The dialog's stream is only valid until this point, and its instance has to be reaped by the
    // caller once the result is in.
    app_stream_unsubscribe(&ctx->zipDialogStream);
    app_manager_stop(result.launch_id);
}

int32_t appMain(int argc, char* argv[]) {
    const uint32_t appInstanceId = app_scheduler_current_app_id();

    Context ctx {};
    ctx.appInstanceId = appInstanceId;
    ctx.postalCode = loadPostalCode();

    task_event_group_construct(&ctx.eventGroup);

    AppEventSubscription eventSubscription {};
    check(app_event_subscribe(&eventSubscription, &ctx.eventGroup) == ERROR_NONE);

    // create_ext, not create: the destroy hook is what keeps render() from touching widgets that a
    // suspend has already deleted. See destroyWidgets().
    const WindowId window = window_manager_create_ext(appInstanceId, createWidgets, destroyWidgets, &ctx);

    ctx.refreshRequested.store(true);

    bool shouldClose = false;
    while (!shouldClose) {
        // Blocks for as long as a fetch takes; a close request raised meanwhile is picked up by the
        // fetch's own abort check and by the event poll below.
        if (ctx.refreshRequested.exchange(false)) {
            refresh(&ctx);
        }

        task_event_group_wait_any(&ctx.eventGroup, nullptr, millis_to_ticks(500));

        AppEvent event {};
        while (app_event_poll(&eventSubscription, &event) == ERROR_NONE) {
            switch (event.type) {
                case APP_EVENT_CLOSE:
                    ctx.closing.store(true);
                    shouldClose = true;
                    break;
                case APP_EVENT_RESULT:
                    handleResult(&ctx, event.result);
                    break;
                default:
                    break;
            }
            if (shouldClose) {
                break;
            }
        }
    }

    ctx.closing.store(true);
    window_manager_remove(window);
    check(app_event_unsubscribe(&eventSubscription) == ERROR_NONE);
    task_event_group_destruct(&ctx.eventGroup);

    return 0;
}

} // namespace

extern const ::AppManifest manifest = {
    .id = "tactility.weather",
    .name = "Weather",
    .category = APP_CATEGORY_USER,
    .location = { APP_LOCATION_MEMORY, reinterpret_cast<void*>(appMain) },
    // The fetch runs on this task: esp_http_client, the TLS handshake, and a shallow JSON parse.
    // 24 KB is measured, not guessed - at 16 KB the very first request panicked the device with a
    // stack protection fault inside the handshake, and 24 KB carries five handshakes per refresh
    // without complaint. Most of it is mbedTLS verifying the server's chain against the certificate
    // bundle, which is why it is far more than the http module's own download task needs with a
    // single pinned certificate. App stacks come out of internal RAM, the scarcest resource here,
    // so this is the one place the app spends it.
    .stack = { .depth = 6144 }
};

} // namespace
