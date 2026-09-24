#include <Tactility/bluetooth/BluetoothPairedDevice.h>

#include "Tactility/DeprecatedPaths.h"

#include <Tactility/file/File.h>
#include <Tactility/file/PropertiesFile.h>
#include <tactility/log.h>

#include <dirent.h>
#include <format>
#include <iomanip>
#include <sstream>
#include <string>
#include <cstdio>

namespace tt::bluetooth::settings {

constexpr auto* TAG = "BluetoothPairedDevice";

// Use the same directory as the old service for backward compatibility.
constexpr auto* DEVICE_SETTINGS_FORMAT = "{}/{}.device.properties";
constexpr auto* KEY_NAME        = "name";
constexpr auto* KEY_ADDR        = "addr";
constexpr auto* KEY_AUTO_CONNECT = "autoConnect";
constexpr auto* KEY_PROFILE_ID  = "profileId";

static std::string getSettingsFilePath() {
    return getDataPath() + "/service/bluetooth";
}

std::string addrToHex(const std::array<uint8_t, 6>& addr) {
    std::stringstream stream;
    stream << std::hex;
    for (int i = 0; i < 6; ++i) {
        stream << std::setw(2) << std::setfill('0') << static_cast<int>(addr[i]);
    }
    return stream.str();
}

static bool hexToAddr(const std::string& hex, std::array<uint8_t, 6>& addr) {
    if (hex.size() != 12) {
        LOG_E(TAG, "hexToAddr() length mismatch: expected 12, got %d", (int)hex.size());
        return false;
    }
    char buf[3] = { 0 };
    for (int i = 0; i < 6; ++i) {
        buf[0] = hex[i * 2];
        buf[1] = hex[i * 2 + 1];
        char* endptr = nullptr;
        addr[i] = static_cast<uint8_t>(strtoul(buf, &endptr, 16));
        if (endptr != buf + 2) {
            LOG_E(TAG, "hexToAddr() invalid hex at byte %d: '%c%c'", i, buf[0], buf[1]);
            return false;
        }
    }
    return true;
}

static std::string getFilePath(const std::string& addr_hex) {
    return std::format(DEVICE_SETTINGS_FORMAT, getSettingsFilePath(), addr_hex);
}

bool hasFileForDevice(const std::string& addr_hex) {
    return file::isFile(getFilePath(addr_hex));
}

bool load(const std::string& addr_hex, PairedDevice& device) {
    auto file_path = getFilePath(addr_hex);
    if (!file::isFile(file_path)) return false;

    std::map<std::string, std::string> map;
    if (!file::loadPropertiesFile(file_path, map)) {
        LOG_E(TAG, "Failed to read %s as a properties file", file_path.c_str());
        return false;
    }
    if (!map.contains(KEY_ADDR)) {
        LOG_E(TAG, "%s has no %s key", file_path.c_str(), KEY_ADDR);
        return false;
    }
    if (!hexToAddr(map[KEY_ADDR], device.addr)) {
        LOG_E(TAG, "%s has an unparsable addr '%s'", file_path.c_str(), map[KEY_ADDR].c_str());
        return false;
    }

    device.name = map.contains(KEY_NAME) ? map[KEY_NAME] : "";

    device.autoConnect = !map.contains(KEY_AUTO_CONNECT) || (map[KEY_AUTO_CONNECT] == "true");

    if (map.contains(KEY_PROFILE_ID)) {
        // TODO: Handle incorrect parsing input
        device.profileId = std::stoi(map[KEY_PROFILE_ID]);
    }
    return true;
}

bool save(const PairedDevice& device) {
    const auto addr_hex = addrToHex(device.addr);
    std::map<std::string, std::string> map;
    map[KEY_NAME]         = device.name;
    map[KEY_ADDR]         = addr_hex;
    map[KEY_AUTO_CONNECT] = device.autoConnect ? "true" : "false";
    map[KEY_PROFILE_ID]   = std::to_string(device.profileId);
    auto file_path = getFilePath(addr_hex);
    if (!file::findOrCreateParentDirectory(file_path, 0755)) {
        LOG_E(TAG, "Failed to create parent dir for %s", file_path.c_str());
        return false;
    }
    return file::savePropertiesFile(file_path, map);
}

bool remove(const std::string& addr_hex) {
    const auto file_path = getFilePath(addr_hex);
    if (!file::isFile(file_path)) return false;
    return ::remove(file_path.c_str()) == 0;
}

// Shared by loadAll() and loadByName(). The file name carries the address a record is filed under,
// which loadAll() drops - so the pairs are what both need, and neither should walk the directory
// itself.
static std::vector<std::pair<std::string, PairedDevice>> loadStoredDevices() {
    std::vector<std::pair<std::string, PairedDevice>> result;
    std::vector<dirent> entries;
    if (!file::isDirectory(getSettingsFilePath())) {
        return result;
    }
    file::scandir(getSettingsFilePath(), entries, [](const dirent* entry) -> int {
        if (entry->d_type != file::TT_DT_REG && entry->d_type != file::TT_DT_UNKNOWN) return -1;
        std::string name = entry->d_name;
        return name.ends_with(".device.properties") ? 0 : -1;
    }, nullptr);

    result.reserve(entries.size());
    constexpr std::string_view suffix = ".device.properties";
    for (const auto& entry : entries) {
        const std::string filename = entry.d_name;
        if (filename.size() <= suffix.size()) continue;
        const std::string addr_hex = filename.substr(0, filename.size() - suffix.size());
        PairedDevice device;
        if (load(addr_hex, device)) {
            result.emplace_back(addr_hex, std::move(device));
        }
    }
    return result;
}

bool loadByName(const std::string& name, PairedDevice& out_device, std::string& out_addr_hex) {
    if (name.empty()) {
        // Without this, an unnamed record would match every unnamed lookup - and the scan cache is
        // full of peers that never advertised a name.
        return false;
    }
    for (auto& [addr_hex, device] : loadStoredDevices()) {
        if (device.name == name) {
            out_device = device;
            out_addr_hex = addr_hex;
            return true;
        }
    }
    return false;
}

std::vector<PairedDevice> loadAll() {
    std::vector<PairedDevice> result;
    auto stored = loadStoredDevices();
    result.reserve(stored.size());
    for (auto& [addr_hex, device] : stored) {
        result.push_back(std::move(device));
    }
    return result;
}

} // namespace tt::bluetooth::settings
