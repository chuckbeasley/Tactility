#include <Tactility/settings/TouchCalibrationSettings.h>

#include <Tactility/DeprecatedPaths.h>

#include <cstdlib>
#include <cerrno>
#include <climits>
#include <map>
#include <string>

#ifdef ESP_PLATFORM
#include <nvs.h>
#else
#include <Tactility/file/File.h>
#include <Tactility/file/PropertiesFile.h>
#endif

namespace tt::settings::touch {

#ifndef ESP_PLATFORM
static std::string getSettingsFilePath() {
    return getUserDataPath() + "/settings/touch-calibration.properties";
}
#endif

constexpr auto* SETTINGS_KEY_ENABLED = "enabled";
constexpr auto* SETTINGS_KEY_SCHEMA_VERSION = "schemaVersion";
constexpr auto* SETTINGS_KEY_X_MIN = "xMin";
constexpr auto* SETTINGS_KEY_X_MAX = "xMax";
constexpr auto* SETTINGS_KEY_Y_MIN = "yMin";
constexpr auto* SETTINGS_KEY_Y_MAX = "yMax";
constexpr auto* SETTINGS_KEY_ROTATE_XY = "rotateXy";
constexpr auto* SETTINGS_KEY_INVERT_X = "invertX";
constexpr auto* SETTINGS_KEY_INVERT_Y = "invertY";
constexpr int32_t CURRENT_CALIBRATION_SCHEMA_VERSION = 13;

#ifdef ESP_PLATFORM
constexpr auto* NVS_NAMESPACE = "tt_touch";
constexpr auto* NVS_KEY_ENABLED = "enabled";
constexpr auto* NVS_KEY_SCHEMA = "schema";
constexpr auto* NVS_KEY_XMIN = "xmin";
constexpr auto* NVS_KEY_XMAX = "xmax";
constexpr auto* NVS_KEY_YMIN = "ymin";
constexpr auto* NVS_KEY_YMAX = "ymax";
constexpr auto* NVS_KEY_ROTXY = "rotxy";
constexpr auto* NVS_KEY_INVX = "invx";
constexpr auto* NVS_KEY_INVY = "invy";
#endif

static bool toBool(const std::string& value) {
    return value == "1" || value == "true" || value == "True";
}

static bool parseInt32(const std::string& value, int32_t& out) {
    errno = 0;
    char* end_ptr = nullptr;
    const long parsed = std::strtol(value.c_str(), &end_ptr, 10);
    if (errno != 0 || end_ptr == value.c_str() || *end_ptr != '\0') {
        return false;
    }
    if (parsed < INT32_MIN || parsed > INT32_MAX) {
        return false;
    }
    out = static_cast<int32_t>(parsed);
    return true;
}

TouchCalibrationSettings getDefault() {
    return {
        .enabled = false,
        .schemaVersion = 0,
        .xMin = 0,
        .xMax = 0,
        .yMin = 0,
        .yMax = 0,
        .rotateXy = false,
        .invertX = false,
        .invertY = false,
    };
}

bool isValid(const TouchCalibrationSettings& settings) {
    constexpr auto MIN_RANGE = 20;
    return settings.xMax > settings.xMin && settings.yMax > settings.yMin &&
        (settings.xMax - settings.xMin) >= MIN_RANGE &&
        (settings.yMax - settings.yMin) >= MIN_RANGE;
}

bool load(TouchCalibrationSettings& settings) {
#ifdef ESP_PLATFORM
    nvs_handle_t handle = 0;
    if (nvs_open(NVS_NAMESPACE, NVS_READONLY, &handle) != ESP_OK) {
        return false;
    }

    TouchCalibrationSettings loaded = getDefault();

    uint8_t enabled = 0;
    if (nvs_get_u8(handle, NVS_KEY_ENABLED, &enabled) != ESP_OK) {
        nvs_close(handle);
        return false;
    }
    loaded.enabled = (enabled != 0);

    int32_t value32 = 0;
    if (nvs_get_i32(handle, NVS_KEY_SCHEMA, &value32) == ESP_OK) {
        loaded.schemaVersion = value32;
    }

    if (nvs_get_i32(handle, NVS_KEY_XMIN, &loaded.xMin) != ESP_OK ||
        nvs_get_i32(handle, NVS_KEY_XMAX, &loaded.xMax) != ESP_OK ||
        nvs_get_i32(handle, NVS_KEY_YMIN, &loaded.yMin) != ESP_OK ||
        nvs_get_i32(handle, NVS_KEY_YMAX, &loaded.yMax) != ESP_OK) {
        nvs_close(handle);
        return false;
    }

    uint8_t flag = 0;
    if (nvs_get_u8(handle, NVS_KEY_ROTXY, &flag) == ESP_OK) loaded.rotateXy = (flag != 0);
    if (nvs_get_u8(handle, NVS_KEY_INVX, &flag) == ESP_OK) loaded.invertX = (flag != 0);
    if (nvs_get_u8(handle, NVS_KEY_INVY, &flag) == ESP_OK) loaded.invertY = (flag != 0);
    nvs_close(handle);

    if (loaded.enabled && !isValid(loaded)) {
        return false;
    }

    settings = loaded;
    return true;
#else
    auto settings_path = getSettingsFilePath();
    if (!file::isFile(settings_path)) {
        return false;
    }

    std::map<std::string, std::string> map;
    if (!file::loadPropertiesFile(settings_path, map)) {
        return false;
    }

    auto enabled_it = map.find(SETTINGS_KEY_ENABLED);
    auto schema_version_it = map.find(SETTINGS_KEY_SCHEMA_VERSION);
    auto x_min_it = map.find(SETTINGS_KEY_X_MIN);
    auto x_max_it = map.find(SETTINGS_KEY_X_MAX);
    auto y_min_it = map.find(SETTINGS_KEY_Y_MIN);
    auto y_max_it = map.find(SETTINGS_KEY_Y_MAX);
    auto rotate_xy_it = map.find(SETTINGS_KEY_ROTATE_XY);
    auto invert_x_it = map.find(SETTINGS_KEY_INVERT_X);
    auto invert_y_it = map.find(SETTINGS_KEY_INVERT_Y);

    if (enabled_it == map.end() || x_min_it == map.end() || x_max_it == map.end() || y_min_it == map.end() || y_max_it == map.end()) {
        return false;
    }

    TouchCalibrationSettings loaded = getDefault();
    loaded.enabled = toBool(enabled_it->second);
    if (schema_version_it != map.end()) {
        if (!parseInt32(schema_version_it->second, loaded.schemaVersion)) {
            return false;
        }
    }
    if (!parseInt32(x_min_it->second, loaded.xMin) ||
        !parseInt32(x_max_it->second, loaded.xMax) ||
        !parseInt32(y_min_it->second, loaded.yMin) ||
        !parseInt32(y_max_it->second, loaded.yMax)) {
        return false;
    }
    if (rotate_xy_it != map.end()) loaded.rotateXy = toBool(rotate_xy_it->second);
    if (invert_x_it != map.end()) loaded.invertX = toBool(invert_x_it->second);
    if (invert_y_it != map.end()) loaded.invertY = toBool(invert_y_it->second);

    if (loaded.enabled && !isValid(loaded)) {
        return false;
    }

    settings = loaded;
    return true;
#endif
}

TouchCalibrationSettings loadOrGetDefault() {
    TouchCalibrationSettings settings;
    if (!load(settings)) {
        settings = getDefault();
    }
    return settings;
}

bool shouldRunCalibration() {
    const TouchCalibrationSettings settings = loadOrGetDefault();
    if (!settings.enabled || !isValid(settings)) {
        return true;
    }
    return settings.schemaVersion < CURRENT_CALIBRATION_SCHEMA_VERSION;
}

bool save(const TouchCalibrationSettings& settings) {
    if (settings.enabled && !isValid(settings)) {
        return false;
    }

#ifdef ESP_PLATFORM
    nvs_handle_t handle = 0;
    if (nvs_open(NVS_NAMESPACE, NVS_READWRITE, &handle) != ESP_OK) {
        return false;
    }

    bool ok = true;
    ok = ok && (nvs_set_u8(handle, NVS_KEY_ENABLED, settings.enabled ? 1 : 0) == ESP_OK);
    ok = ok && (nvs_set_i32(handle, NVS_KEY_SCHEMA, CURRENT_CALIBRATION_SCHEMA_VERSION) == ESP_OK);
    ok = ok && (nvs_set_i32(handle, NVS_KEY_XMIN, settings.xMin) == ESP_OK);
    ok = ok && (nvs_set_i32(handle, NVS_KEY_XMAX, settings.xMax) == ESP_OK);
    ok = ok && (nvs_set_i32(handle, NVS_KEY_YMIN, settings.yMin) == ESP_OK);
    ok = ok && (nvs_set_i32(handle, NVS_KEY_YMAX, settings.yMax) == ESP_OK);
    ok = ok && (nvs_set_u8(handle, NVS_KEY_ROTXY, settings.rotateXy ? 1 : 0) == ESP_OK);
    ok = ok && (nvs_set_u8(handle, NVS_KEY_INVX, settings.invertX ? 1 : 0) == ESP_OK);
    ok = ok && (nvs_set_u8(handle, NVS_KEY_INVY, settings.invertY ? 1 : 0) == ESP_OK);
    ok = ok && (nvs_commit(handle) == ESP_OK);
    nvs_close(handle);
    return ok;
#else
    std::map<std::string, std::string> map;
    map[SETTINGS_KEY_ENABLED] = settings.enabled ? "1" : "0";
    map[SETTINGS_KEY_SCHEMA_VERSION] = std::to_string(CURRENT_CALIBRATION_SCHEMA_VERSION);
    map[SETTINGS_KEY_X_MIN] = std::to_string(settings.xMin);
    map[SETTINGS_KEY_X_MAX] = std::to_string(settings.xMax);
    map[SETTINGS_KEY_Y_MIN] = std::to_string(settings.yMin);
    map[SETTINGS_KEY_Y_MAX] = std::to_string(settings.yMax);
    map[SETTINGS_KEY_ROTATE_XY] = settings.rotateXy ? "1" : "0";
    map[SETTINGS_KEY_INVERT_X] = settings.invertX ? "1" : "0";
    map[SETTINGS_KEY_INVERT_Y] = settings.invertY ? "1" : "0";

    auto settings_path = getSettingsFilePath();
    if (!file::findOrCreateParentDirectory(settings_path, 0755)) {
        return false;
    }

    return file::savePropertiesFile(settings_path, map);
#endif
}

} // namespace tt::settings::touch
