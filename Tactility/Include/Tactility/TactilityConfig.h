#pragma once

#ifdef ESP_PLATFORM
#include <sdkconfig.h>
#endif

#define TT_CONFIG_FORCE_ONSCREEN_KEYBOARD false // for development/debug purposes

#ifdef ESP_PLATFORM
// Screenshot capture (the screenshot service/app and the HTTP /api/screenshot endpoint) uses the
// vendored LVGL 100ask add-on (Libraries/lv_screenshot), whose lv_screenshot_create() wraps
// lv_snapshot_take(). Always enabled; CONFIG_LV_USE_SNAPSHOT is on for the C5.
#define TT_FEATURE_SCREENSHOT_ENABLED true
#define TT_FEATURE_SCREENSHOT_API true
#else // Sim
#define TT_FEATURE_SCREENSHOT_ENABLED true
#define TT_FEATURE_SCREENSHOT_API true
#endif

namespace tt::config {

constexpr auto SHOW_SYSTEM_PARTITION = false;

}
