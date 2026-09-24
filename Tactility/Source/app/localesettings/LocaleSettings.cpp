#include <Tactility/Tactility.h>

#include <Tactility/RecursiveMutex.h>
#include <Tactility/StringUtils.h>
#include <Tactility/app/localesettings/TextResources.h>
#include <Tactility/settings/Language.h>
#include <Tactility/settings/SystemSettings.h>
#include <Tactility/units/Units.h>

#include <app/event.h>
#include <app/manager.h>
#include <app/manifest.h>
#include <app/scheduler.h>

#include <lvgl_window_manager/window_manager.h>

#include <tactility/check.h>

#include <lvgl/widgets/toolbar.h>

#include <lvgl.h>
#include <map>

namespace tt::app::localesettings {

constexpr auto* TAG = "LocaleSettings";

#ifdef ESP_PLATFORM
constexpr auto* TEXT_RESOURCE_PATH = "/system/app/LocaleSettings/i18n";
#else
constexpr auto* TEXT_RESOURCE_PATH = "system/app/LocaleSettings/i18n";
#endif

extern const ::AppManifest manifest;

namespace {

struct Context {
    uint32_t appInstanceId;
    tt::i18n::TextResources textResources = tt::i18n::TextResources(TEXT_RESOURCE_PATH);
    RecursiveMutex mutex;
    lv_obj_t* languageDropdown = nullptr;
    lv_obj_t* unitsDropdown = nullptr;
    bool settingsUpdated = false;

    std::map<settings::Language, std::string> languageMap;
};

/** The two unit systems, in the order the dropdown shows them. */
constexpr settings::UnitSystem UNIT_SYSTEMS[] = {
    settings::UnitSystem::Metric,
    settings::UnitSystem::Imperial,
};


std::string getLanguageOptions(Context* ctx) {
    std::vector<std::string> items;
    for (int i = 0; i < static_cast<int>(settings::Language::count); i++) {
        switch (static_cast<settings::Language>(i)) {
            case settings::Language::en_GB:
                items.push_back(ctx->textResources[i18n::Text::EN_GB]);
                break;
            case settings::Language::en_US:
                items.push_back(ctx->textResources[i18n::Text::EN_US]);
                break;
            case settings::Language::fr_FR:
                items.push_back(ctx->textResources[i18n::Text::FR_FR]);
                break;
            case settings::Language::nl_BE:
                items.push_back(ctx->textResources[i18n::Text::NL_BE]);
                break;
            case settings::Language::nl_NL:
                items.push_back(ctx->textResources[i18n::Text::NL_NL]);
                break;
            case settings::Language::count:
                break;
        }
    }
    return string::join(items, "\n");
}

void updateViews(Context* ctx) {
    ctx->textResources.load();

    std::string language_options = getLanguageOptions(ctx);
    lv_dropdown_set_options(ctx->languageDropdown, language_options.c_str());
    lv_dropdown_set_selected(ctx->languageDropdown, static_cast<uint32_t>(settings::getLanguage()));

    if (ctx->unitsDropdown != nullptr) {
        // Labels rather than the raw enum: the dropdown is what a user reads.
        std::string options = ctx->textResources[i18n::Text::METRIC] + "\n" +
                              ctx->textResources[i18n::Text::IMPERIAL];
        lv_dropdown_set_options(ctx->unitsDropdown, options.c_str());

        const auto current = units::getSystem();
        const uint32_t index = (current == UNIT_SYSTEMS[1]) ? 1 : 0;
        lv_dropdown_set_selected(ctx->unitsDropdown, index);
    }
}

void onUnitsSet(lv_event_t* event) {
    auto* ctx = static_cast<Context*>(lv_event_get_user_data(event));
    auto* dropdown = static_cast<lv_obj_t*>(lv_event_get_target(event));
    const uint32_t index = lv_dropdown_get_selected(dropdown);
    if (index >= sizeof(UNIT_SYSTEMS) / sizeof(UNIT_SYSTEMS[0])) {
        return;
    }

    const auto chosen = UNIT_SYSTEMS[index];
    units::setSystem(chosen);
    LOG_I(TAG, "Unit system set to %s", units::toString(chosen));

    // Deliberately no updateViews() here. That function re-sets the dropdown's options, and doing that
    // from inside the dropdown's own value-changed callback rebuilds the option list while LVGL is
    // iterating it - observed on the device as a list that grows a duplicate entry ("Metric, Imperial,
    // Imperial"). The selection is already displayed by the dropdown itself, and every screen that
    // shows a measurement reads the setting through tt::units when it renders, so there is nothing to
    // refresh. updateViews() is still what a *language* change calls, because then the option labels
    // themselves have changed and they belong to different dropdowns.
    (void)ctx;
}

void onLanguageSet(lv_event_t* event) {
    auto* ctx = static_cast<Context*>(lv_event_get_user_data(event));
    auto* dropdown = static_cast<lv_obj_t*>(lv_event_get_target(event));
    auto index = lv_dropdown_get_selected(dropdown);
    auto language = static_cast<settings::Language>(index);
    settings::setLanguage(language);

    updateViews(ctx);
}

// Preserved from the pre-conversion code as-is: declared but never wired to any widget there
// either, so this has always been dead code (kept verbatim rather than dropped, since removing
// it would be a functional judgment call outside the scope of this lifecycle-only conversion).
[[maybe_unused]] void onRegionChanged(lv_event_t* event) {
    auto* ctx = static_cast<Context*>(lv_event_get_user_data(event));
    ctx->settingsUpdated = true;
}

void onBackPressed(lv_event_t* event) {
    auto* ctx = static_cast<Context*>(lv_event_get_user_data(event));
    app_event_emit_close(ctx->appInstanceId);
}

void createWidgets(lv_obj_t* parent, void* userData) {
    auto* ctx = static_cast<Context*>(userData);
    ctx->textResources.load();

    lv_obj_set_flex_flow(parent, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_row(parent, 0, LV_STATE_DEFAULT);

    auto* toolbar = lvgl_toolbar_create(parent, "Region & Language");
    // The global toolbar nav callback only knows how to stop old-model apps.
    lvgl_toolbar_set_nav_action(toolbar, LV_SYMBOL_CLOSE, onBackPressed, ctx);

    auto* main_wrapper = lv_obj_create(parent);
    lv_obj_set_flex_flow(main_wrapper, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_width(main_wrapper, LV_PCT(100));
    lv_obj_set_flex_grow(main_wrapper, 1);

    // Language

    auto* language_wrapper = lv_obj_create(main_wrapper);
    lv_obj_set_width(language_wrapper, LV_PCT(100));
    lv_obj_set_height(language_wrapper, LV_SIZE_CONTENT);
    lv_obj_set_style_pad_all(language_wrapper, 8, 0);
    lv_obj_set_style_border_width(language_wrapper, 0, 0);

    auto* languageLabel = lv_label_create(language_wrapper);
    lv_label_set_text(languageLabel, ctx->textResources[i18n::Text::LANGUAGE].c_str());
    lv_obj_align(languageLabel, LV_ALIGN_LEFT_MID, 4, 0);

    ctx->languageDropdown = lv_dropdown_create(language_wrapper);
    // Generous width so longer language names (e.g. "English (United States)") aren't truncated and
    // the open option list stays on-screen when the dropdown is right-aligned.
    lv_obj_set_width(ctx->languageDropdown, LV_PCT(55));
    lv_obj_align(ctx->languageDropdown, LV_ALIGN_RIGHT_MID, 0, 0);
    std::string language_options = getLanguageOptions(ctx);
    lv_dropdown_set_options(ctx->languageDropdown, language_options.c_str());
    lv_dropdown_set_selected(ctx->languageDropdown, static_cast<uint32_t>(settings::getLanguage()));
    lv_obj_add_event_cb(ctx->languageDropdown, onLanguageSet, LV_EVENT_VALUE_CHANGED, ctx);

    // Units

    auto* units_wrapper = lv_obj_create(main_wrapper);
    lv_obj_set_width(units_wrapper, LV_PCT(100));
    lv_obj_set_height(units_wrapper, LV_SIZE_CONTENT);
    lv_obj_set_style_pad_all(units_wrapper, 8, 0);
    lv_obj_set_style_border_width(units_wrapper, 0, 0);

    auto* unitsLabel = lv_label_create(units_wrapper);
    lv_label_set_text(unitsLabel, ctx->textResources[i18n::Text::UNITS].c_str());
    lv_obj_align(unitsLabel, LV_ALIGN_LEFT_MID, 4, 0);

    ctx->unitsDropdown = lv_dropdown_create(units_wrapper);
    lv_obj_set_width(ctx->unitsDropdown, LV_PCT(55));
    lv_obj_align(ctx->unitsDropdown, LV_ALIGN_RIGHT_MID, 0, 0);
    std::string units_options = ctx->textResources[i18n::Text::METRIC] + "\n" +
                                ctx->textResources[i18n::Text::IMPERIAL];
    lv_dropdown_set_options(ctx->unitsDropdown, units_options.c_str());
    lv_dropdown_set_selected(ctx->unitsDropdown,
        units::getSystem() == settings::UnitSystem::Imperial ? 1 : 0);
    lv_obj_add_event_cb(ctx->unitsDropdown, onUnitsSet, LV_EVENT_VALUE_CHANGED, ctx);
}

int32_t appMain(int argc, char* argv[]) {
    uint32_t appInstanceId = app_scheduler_current_app_id();
    Context ctx;
    ctx.appInstanceId = appInstanceId;

    TaskEventGroup event_group {};
    task_event_group_construct(&event_group);

    AppEventSubscription sub {};
    check(app_event_subscribe(&sub, &event_group) == ERROR_NONE);

    WindowId window = window_manager_create(appInstanceId, createWidgets, &ctx);

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

    window_manager_remove(window);
    check(app_event_unsubscribe(&sub) == ERROR_NONE);
    task_event_group_destruct(&event_group);

    return 0;
}

} // namespace

extern const ::AppManifest manifest = {
    .id = "tactility.localesettings",
    .name = "Region & Language",
    .category = APP_CATEGORY_SETTINGS,
    .location = { APP_LOCATION_MEMORY, reinterpret_cast<void*>(appMain) }
};

} // namespace tt::app::localesettings
