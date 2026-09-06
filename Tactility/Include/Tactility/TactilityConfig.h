#pragma once

#ifdef ESP_PLATFORM
#include <sdkconfig.h>
#endif

#define TT_CONFIG_FORCE_ONSCREEN_KEYBOARD false // for development/debug purposes

#ifdef ESP_PLATFORM
// The automatic screenshot *service/app* (timed / per-app captures) relies on the LVGL 100ask
// screenshot add-on (lv_screenshot_create), which is not bundled with this build, so it stays off.
#define TT_FEATURE_SCREENSHOT_ENABLED false
// The HTTP /api/screenshot endpoint uses LVGL's built-in snapshot API (lv_snapshot_take) instead,
// available when CONFIG_LV_USE_SNAPSHOT is enabled.
#define TT_FEATURE_SCREENSHOT_API (CONFIG_LV_USE_SNAPSHOT == 1)
#else // Sim
#define TT_FEATURE_SCREENSHOT_ENABLED true
#define TT_FEATURE_SCREENSHOT_API true
#endif

namespace tt::config {

constexpr auto SHOW_SYSTEM_PARTITION = false;

}
