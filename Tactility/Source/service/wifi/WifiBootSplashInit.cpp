#include <Tactility/service/wifi/WifiBootSplashInit.h>

#include "Tactility/service/wifi/Wifi.h"
#include "Tactility/service/wifi/WifiSettings.h"

#include <Tactility/file/PropertiesFile.h>

#include <Tactility/MountPoints.h>
#include <Tactility/file/File.h>
#include <Tactility/service/wifi/WifiApSettings.h>

#include <Tactility/DeprecatedPaths.h>
#include <Tactility/Tactility.h>
#include <Tactility/kernel/Kernel.h>

#include <tactility/log.h>

#include <cstring>
#include <dirent.h>
#include <format>
#include <map>
#include <string>
#include <vector>

namespace tt::service::wifi {

constexpr auto* TAG = "WifiBootSplashInit";

constexpr auto* AP_PROPERTIES_KEY_SSID = "ssid";
constexpr auto* AP_PROPERTIES_KEY_PASSWORD = "password";
constexpr auto* AP_PROPERTIES_KEY_AUTO_CONNECT = "autoConnect";
constexpr auto* AP_PROPERTIES_KEY_CHANNEL = "channel";
constexpr auto* AP_PROPERTIES_KEY_AUTO_REMOVE = "autoRemovePropertiesFile";

struct ApProperties {
    std::string ssid;
    std::string password;
    bool autoConnect;
    int32_t channel;
    bool autoRemovePropertiesFile;
};

static void importWifiAp(const std::string& filePath) {
    std::map<std::string, std::string> map;
    if (!file::loadPropertiesFile(filePath, map)) {
        LOG_E(TAG, "Failed to load AP properties at %s", filePath.c_str());
        return;
    }

    const auto ssid_iterator = map.find(AP_PROPERTIES_KEY_SSID);
    if (ssid_iterator == map.end()) {
        LOG_E(TAG, "%s is missing ssid", filePath.c_str());
        return;
    }
    const auto ssid = ssid_iterator->second;

    if (!settings::contains(ssid)) {

        const auto password_iterator = map.find(AP_PROPERTIES_KEY_PASSWORD);
        const auto password = password_iterator == map.end() ? "" : password_iterator->second;

        const auto auto_connect_iterator = map.find(AP_PROPERTIES_KEY_AUTO_CONNECT);
        const auto auto_connect = auto_connect_iterator == map.end() ? true : (auto_connect_iterator->second == "true");

        const auto channel_iterator = map.find(AP_PROPERTIES_KEY_CHANNEL);
        const auto channel = channel_iterator == map.end() ? 0 : std::stoi(channel_iterator->second);

        settings::WifiApSettings settings(
            ssid,
            password,
            auto_connect,
            channel
        );

        if (!settings::save(settings)) {
            LOG_E(TAG, "Failed to save settings for %s", ssid.c_str());
        } else {
            LOG_I(TAG, "Imported %s from %s", ssid.c_str(), filePath.c_str());
        }
    }

    const auto auto_remove_iterator = map.find(AP_PROPERTIES_KEY_AUTO_REMOVE);
    if (auto_remove_iterator != map.end() && auto_remove_iterator->second == "true") {
        // std::remove() returns 0 on success - the previous check was inverted, so this logged
        // failure on success and vice versa.
        if (std::remove(filePath.c_str()) != 0) {
            LOG_E(TAG, "Failed to auto-remove %s", filePath.c_str());
        } else {
            LOG_I(TAG, "Auto-removed %s", filePath.c_str());
        }
    }
}

constexpr auto* AP_PROPERTIES_SUFFIX = ".ap.properties";

// Accepts the interrupted-save leftovers too. properties_file.cpp stages a save at "<path>.tmp"
// and parks the old content at "<path>.bak"; a reset mid-swap can leave one of those as the only
// copy. The importer renames such an orphan back into place before reading it - otherwise the
// provisioning file is invisible here and the AP is silently never imported.
static bool isApPropertiesName(const std::string& name) {
    return name.ends_with(AP_PROPERTIES_SUFFIX) ||
        name.ends_with(std::string(AP_PROPERTIES_SUFFIX) + ".tmp") ||
        name.ends_with(std::string(AP_PROPERTIES_SUFFIX) + ".bak");
}

static int apPropertiesFilter(const dirent* entry) {
    switch (entry->d_type) {
        case file::TT_DT_DIR:
        case file::TT_DT_CHR:
        case file::TT_DT_LNK:
            return -1;
        case file::TT_DT_REG:
        default:
            return isApPropertiesName(entry->d_name) ? 0 : -1;
    }
}

static void importWifiApSettingsFromDir(const std::string& path) {
    // The caller already established that this directory exists, so an empty listing is
    // anomalous. It has been observed to happen intermittently on the freshly mounted FATFS
    // volume during boot, and because the old code gave up silently on a zero result, WiFi
    // provisioning was skipped without a trace and auto-connect never happened. Retry over a
    // window of about a second before believing it, and never fail quietly.
    constexpr int MAX_ATTEMPTS = 5;
    constexpr uint32_t RETRY_DELAY_MS = 250;
    std::vector<dirent> dirent_list;
    int found = 0;

    for (int attempt = 1; attempt <= MAX_ATTEMPTS; attempt++) {
        dirent_list.clear();
        found = file::scandir(path, dirent_list, apPropertiesFilter, nullptr);
        if (found > 0) {
            break;
        }
        if (attempt < MAX_ATTEMPTS) {
            LOG_W(TAG, "Read no AP files at %s (attempt %d/%d), retrying", path.c_str(), attempt, MAX_ATTEMPTS);
            kernel::delayMillis(RETRY_DELAY_MS);
        }
    }

    if (found < 0) {
        LOG_E(TAG, "Failed to read %s", path.c_str());
        return;
    }

    if (dirent_list.empty()) {
        LOG_W(TAG, "No AP files found at %s", path.c_str());
        return;
    }

    for (auto& dirent : dirent_list) {
        std::string absolute_path = std::format("{}/{}", path, dirent.d_name);

        // Restore an interrupted save before importing, so the file is both readable now and
        // present under its real name for subsequent boots.
        for (const auto* suffix : { ".tmp", ".bak" }) {
            if (!absolute_path.ends_with(suffix)) {
                continue;
            }
            std::string real_path = absolute_path.substr(0, absolute_path.size() - std::strlen(suffix));
            if (file::isFile(real_path)) {
                // The real file survived after all - the leftover is stale, drop it.
                std::remove(absolute_path.c_str());
                absolute_path.clear();
            } else if (std::rename(absolute_path.c_str(), real_path.c_str()) == 0) {
                LOG_W(TAG, "Recovered %s from an interrupted save", real_path.c_str());
                absolute_path = real_path;
            }
            break;
        }

        if (absolute_path.empty()) {
            continue;
        }
        importWifiAp(absolute_path);
    }
}

void bootSplashInit() {
    LOG_I(TAG, "bootSplashInit dispatch");
    getMainDispatcher().dispatch([] {
        LOG_I(TAG, "bootSplashInit dispatch begin");
        // Import any provisioning files placed on the system data partition.
        const std::string provisioning_path = file::getChildPath(getUserDataPath(), "provisioning");
        if (file::isDirectory(provisioning_path)) {
            importWifiApSettingsFromDir(provisioning_path);
        } else {
            LOG_I(TAG, "Skip provisioning: no files at %s", provisioning_path.c_str());
        }

        // Dispatch WiFi on
        if (settings::shouldEnableOnBoot()) {
            LOG_I(TAG, "Auto-enabling WiFi");
            getMainDispatcher().dispatch([] -> void { setEnabled(true); });
        }

        LOG_I(TAG, "bootSplashInit dispatch end");
    });
}

}
