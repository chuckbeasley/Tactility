// SPDX-License-Identifier: Apache-2.0
#include <Tactility/wifi/WifiRanging.h>

#include <Tactility/DeprecatedPaths.h>
#include <Tactility/file/File.h>
#include <Tactility/file/PropertiesFile.h>

#include <tactility/log.h>

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <string>

namespace tt::wifi::ranging {

constexpr auto* TAG = "WifiRanging";

namespace {

constexpr auto* FILE_NAME = "wifi-ranging.properties";
constexpr auto* KEY_RSSI_1M_2G = "rssiAt1m2Ghz";
constexpr auto* KEY_RSSI_1M_5G = "rssiAt1m5Ghz";
constexpr auto* KEY_EXPONENT = "pathLossExponent";
constexpr auto* KEY_SIGMA = "sigmaDb";

std::string getSettingsFilePath() {
    return getDataPath() + "/settings/" + FILE_NAME;
}

// Written once, only when the file is absent. The properties reader skips "#" lines, so the comments
// survive editing by hand - but they do not survive a write through the properties writer, which is
// why nothing in this module ever saves the file back.
constexpr auto* TEMPLATE =
    "# Wi-Fi RSSI ranging: the constants the distance band is computed from.\n"
    "#\n"
    "# Calibrating means measuring, not guessing: stand 1 m from the access point\n"
    "# (same height, antenna clear), read its RSSI in the Wi-Fi app, and put that\n"
    "# number here. Repeat at 3 m and the pair tells you the exponent as well:\n"
    "#\n"
    "#   n = (rssiAt1m - rssiAt3m) / (10 * log10(3))\n"
    "#\n"
    "# Outdoors or clear line of sight, n is close to 2.0; a furnished room is\n"
    "# 2.7-3.5. sigmaDb is how much one reading wanders: measure the same spot\n"
    "# twenty times and use the standard deviation, typically 4-8 dB indoors. It is\n"
    "# what the +/- factor in the displayed band comes from, so setting it lower\n"
    "# only makes the display more confident, not the estimate better.\n"
    "\n"
    "rssiAt1m2Ghz=-30\n"
    "rssiAt1m5Ghz=-36\n"
    "pathLossExponent=2.7\n"
    "sigmaDb=6\n";

float parseFloat(const std::map<std::string, std::string>& map, const char* key, float fallback,
                 float min, float max) {
    auto it = map.find(key);
    if (it == map.end()) return fallback;

    char* end = nullptr;
    const float value = std::strtof(it->second.c_str(), &end);
    if (end == it->second.c_str()) {
        LOG_W(TAG, "%s is not a number (\"%s\") - using %g", key, it->second.c_str(), (double)fallback);
        return fallback;
    }
    if (value < min || value > max) {
        LOG_W(TAG, "%s = %g is outside %g..%g - using %g", key, (double)value, (double)min,
            (double)max, (double)fallback);
        return fallback;
    }
    return value;
}

} // namespace

Calibration loadCalibration() {
    Calibration calibration;
    const std::string path = getSettingsFilePath();

    if (!file::isFile(path)) {
        // Publish the template so the constants are visible and editable in a known place, rather
        // than living only in this file's defaults.
        if (!file::writeString(path, TEMPLATE)) {
            LOG_W(TAG, "Could not write the calibration template to %s", path.c_str());
        }
        return calibration;
    }

    std::map<std::string, std::string> map;
    if (!file::loadPropertiesFile(path, map)) {
        LOG_W(TAG, "Could not read %s - using the built-in defaults", path.c_str());
        return calibration;
    }

    // Ranges are deliberately wide (they are sanity checks, not policy): a value inside them is the
    // user's calibration and is used as given, and a value outside them is more likely a typo than a
    // real measurement.
    calibration.rssiAt1m2Ghz = parseFloat(map, KEY_RSSI_1M_2G, calibration.rssiAt1m2Ghz, -90.0f, -5.0f);
    calibration.rssiAt1m5Ghz = parseFloat(map, KEY_RSSI_1M_5G, calibration.rssiAt1m5Ghz, -90.0f, -5.0f);
    calibration.pathLossExponent = parseFloat(map, KEY_EXPONENT, calibration.pathLossExponent, 1.5f, 5.0f);
    calibration.sigmaDb = parseFloat(map, KEY_SIGMA, calibration.sigmaDb, 0.5f, 20.0f);

    LOG_I(TAG, "Calibration: %g dBm @1m (2.4 GHz), %g dBm @1m (5 GHz), n=%g, sigma=%g dB",
        (double)calibration.rssiAt1m2Ghz, (double)calibration.rssiAt1m5Ghz,
        (double)calibration.pathLossExponent, (double)calibration.sigmaDb);

    return calibration;
}

Estimate estimate(float rssiDbm, int32_t channel, const Calibration& calibration) {
    // Channel 14 is the top of 2.4 GHz; 5 GHz starts at 36. Anything else (a caller that has no
    // channel) uses the 2.4 GHz reference, which is the more conservative of the two.
    const bool is_5ghz = channel >= 32;
    const float reference = is_5ghz ? calibration.rssiAt1m5Ghz : calibration.rssiAt1m2Ghz;

    // Clamped so a missing or nonsense RSSI cannot produce an enormous exponent; 0.05 m to 1 km is
    // already far past anything the model says anything useful about.
    const float exponent = (reference - rssiDbm) / (10.0f * calibration.pathLossExponent);
    float meters = std::pow(10.0f, exponent);
    if (meters < 0.05f) meters = 0.05f;
    if (meters > 1000.0f) meters = 1000.0f;

    // 1 sigma: the same arithmetic with the RSSI moved by the assumed spread of a reading.
    const float sigma_factor = std::pow(10.0f, calibration.sigmaDb / (10.0f * calibration.pathLossExponent));

    Estimate result;
    result.meters = meters;
    result.lowMeters = meters / sigma_factor;
    result.highMeters = meters * sigma_factor;

    // Coarse on purpose. These boundaries are about what a person can check by walking, not about
    // precision the measurement does not have.
    if (meters < 0.5f) {
        result.band = "touching";
    } else if (meters < 1.5f) {
        result.band = "within reach";
    } else if (meters < 5.0f) {
        result.band = "same room";
    } else if (meters < 15.0f) {
        result.band = "same floor";
    } else {
        result.band = "far / other room";
    }

    return result;
}

void formatBand(const Estimate& value, char* out, size_t outSize) {
    const float sigma_factor = (value.lowMeters > 0.0f) ? (value.meters / value.lowMeters) : 1.0f;
    std::snprintf(out, outSize, "%s  %.1f-%.1f m  +/-%.1fx",
        value.band, (double)value.lowMeters, (double)value.highMeters, (double)sigma_factor);
}

void RssiFilter::add(int8_t rssi) {
    if (!valid) {
        valueDb = static_cast<float>(rssi);
        valid = true;
        return;
    }
    valueDb += ALPHA * (static_cast<float>(rssi) - valueDb);
}

void RssiFilter::reset() {
    valueDb = 0.0f;
    valid = false;
}

} // namespace tt::wifi::ranging
