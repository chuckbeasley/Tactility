#ifdef ESP_PLATFORM

#include "Tactility/PanicHandler.h"


#include <Tactility/app/crashdiagnostics/QrUrl.h>
#include <Tactility/app/launcher/Launcher.h>
#include <Tactility/file/File.h>
#include <Tactility/lvgl/QrCode.h>
#include <Tactility/lvgl/Statusbar.h>

#include <app/event.h>
#include <app/manager.h>
#include <app/start.h>
#include <app/manifest.h>
#include <app/scheduler.h>

#include <lvgl_window_manager/window_manager.h>

#include <lvgl.h>
#include <qrcode.h>
#include <tactility/check.h>
#include <tactility/drivers/pointer.h>
#include <tactility/log.h>
#include <tactility/paths.h>

#if CONFIG_IDF_TARGET_ARCH_XTENSA
#include <esp_cpu_utils.h>
#else
#include <esp_cpu.h>
#endif

#include <sdkconfig.h>

#include <algorithm>
#include <iomanip>
#include <memory>
#include <sstream>

namespace tt::app::crashdiagnostics {

constexpr auto* TAG = "CrashDiagnostics";

extern const ::AppManifest manifest;

namespace {

struct Context {
    uint32_t appInstanceId;
    // Set when widget creation hit an unrecoverable error (e.g. the QR code doesn't fit on
    // screen) - appMain() skips the event loop and closes immediately without ever starting
    // the launcher, matching the old model's stop()-without-launcher-start() error paths.
    bool hasFatalError = false;
    // Set by onContinuePressed() right before it emits APP_EVENT_CLOSE - read by appMain()
    // after its own thread finishes cleanup, to decide whether to start the launcher
    // afterwards (matches the old model's onContinuePressed(): stop() then launcher::start()).
    bool continuePressed = false;
};


const char* crashCauseToString(CrashCause cause) {
    switch (cause) {
        case CrashCause::Debug: return "Debug";
        case CrashCause::WatchdogInterrupt: return "Watchdog (interrupt)";
        case CrashCause::WatchdogTask: return "Watchdog (task)";
        case CrashCause::Abort: return "Abort";
        case CrashCause::Fault: return "Fault";
        case CrashCause::Unknown:
        default: return "Unknown";
    }
}

std::string formatCrashData(const CrashData& crashData) {
    std::stringstream stream;

    stream << "Cause: " << crashCauseToString(crashData.cause) << "\n";

    stream << "Reason: ";
    if (crashData.reason[0] != '\0') {
         stream << crashData.reason;
    } else {
        stream << "unknown";
    }
    stream << "\n";

    stream << "Fault address: " << std::hex << std::setw(8) << std::setfill('0') << crashData.faultAddress << std::dec << "\n";

    stream << "Callstack" << (crashData.callstackCorrupted ? " (corrupted)" : "") << ":";
    if (crashData.callstackLength > 0) {
        stream << "\n";
        for (uint8_t i = 0; i < crashData.callstackLength; i++) {
#if CONFIG_IDF_TARGET_ARCH_XTENSA
            uint32_t pc = esp_cpu_process_stack_pc(crashData.callstack[i].pc);
#else
            uint32_t pc = crashData.callstack[i].pc; // No processing needed on RISC-V
#endif
            stream << std::hex << std::setw(8) << std::setfill('0') << pc << std::dec << " ";
        }
    } else {
        stream << " empty" << "\n";
    }

    return stream.str();
}

// Best-effort: crash.txt is a convenience for offline inspection, not required for the app to work.
void writeCrashLogFile(const CrashData& crashData) {
    char root[128];
    if (paths_get_data_path(root, sizeof(root)) != ERROR_NONE) {
        LOG_E(TAG, "Failed to resolve data path for crash.txt");
        return;
    }

    std::string path = std::string(root) + "/crash.txt";
    if (!file::writeString(path, formatCrashData(crashData))) {
        LOG_E(TAG, "Failed to write %s", path.c_str());
    }
}

void onContinuePressed(lv_event_t* event) {
    auto* ctx = static_cast<Context*>(lv_event_get_user_data(event));
    ctx->continuePressed = true;
    app_event_emit_close(ctx->appInstanceId);
}

void createWidgets(lv_obj_t* parent, void* userData) {
    auto* ctx = static_cast<Context*>(userData);

    auto* display = lv_obj_get_display(parent);
    int32_t parent_height = lv_display_get_vertical_resolution(display) - lvgl::statusbar_get_height();

    lv_obj_add_event_cb(parent, onContinuePressed, LV_EVENT_SHORT_CLICKED, ctx);
    auto* top_label = lv_label_create(parent);
    lv_label_set_text(top_label, "Oops! We've crashed ..."); // TODO: Funny messages
    lv_obj_align(top_label, LV_ALIGN_TOP_MID, 0, 2);

    auto* bottom_label = lv_label_create(parent);
    if (device_has_active_by_type(&POINTER_TYPE)) {
        lv_label_set_text(bottom_label, "Tap screen to continue");
    } else {
        lv_label_set_text(bottom_label, "Reboot device to continue");
    }
    lv_obj_align(bottom_label, LV_ALIGN_BOTTOM_MID, 0, -2);

    const auto& crash_data = getRtcCrashData();

    std::string url = getUrlFromCrashData(crash_data);
    LOG_I(TAG, "%s", url.c_str());

    // Sized from the space the labels leave behind, then handed to the shared QR helper, which
    // scales the code in whole modules and reports whether it fitted.
    const int32_t top_label_height = lv_obj_get_height(top_label) + 2;
    const int32_t bottom_label_height = lv_obj_get_height(bottom_label) + 2;
    const int32_t available_height = parent_height - top_label_height - bottom_label_height;
    const int32_t available_width = lv_display_get_horizontal_resolution(display);
    const int32_t smallest_size = std::min(available_height, available_width);

    int32_t qr_size = 0;
    // Light on dark, as this screen has always drawn it.
    lv_obj_t* canvas = tt::lvgl::qr_code_create(parent, url, smallest_size, false, &qr_size);
    if (canvas == nullptr) {
        LOG_E(TAG, "Failed to create a QR code for the report URL");
        ctx->hasFatalError = true;
        return;
    }
    LOG_I(TAG, "QR is %d pixels", static_cast<int>(qr_size));
    lv_obj_align(canvas, LV_ALIGN_CENTER, 0, 0);
}

int32_t appMain(int argc, char* argv[]) {
    uint32_t appInstanceId = app_scheduler_current_app_id();
    Context ctx {};
    ctx.appInstanceId = appInstanceId;

    writeCrashLogFile(getRtcCrashData());

    TaskEventGroup event_group {};
    task_event_group_construct(&event_group);

    AppEventSubscription sub {};
    check(app_event_subscribe(&sub, &event_group) == ERROR_NONE);

    WindowId window = window_manager_create(appInstanceId, createWidgets, &ctx);

    if (!ctx.hasFatalError) {
        bool shouldClose = false;
        while (!shouldClose) {
            task_event_group_wait_any(&event_group, nullptr, portMAX_DELAY);

            AppEvent event {};
            while (app_event_poll(&sub, &event) == ERROR_NONE) {
                switch (event.type) {
                    case APP_EVENT_CLOSE:
                        shouldClose = true;
                        break;
                    default:
                        break;
                }
                if (shouldClose) break;
            }
        }
    }

    window_manager_remove(window);
    check(app_event_unsubscribe(&sub) == ERROR_NONE);
    task_event_group_destruct(&event_group);

    bool continuePressed = ctx.continuePressed;

    if (continuePressed) {
        launcher::start();
    }

    return 0;
}

} // namespace

void start() {
    uint32_t instanceId = 0;
    app_start(manifest.id, 0, nullptr, &instanceId);
}

extern const ::AppManifest manifest = {
    .id = "tactility.crashdiagnostics",
    .name = "Crash Diagnostics",
    .category = APP_CATEGORY_SYSTEM,
    .location = { .type = APP_LOCATION_MEMORY, .location = reinterpret_cast<void*>(appMain) },
    .flags = APP_MANIFEST_FLAG_HIDDEN,
};

} // namespace

#endif
