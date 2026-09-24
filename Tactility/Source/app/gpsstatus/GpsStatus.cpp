// SPDX-License-Identifier: Apache-2.0
//
// A live view of the GPS receiver's fix state: whether it has a fix at all, how many satellites it
// is using, and the position, velocity and time the driver last published.
//
// It exists because the driver is deliberately quiet. Valid NMEA is parsed and published as events
// and the only thing it ever logs is a *parse failure*, so a receiver that is working perfectly and
// one that is not wired up at all are indistinguishable from the console. That is exactly the
// question that cost time during this board's bring-up, and the answer had to come from a
// temporary raw-byte echo bolted into the driver. This screen is the permanent version of that.
//
// Nothing here polls the receiver: the app subscribes to the driver's GPS events and blocks on
// them, so a sentence arriving is what wakes the loop. Fix data is only ever published through
// those events - there is no "last known position" accessor on the device - so subscribing is not
// merely the tidier choice, it is the only way to see a fix.
#include <gps/gps.h>

#include <Tactility/time.h>
#include <Tactility/units/Units.h>

#include <app/event.h>
#include <app/manifest.h>
#include <app/scheduler.h>

#include <lvgl/lvgl.h>
#include <lvgl/widgets/toolbar.h>
#include <lvgl_window_manager/window_manager.h>

#include <tactility/check.h>
#include <tactility/device.h>
#include <tactility/log.h>

#include <cmath>
#include <format>
#include <string>

namespace tt::app::gpsstatus {

extern const ::AppManifest manifest;

namespace {

constexpr auto* TAG = "GpsStatus";

// How long a wait on the GPS subscription lasts before the loop comes up for air. It bounds how
// quickly the screen notices that the device went away, and how often the "age" field is refreshed.
constexpr uint32_t LOOP_INTERVAL_MILLIS = 250;

// The screen is a status readout, so it does not need to redraw at the loop's rate. Nothing here
// changes faster than the 1 Hz sentence rate, and each redraw costs SPI traffic on a screen that
// shares its bus with everything else.
constexpr uint32_t RENDER_INTERVAL_MILLIS = 500;

struct GpsDeviceSearch {
    Device* first = nullptr;
    Device* ready = nullptr;
};

/** The last values the receiver published. Kept in plain structs because minmea already parses
 *  into fixed-point and a fix is only meaningful as the set of fields a sentence carried. */
struct FixState {
    bool hasRmc = false;
    bool hasGga = false;

    /** RMC's own "A" flag. Kept separately because a receiver can report a valid position in RMC
     *  while GGA carries no fix quality - the two sentences do not have to agree. */
    bool rmcValid = false;

    int fixQuality = 0;
    int satellites = 0;
    struct minmea_float hdop {};
    struct minmea_float latitude {};
    struct minmea_float longitude {};
    struct minmea_float altitude {};
    struct minmea_float speed {};
    struct minmea_float course {};
    struct minmea_time time {};

    uint32_t sentenceCount = 0;
    size_t lastSentenceMillis = 0;

    bool hasFix() const {
        return fixQuality > 0 || rmcValid;
    }
};

struct Context {
    uint32_t appInstanceId = 0;
    Device* device = nullptr;

    GpsSubscription subscription {};
    bool subscribed = false;

    FixState fix {};

    size_t lastRenderMillis = 0;
    bool renderPending = true;

    // Null whenever this window has no live widget. The window manager deletes the widget tree on
    // suspend as well as on close, so these cannot be assumed valid just because the app is alive.
    lv_obj_t* deviceValue = nullptr;
    lv_obj_t* stateValue = nullptr;
    lv_obj_t* fixValue = nullptr;
    lv_obj_t* satellitesValue = nullptr;
    lv_obj_t* hdopValue = nullptr;
    lv_obj_t* latitudeValue = nullptr;
    lv_obj_t* longitudeValue = nullptr;
    lv_obj_t* altitudeValue = nullptr;
    lv_obj_t* speedValue = nullptr;
    lv_obj_t* courseValue = nullptr;
    lv_obj_t* timeValue = nullptr;
    lv_obj_t* sentencesValue = nullptr;

    bool hasWidgets() const {
        return deviceValue != nullptr;
    }

    void clearWidgets() {
        deviceValue = nullptr;
        stateValue = nullptr;
        fixValue = nullptr;
        satellitesValue = nullptr;
        hdopValue = nullptr;
        latitudeValue = nullptr;
        longitudeValue = nullptr;
        altitudeValue = nullptr;
        speedValue = nullptr;
        courseValue = nullptr;
        timeValue = nullptr;
        sentencesValue = nullptr;
    }
};

Device* findGpsDevice() {
    GpsDeviceSearch search {};
    device_for_each_of_type(&GPS_TYPE, &search, [](Device* device, void* context) {
        auto* search = static_cast<GpsDeviceSearch*>(context);
        if (search->first == nullptr) {
            search->first = device;
        }
        if (search->ready == nullptr && device_is_ready(device)) {
            search->ready = device;
        }
        return true;
    });

    // A running device is the one worth showing. Falling back to a stopped one still lets the
    // screen report that the hardware exists but is off, which is more useful than "not found".
    return search.ready != nullptr ? search.ready : search.first;
}

/** A title on the left and a value on the right, returned so the value can be rewritten later. */
lv_obj_t* createStatusRow(lv_obj_t* parent, const char* title) {
    auto* row = lv_obj_create(parent);
    lv_obj_set_size(row, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(row, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(row, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_border_width(row, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(row, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(row, 0, LV_STATE_DEFAULT);

    auto* titleLabel = lv_label_create(row);
    lv_label_set_text(titleLabel, title);

    auto* valueLabel = lv_label_create(row);
    lv_label_set_text(valueLabel, "--");
    return valueLabel;
}

const char* stateToText(enum GpsState state) {
    switch (state) {
        case GPS_STATE_OFF: return "off";
        case GPS_STATE_PENDING_ON: return "starting";
        case GPS_STATE_ON: return "on";
        case GPS_STATE_ERROR: return "error";
        case GPS_STATE_PENDING_OFF: return "stopping";
        default: return "unknown";
    }
}

std::string deviceToText(const Context* ctx) {
    if (ctx->device == nullptr) {
        return "not found";
    }
    if (!device_is_ready(ctx->device)) {
        return ctx->device->name;
    }

    char modelName[64] = {};
    if (gps_get_model_name(ctx->device, modelName, sizeof(modelName)) == ERROR_NONE) {
        return std::format("{} ({})", modelName, ctx->device->name);
    }
    return ctx->device->name;
}

/** Fixed-point to decimal text. minmea reports "unknown" as a zero scale, which becomes NaN. */
std::string formatFixed(const struct minmea_float* value, int decimals, const char* suffix) {
    const float converted = minmea_tofloat(value);
    if (std::isnan(converted)) {
        return "--";
    }
    return std::format("{:.{}f}{}", converted, decimals, suffix);
}

/**
 * GGA reports altitude in metres. NMEA defines that field's unit as 'M' and nothing else, which is
 * what makes the stored value unambiguous; how it is *shown* follows the unit system in Region &
 * Language, because "1400 ft" is what a reader in the US expects and "430 m" is what everyone else
 * does. The conversion lives in tt::units so every screen converts the same way.
 */
std::string formatAltitude(const struct minmea_float* value) {
    const float metres = minmea_tofloat(value);
    if (std::isnan(metres)) {
        return "--";
    }
    return units::formatDistance(metres, 1);
}

/**
 * RMC reports speed in knots, which is fixed by the sentence definition - minmea parses it as a
 * bare value with no unit field, so there is nothing else it could be. Knots are neither metric nor
 * imperial, so this is a conversion either way: 1 kn = 1.852 km/h exactly, and 1.150779448 mph.
 */
std::string formatSpeed(const struct minmea_float* value) {
    const float knots = minmea_tofloat(value);
    if (std::isnan(knots)) {
        return "--";
    }
    return units::formatSpeedFromKnots(knots, 1);
}

/** Coordinates are DDMM.MMMM in the sentence; minmea_tocoord converts to signed decimal degrees. */
std::string formatCoordinate(const struct minmea_float* value) {
    const float coordinate = minmea_tocoord(value);
    if (std::isnan(coordinate)) {
        return "--";
    }
    return std::format("{:.6f}", coordinate);
}

std::string fixToText(const FixState& fix) {
    if (!fix.hasRmc && !fix.hasGga) {
        return "waiting for data";
    }

    switch (fix.fixQuality) {
        case 0: break;
        case 1: return "GPS fix";
        case 2: return "DGPS fix";
        case 3: return "PPS fix";
        default: return std::format("fix (quality {})", fix.fixQuality);
    }

    // GGA says there is no fix quality, but RMC is the authority on whether the position in it is
    // usable, so a receiver that populates one and not the other is still reported as fixed.
    return fix.rmcValid ? "fix (RMC)" : "no fix";
}

/**
 * minmea leaves every field of an unpopulated sentence at -1, so a receiver with no fix reports a
 * time of -1:-1:-1. Printing that verbatim looks like a bug in this screen rather than data from
 * the module, so unknown times are shown as unknown.
 */
std::string timeToText(const struct minmea_time* time) {
    if (time->hours < 0 || time->minutes < 0 || time->seconds < 0) {
        return "--";
    }
    return std::format("{:02}:{:02}:{:02} UTC", time->hours, time->minutes, time->seconds);
}

/** HDOP is meaningless without a fix, and the conventional "no data" value the receivers send is
 *  99.99 - a number that reads like a measurement unless it is filtered out. */
std::string hdopToText(const FixState& fix) {
    const float hdop = minmea_tofloat(&fix.hdop);
    if (std::isnan(hdop) || hdop >= 99.0f) {
        return "--";
    }
    return std::format("{:.2f}", hdop);
}

std::string sentencesToText(const FixState& fix) {
    if (fix.sentenceCount == 0) {
        return "none";
    }

    const size_t ageMillis = get_millis() - fix.lastSentenceMillis;
    if (ageMillis < 1000) {
        return std::format("{} ({} ms ago)", fix.sentenceCount, ageMillis);
    }
    return std::format("{} ({} s ago)", fix.sentenceCount, ageMillis / 1000);
}

/**
 * Subscribes while the device is running and drops the subscription when it stops. Order matters:
 * the driver nulls its driver data when the device stops, so calling unsubscribe after noticing it
 * is gone would dereference nothing. It does not need calling either - the driver clears its whole
 * subscriber list on the way out - so the stopped case only has to update this flag.
 */
void updateSubscription(Context* ctx) {
    if (ctx->device == nullptr) {
        ctx->subscribed = false;
        return;
    }

    const bool ready = device_is_ready(ctx->device);
    if (ready && !ctx->subscribed) {
        if (gps_event_subscribe(ctx->device, &ctx->subscription) == ERROR_NONE) {
            ctx->subscribed = true;
        } else {
            LOG_W(TAG, "Failed to subscribe to %s", ctx->device->name);
        }
    } else if (!ready && ctx->subscribed) {
        ctx->subscribed = false;
    }
}

void applyEvent(Context* ctx, const GpsEvent& event) {
    FixState& fix = ctx->fix;

    if (event.type == GPS_EVENT_UNSUBSCRIBED) {
        ctx->subscribed = false;
        return;
    }

    fix.sentenceCount++;
    fix.lastSentenceMillis = get_millis();
    ctx->renderPending = true;

    switch (event.type) {
        case GPS_EVENT_MESSAGE_RMC:
            fix.hasRmc = true;
            fix.rmcValid = event.data.rmc.valid;
            fix.latitude = event.data.rmc.latitude;
            fix.longitude = event.data.rmc.longitude;
            fix.speed = event.data.rmc.speed;
            fix.course = event.data.rmc.course;
            fix.time = event.data.rmc.time;
            break;
        case GPS_EVENT_MESSAGE_GGA:
            fix.hasGga = true;
            fix.fixQuality = event.data.gga.fix_quality;
            fix.satellites = event.data.gga.satellites_tracked;
            fix.hdop = event.data.gga.hdop;
            fix.altitude = event.data.gga.altitude;
            fix.latitude = event.data.gga.latitude;
            fix.longitude = event.data.gga.longitude;
            fix.time = event.data.gga.time;
            break;
        default:
            break;
    }
}

void render(Context* ctx) {
    lvgl_lock();

    if (ctx->hasWidgets()) {
        const FixState& fix = ctx->fix;

        lv_label_set_text(ctx->deviceValue, deviceToText(ctx).c_str());

        const bool ready = ctx->device != nullptr && device_is_ready(ctx->device);
        lv_label_set_text(ctx->stateValue, ready ? stateToText(gps_get_state(ctx->device)) : "not running");

        lv_label_set_text(ctx->fixValue, fixToText(fix).c_str());
        lv_obj_set_style_text_color(
            ctx->fixValue,
            fix.hasFix() ? lv_palette_main(LV_PALETTE_GREEN) : lv_palette_main(LV_PALETTE_ORANGE),
            LV_STATE_DEFAULT
        );

        // Formatted straight into the label: std::to_string allocated a temporary and then LVGL
        // copied it, which is two allocations per update on a screen that updates every second.
        lv_label_set_text_fmt(ctx->satellitesValue, "%u", static_cast<unsigned>(fix.satellites));
        lv_label_set_text(ctx->hdopValue, hdopToText(fix).c_str());
        lv_label_set_text(ctx->latitudeValue, formatCoordinate(&fix.latitude).c_str());
        lv_label_set_text(ctx->longitudeValue, formatCoordinate(&fix.longitude).c_str());
        lv_label_set_text(ctx->altitudeValue, formatAltitude(&fix.altitude).c_str());
        lv_label_set_text(ctx->speedValue, formatSpeed(&fix.speed).c_str());
        lv_label_set_text(ctx->courseValue, formatFixed(&fix.course, 1, " deg").c_str());
        lv_label_set_text(ctx->timeValue, timeToText(&fix.time).c_str());
        lv_label_set_text(ctx->sentencesValue, sentencesToText(fix).c_str());
    }

    lvgl_unlock();
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

    auto* toolbar = lvgl_toolbar_create(parent, "GPS Status");
    // The global toolbar nav callback only knows how to stop old-model apps.
    lvgl_toolbar_set_nav_action(toolbar, LV_SYMBOL_CLOSE, onBackPressed, ctx);
    lv_obj_set_style_margin_bottom(toolbar, margin, LV_STATE_DEFAULT);

    auto* wrapper = lv_obj_create(parent);
    lv_obj_set_size(wrapper, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(wrapper, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_grow(wrapper, 1);
    lv_obj_set_style_border_width(wrapper, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(wrapper, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_pad_hor(wrapper, margin, 0);
    lv_obj_set_style_pad_top(wrapper, 0, 0);
    lv_obj_set_style_pad_bottom(wrapper, margin, 0);
    lv_obj_set_style_pad_column(wrapper, margin, 0);

    // Two columns, because twelve rows of the default font do not fit in this screen's height and a
    // status readout that has to be scrolled to reach the time is worse than one that does not.
    // Left answers "is the receiver working", right answers "where and when is it".
    auto* receiverColumn = lv_obj_create(wrapper);
    lv_obj_set_flex_flow(receiverColumn, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_grow(receiverColumn, 1);
    lv_obj_set_style_border_width(receiverColumn, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(receiverColumn, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(receiverColumn, 0, 0);
    lv_obj_set_style_pad_row(receiverColumn, margin, 0);

    auto* positionColumn = lv_obj_create(wrapper);
    lv_obj_set_flex_flow(positionColumn, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_grow(positionColumn, 1);
    lv_obj_set_style_border_width(positionColumn, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(positionColumn, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(positionColumn, 0, 0);
    lv_obj_set_style_pad_row(positionColumn, margin, 0);

    ctx->deviceValue = createStatusRow(receiverColumn, "Device");
    ctx->stateValue = createStatusRow(receiverColumn, "State");
    ctx->fixValue = createStatusRow(receiverColumn, "Fix");
    ctx->satellitesValue = createStatusRow(receiverColumn, "Satellites");
    ctx->hdopValue = createStatusRow(receiverColumn, "HDOP");
    ctx->sentencesValue = createStatusRow(receiverColumn, "Sentences");

    ctx->latitudeValue = createStatusRow(positionColumn, "Latitude");
    ctx->longitudeValue = createStatusRow(positionColumn, "Longitude");
    ctx->altitudeValue = createStatusRow(positionColumn, "Altitude");
    ctx->speedValue = createStatusRow(positionColumn, "Speed");
    ctx->courseValue = createStatusRow(positionColumn, "Course");
    ctx->timeValue = createStatusRow(positionColumn, "UTC time");

    // The window is rebuilt on resume, so fill it from whatever has already been received instead
    // of waiting for the next sentence to arrive before showing anything.
    ctx->renderPending = false;
    ctx->lastRenderMillis = get_millis();
    render(ctx);
}

/**
 * Called when this window's widget is deleted, which happens on suspend as well as on close. The
 * rows above are pointers into that tree, so they have to be dropped here or the next render walks
 * freed widgets. This runs inside the LVGL-locked section that is deleting them, and render() holds
 * that same lock for its whole body, so clearing them here is sufficient on its own.
 */
void destroyWidgets(void* userData) {
    auto* ctx = static_cast<Context*>(userData);
    ctx->clearWidgets();
}

int32_t appMain(int argc, char* argv[]) {
    const uint32_t appInstanceId = app_scheduler_current_app_id();

    Context ctx {};
    ctx.appInstanceId = appInstanceId;
    ctx.device = findGpsDevice();

    TaskEventGroup eventGroup {};
    task_event_group_construct(&eventGroup);

    AppEventSubscription eventSubscription {};
    check(app_event_subscribe(&eventSubscription, &eventGroup) == ERROR_NONE);

    // create_ext, not create: the destroy hook is what keeps render() from touching widgets that a
    // suspend has already deleted. See destroyWidgets() above.
    const WindowId window = window_manager_create_ext(appInstanceId, createWidgets, destroyWidgets, &ctx);

    bool shouldClose = false;
    while (!shouldClose) {
        updateSubscription(&ctx);

        if (ctx.subscribed) {
            // This is the wait that paces the loop: it returns as soon as a sentence arrives, and
            // after the timeout if the receiver is silent (no fix, no sky view, or unplugged).
            if (gps_event_await(ctx.device, &ctx.subscription, millis_to_ticks(LOOP_INTERVAL_MILLIS)) == ERROR_NONE) {
                applyEvent(&ctx, ctx.subscription.event);
            }
        } else {
            // Nothing to wait on, so the loop has to pace itself.
            task_event_group_wait_any(&eventGroup, nullptr, millis_to_ticks(LOOP_INTERVAL_MILLIS));
        }

        const size_t now = get_millis();
        if (ctx.renderPending || now - ctx.lastRenderMillis >= RENDER_INTERVAL_MILLIS) {
            ctx.renderPending = false;
            ctx.lastRenderMillis = now;
            render(&ctx);
        }

        AppEvent event {};
        while (app_event_poll(&eventSubscription, &event) == ERROR_NONE) {
            if (event.type == APP_EVENT_CLOSE) {
                shouldClose = true;
                break;
            }
        }
    }

    // The driver clears its own subscriber list when the device stops, and it must not be touched
    // here if that has already happened, so only unsubscribe from a device that is still running.
    if (ctx.subscribed && ctx.device != nullptr && device_is_ready(ctx.device)) {
        gps_event_unsubscribe(ctx.device, &ctx.subscription);
    }
    ctx.subscribed = false;

    window_manager_remove(window);
    check(app_event_unsubscribe(&eventSubscription) == ERROR_NONE);
    task_event_group_destruct(&eventGroup);

    return 0;
}

} // namespace

extern const ::AppManifest manifest = {
    .id = "tactility.gpsstatus",
    .name = "GPS Status",
    .category = APP_CATEGORY_SETTINGS,
    .location = { APP_LOCATION_MEMORY, reinterpret_cast<void*>(appMain) },
    .stack = { .depth = 4096 }
};

} // namespace
