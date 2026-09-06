#include <Tactility/DeprecatedPaths.h>
#include <Tactility/Mutex.h>
#include <Tactility/app/apphub/AppHub.h>
#include <Tactility/app/apphub/AppHubEntry.h>
#include <Tactility/app/apphubdetails/AppHubDetailsApp.h>
#include <Tactility/file/File.h>
#include <Tactility/network/Http.h>
#include <Tactility/service/wifi/Wifi.h>

#include <app/event.h>
#include <app/manager.h>
#include <app/manifest.h>

#include <lvgl_window_manager/window_manager.h>

#include <tactility/concurrent/task_event_group.h>
#include <tactility/log.h>

#include <lvgl/lvgl.h>
#include <lvgl/widgets/spinner.h>
#include <lvgl/widgets/toolbar.h>

#include <algorithm>
#include <atomic>
#include <format>

namespace tt::app::apphub {

constexpr auto* TAG = "AppHub";

extern const ::AppManifest manifest;

namespace {

struct Context {
    uint32_t appInstanceId;

    lv_obj_t* contentWrapper = nullptr;
    lv_obj_t* refreshButton = nullptr;
    std::string cachedAppsJsonFile = std::format("{}/app_hub.json", getTempPath());
    std::vector<AppHubEntry> entries;
    Mutex mutex;
    std::atomic<uint32_t> uiGeneration = 1;
};


bool showApps(Context* ctx);
void refresh(Context* ctx, bool force);
void tryRefreshVersion(Context* ctx, uint32_t refreshGeneration, const std::vector<std::string>& urls, size_t index);

void onBackPressed(lv_event_t* event) {
    auto* ctx = static_cast<Context*>(lv_event_get_user_data(event));
    // Async, non-blocking - must NOT call app_manager_stop() directly here: that bound-waits
    // (thread_join) for this app's own thread to finish, which needs the LVGL lock
    // (window_manager_remove()) - but this callback runs ON the LVGL task, which would
    // deadlock against itself.
    app_event_emit_close(ctx->appInstanceId);
}

void onAppPressed(lv_event_t* e) {
    auto* ctx = static_cast<Context*>(lv_event_get_user_data(e));
    auto* widget = lv_event_get_target_obj(e);
    const auto* user_data = lv_obj_get_user_data(widget);
    const intptr_t index = reinterpret_cast<intptr_t>(user_data);
    AppHubEntry selected {};
    bool found = false;
    ctx->mutex.lock();
    if (index < ctx->entries.size()) {
        selected = ctx->entries[index];
        found = true;
    }
    ctx->mutex.unlock();
    if (found) {
        apphubdetails::start(selected);
    }
}

void onRefreshPressed(lv_event_t* e) {
    auto* ctx = static_cast<Context*>(lv_event_get_user_data(e));
    refresh(ctx, true);
}

void showRefreshFailedError(Context* ctx, const char* message) {
    if (ctx->contentWrapper == nullptr || ctx->refreshButton == nullptr) {
        return;
    }
    lv_obj_clean(ctx->contentWrapper);

    auto* label = lv_label_create(ctx->contentWrapper);
    lv_label_set_text(label, message);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

    lv_obj_remove_flag(ctx->refreshButton, LV_OBJ_FLAG_HIDDEN);
}

void showNoInternet(Context* ctx) {
    showRefreshFailedError(ctx, "No Internet Connection");
}

// The download layer emits short, technical error strings. Map them to something the user
// can reason about: the important distinction is "couldn't connect at all" (network/TLS/DNS)
// versus "the server answered but had no apps content for this version" (a 404/error).
static const char* mapDownloadError(const char* error) {
    std::string reason(error == nullptr ? "" : error);
    if (reason.empty()) {
        // No detail available; conservatively report a connectivity problem.
        return "Cannot reach server";
    }
    if (reason == "Failed to read certificate") {
        return "Certificate error";
    }
    if (reason == "Failed to open connection") {
        // DNS resolution, TCP connect or the TLS handshake failed.
        return "Cannot reach server";
    }
    if (reason == "Failed to get request headers") {
        return "Server did not respond";
    }
    if (reason == "Server response is not OK") {
        // Reachable, but the object isn't there (e.g. no apps published for this version).
        return "No apps available for this version";
    }
    return error;
}

bool showApps(Context* ctx) {
    if (ctx->contentWrapper == nullptr) {
        return false;
    }
    lv_obj_clean(ctx->contentWrapper);
    ctx->mutex.lock();
    if (parseJson(ctx->cachedAppsJsonFile, ctx->entries)) {
        std::ranges::sort(ctx->entries, [](auto left, auto right) {
            return left.appName < right.appName;
        });

        auto* list = lv_list_create(ctx->contentWrapper);
        lv_obj_set_style_pad_all(list, 0, LV_STATE_DEFAULT);
        lv_obj_set_size(list, LV_PCT(100), LV_SIZE_CONTENT);
        for (int i = 0; i < ctx->entries.size(); i++) {
            auto& entry = ctx->entries[i];
            LOG_I(TAG, "Adding %s", entry.appName.c_str());
            AppManifest manifest;
            const char* icon = app_manager_find_manifest(entry.appId.c_str(), &manifest) == ERROR_NONE ? LV_SYMBOL_OK : nullptr;
            auto* entry_button = lv_list_add_button(list, icon, entry.appName.c_str());
            auto int_as_voidptr = reinterpret_cast<void*>(i);
            lv_obj_set_user_data(entry_button, int_as_voidptr);
            lv_obj_add_event_cb(entry_button, onAppPressed, LV_EVENT_SHORT_CLICKED, ctx);
        }
        ctx->mutex.unlock();
        return true;
    }
    showRefreshFailedError(ctx, "Failed to load content");
    ctx->mutex.unlock();
    return false;
}

void refresh(Context* ctx, bool force) {
    if (ctx->contentWrapper == nullptr || ctx->refreshButton == nullptr) {
        return;
    }

    lv_obj_clean(ctx->contentWrapper);
    auto* spinner = lvgl_spinner_create(ctx->contentWrapper);
    lv_obj_align(spinner, LV_ALIGN_CENTER, 0, 0);

    lv_obj_add_flag(ctx->refreshButton, LV_OBJ_FLAG_HIDDEN);

    if (service::wifi::getRadioState() != service::wifi::RadioState::ConnectionActive) {
        showNoInternet(ctx);
        return;
    }

    // Show a valid cached manifest immediately and only re-download on the first load (no
    // cache) or an explicit refresh. Re-downloading on every re-open exhausts the C5's limited
    // internal RAM - each download allocates TLS/mbedTLS buffers from it - so a later download
    // fails its TLS handshake and reports "Cannot reach server".
    const bool cachedShown = file::isFile(ctx->cachedAppsJsonFile) && showApps(ctx);
    if (!force && cachedShown) {
        lv_obj_remove_flag(ctx->refreshButton, LV_OBJ_FLAG_HIDDEN);
        return;
    }

    // These callbacks run on the main dispatcher and reach back into this app's widgets via the
    // captured ctx pointer - same convention as AppHubDetailsApp.cpp's download callback for the
    // sibling "install/update" flow.
    const uint32_t refreshGeneration = ctx->uiGeneration.load();
    tryRefreshVersion(ctx, refreshGeneration, apphub::getAppsJsonUrlCandidates(), 0);
}

// Tries the manifest URLs newest-first. When a URL is reachable but the manifest isn't there
// (e.g. this OS version isn't published yet, so the CDN answers 404), it moves on to the next
// older version. Other failures (DNS/TCP/TLS) are shown as-is because they may be a genuine
// connectivity problem rather than a missing manifest.
void tryRefreshVersion(Context* ctx, uint32_t refreshGeneration, const std::vector<std::string>& urls, size_t index) {
    if (index >= urls.size()) {
        // Every candidate was reachable but had no content: this version isn't published yet.
        lvgl_lock();
        if (ctx->uiGeneration.load() == refreshGeneration && ctx->contentWrapper != nullptr && ctx->refreshButton != nullptr) {
            showRefreshFailedError(ctx, "No apps available for this version");
        }
        lvgl_unlock();
        return;
    }

    const std::string url = urls[index];
    network::http::download(
        url,
        CERTIFICATE_PATH,
        ctx->cachedAppsJsonFile,
        [ctx, refreshGeneration] {
            LOG_I(TAG, "Request success");
            lvgl_lock();
            if (ctx->uiGeneration.load() == refreshGeneration && ctx->contentWrapper != nullptr) {
                showApps(ctx);
            }
            lvgl_unlock();
        },
        [ctx, refreshGeneration, urls, index](const char* error) {
            if (error != nullptr) {
                LOG_E(TAG, "Request failed (%zu): %s", index, error);
            } else {
                LOG_E(TAG, "Request failed (%zu): (null error)", index);
            }
            // The only failure worth stepping back a version is "the server was reached but
            // has no content for this version" (e.g. a 404). A connect/TLS/DNS failure is a
            // real problem, not a version problem, so it must not be masked by a fallback.
            if (error != nullptr && std::string(error) == "Server response is not OK") {
                if (ctx->uiGeneration.load() == refreshGeneration) {
                    return tryRefreshVersion(ctx, refreshGeneration, urls, index + 1);
                }
                return;
            }
            lvgl_lock();
            if (ctx->uiGeneration.load() == refreshGeneration && ctx->contentWrapper != nullptr && ctx->refreshButton != nullptr) {
                showRefreshFailedError(ctx, mapDownloadError(error));
            }
            lvgl_unlock();
        }
    );
}

void destroyWidgets(void* userData) {
    auto* ctx = static_cast<Context*>(userData);
    ctx->contentWrapper = nullptr;
    ctx->refreshButton = nullptr;
    ctx->uiGeneration.fetch_add(1);
}

void createWidgets(lv_obj_t* parent, void* userData) {
    auto* ctx = static_cast<Context*>(userData);

    lv_obj_set_flex_flow(parent, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_row(parent, 0, LV_STATE_DEFAULT);

    auto* toolbar = lvgl_toolbar_create(parent, "App Hub");
    // The global toolbar nav callback only knows how to stop old-model apps.
    lvgl_toolbar_set_nav_action(toolbar, LV_SYMBOL_CLOSE, onBackPressed, ctx);
    ctx->refreshButton = lvgl_toolbar_add_image_button_action(toolbar, LV_SYMBOL_REFRESH, onRefreshPressed, ctx);
    lv_obj_add_flag(ctx->refreshButton, LV_OBJ_FLAG_HIDDEN);

    ctx->contentWrapper = lv_obj_create(parent);
    lv_obj_set_width(ctx->contentWrapper, LV_PCT(100));
    lv_obj_set_flex_grow(ctx->contentWrapper, 1);
    lv_obj_set_style_pad_all(ctx->contentWrapper, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_pad_ver(ctx->contentWrapper, 0, LV_STATE_DEFAULT);

    // Invalidate any in-flight async callback captured before this UI tree existed.
    ctx->uiGeneration.fetch_add(1);
    refresh(ctx, false);
}

int32_t appMain(uint32_t appInstanceId, int argc, char* argv[]) {
    Context ctx;
    ctx.appInstanceId = appInstanceId;

    TaskEventGroup event_group;
    task_event_group_construct(&event_group);
    AppEventSubscription sub {};
    app_event_subscribe(&sub, &event_group);

    WindowId window = window_manager_create_ext(appInstanceId, createWidgets, destroyWidgets, &ctx);

    bool shouldClose = false;
    while (!shouldClose) {
        task_event_group_wait_any(&event_group, nullptr, portMAX_DELAY);
        AppEvent event {};
        while (app_event_poll(&sub, &event) == ERROR_NONE) {
        switch (event.type) {
            case APP_EVENT_CLOSE:
                app_manager_finish(appInstanceId);
                shouldClose = true;
                break;
            default:
                break;
        }
        }
    }

    ctx.uiGeneration.fetch_add(1);
    window_manager_remove(window);
    app_event_unsubscribe(&sub);
    task_event_group_destruct(&event_group);

    return 0;
}

} // namespace

extern const ::AppManifest manifest = {
    .id = "AppHub",
    .name = "App Hub",
    .category = APP_CATEGORY_SYSTEM,
    .location = { APP_LOCATION_MEMORY, reinterpret_cast<void*>(appMain) }
};

} // namespace
