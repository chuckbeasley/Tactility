#include <Tactility/service/wifi/WifiSettings.h>

#include <Tactility/file/File.h>
#include <Tactility/file/PropertiesFile.h>
#include <Tactility/service/ServicePaths.h>
#include <Tactility/service/wifi/WifiPrivate.h>

#include <tactility/log.h>

#include <map>

#ifdef ESP_PLATFORM
#include <nvs.h>
#endif

namespace tt::service::wifi::settings {

constexpr auto* TAG = "WifiSettings";
#ifndef ESP_PLATFORM
constexpr auto* SETTINGS_KEY_ENABLE_ON_BOOT = "enableOnBoot";
#endif
#ifdef ESP_PLATFORM
constexpr auto* NVS_NAMESPACE = "tt_wifi";
constexpr auto* NVS_KEY_ENABLE_ON_BOOT = "en_boot";
#endif

struct WifiSettings {
    bool enableOnBoot;
};

static WifiSettings cachedSettings {
    .enableOnBoot = false
};

static bool cached = false;

#ifndef ESP_PLATFORM
static bool hasWifiSettingsFile(std::shared_ptr<ServiceContext> context) {
    std::string settings_path = context->getPaths()->getUserDataPath("settings.properties");
    return file::isFile(settings_path);
}

static bool load(std::shared_ptr<ServiceContext> context, WifiSettings& settings) {
    std::map<std::string, std::string> map;
    std::string settings_path = context->getPaths()->getUserDataPath("settings.properties");
    if (!file::loadPropertiesFile(settings_path, map)) {
        return false;
    }

    if (!map.contains(SETTINGS_KEY_ENABLE_ON_BOOT)) {
        return false;
    }

    auto enable_on_boot_string = map[SETTINGS_KEY_ENABLE_ON_BOOT];
    settings.enableOnBoot = (enable_on_boot_string == "true");
    return true;
}

static bool save(std::shared_ptr<ServiceContext> context, const WifiSettings& settings) {
    std::map<std::string, std::string> map;
    map[SETTINGS_KEY_ENABLE_ON_BOOT] = settings.enableOnBoot ? "true" : "false";
    std::string settings_path = context->getPaths()->getUserDataPath("settings.properties");
    if (!file::findOrCreateParentDirectory(settings_path, 0755)) {
        LOG_E(TAG, "Failed to create %s", settings_path.c_str());
        return false;
    }
    return file::savePropertiesFile(settings_path, map);
}
#endif

#ifdef ESP_PLATFORM
static bool loadFromNvs(WifiSettings& settings) {
    nvs_handle_t handle = 0;
    const esp_err_t open_result = nvs_open(NVS_NAMESPACE, NVS_READONLY, &handle);
    if (open_result != ESP_OK) {
        return false;
    }

    uint8_t value = 0;
    const esp_err_t get_result = nvs_get_u8(handle, NVS_KEY_ENABLE_ON_BOOT, &value);
    nvs_close(handle);
    if (get_result != ESP_OK) {
        return false;
    }

    settings.enableOnBoot = (value != 0);
    return true;
}

static bool saveToNvs(const WifiSettings& settings) {
    nvs_handle_t handle = 0;
    const esp_err_t open_result = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &handle);
    if (open_result != ESP_OK) {
        LOG_E(TAG, "Failed to open NVS namespace %s (%d)", NVS_NAMESPACE, (int)open_result);
        return false;
    }

    const esp_err_t set_result = nvs_set_u8(handle, NVS_KEY_ENABLE_ON_BOOT, settings.enableOnBoot ? 1 : 0);
    if (set_result != ESP_OK) {
        LOG_E(TAG, "Failed to set NVS key %s (%d)", NVS_KEY_ENABLE_ON_BOOT, (int)set_result);
        nvs_close(handle);
        return false;
    }

    const esp_err_t commit_result = nvs_commit(handle);
    nvs_close(handle);
    if (commit_result != ESP_OK) {
        LOG_E(TAG, "Failed to commit NVS key %s (%d)", NVS_KEY_ENABLE_ON_BOOT, (int)commit_result);
        return false;
    }
    return true;
}
#endif

WifiSettings getCachedOrLoad() {
    if (!cached) {
#ifdef ESP_PLATFORM
        if (!loadFromNvs(cachedSettings)) {
            LOG_I(TAG, "No NVS WiFi boot setting, using defaults");
        }
        cached = true;
#else
        auto context = findServiceContext();
        if (context && hasWifiSettingsFile(context)) {
            if (load(context, cachedSettings)) {
                cached = true;
            } else {
                LOG_I(TAG, "Failed to load settings, using defaults");
            }
        }
#endif
    }

    return cachedSettings;
}

void setEnableOnBoot(bool enable) {
    cachedSettings.enableOnBoot = enable;
    cached = true;
#ifdef ESP_PLATFORM
    if (!saveToNvs(cachedSettings)) {
        LOG_E(TAG, "Failed to save settings in NVS");
    }
#else
    auto context = findServiceContext();
    if (context && !save(context, cachedSettings)) {
        LOG_E(TAG, "Failed to save settings");
    }
#endif
}

bool shouldEnableOnBoot() {
    return getCachedOrLoad().enableOnBoot;
}

} // namespace
