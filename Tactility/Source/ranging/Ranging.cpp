// SPDX-License-Identifier: Apache-2.0
#include <Tactility/ranging/Ranging.h>

#include <Tactility/DeprecatedPaths.h>
#include <Tactility/file/File.h>
#include <Tactility/file/PropertiesFile.h>

#include <tactility/log.h>

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <string>

namespace tt::ranging {

constexpr auto* TAG = "Ranging";

namespace {

constexpr auto* FILE_NAME = "ranging.properties";
// The Wi-Fi-only file this replaced. Read as a fallback so a calibration made before the BLE
// reference existed is not silently discarded.
constexpr auto* LEGACY_FILE_NAME = "wifi-ranging.properties";

constexpr auto* KEY_WIFI_2G = "wifiRssiAt1m2Ghz";
constexpr auto* KEY_WIFI_5G = "wifiRssiAt1m5Ghz";
constexpr auto* KEY_BLE = "bleRssiAt1m";
constexpr auto* KEY_EXPONENT = "pathLossExponent";
constexpr auto* KEY_SIGMA = "sigmaDb";
constexpr auto* KEY_WIFI_REF_UNCERTAINTY = "wifiReferenceUncertaintyDb";
constexpr auto* KEY_BLE_REF_UNCERTAINTY = "bleReferenceUncertaintyDb";

// Legacy keys, as the Wi-Fi-only file spelled them.
constexpr auto* LEGACY_KEY_WIFI_2G = "rssiAt1m2Ghz";
constexpr auto* LEGACY_KEY_WIFI_5G = "rssiAt1m5Ghz";

std::string getSettingsFilePath() {
    return getDataPath() + "/settings/" + FILE_NAME;
}

std::string getLegacySettingsFilePath() {
    return getDataPath() + "/settings/" + LEGACY_FILE_NAME;
}

// Written once, only when neither file exists. The properties reader skips "#" lines, so the comments
// survive editing by hand - but they do not survive a write through the properties writer, which is
// why nothing in this module ever saves the file back.
constexpr auto* TEMPLATE =
    "# RSSI ranging: the constants a distance band is computed from.\n"
    "#\n"
    "# Calibrating means measuring, not guessing. Stand 1 m from the device (same\n"
    "# height, antenna clear), read its RSSI in the Wi-Fi or Bluetooth app, and put\n"
    "# that number in the matching line below. Repeat at 3 m and the pair gives the\n"
    "# exponent as well:\n"
    "#\n"
    "#   n = (rssiAt1m - rssiAt3m) / (10 * log10(3))\n"
    "#\n"
    "# Outdoors or clear line of sight, n is close to 2.0; a furnished room is\n"
    "# 2.7-3.5. sigmaDb is how much one reading wanders: measure the same spot\n"
    "# twenty times and use the standard deviation, typically 4-8 dB indoors. It is\n"
    "# what the +/- factor in the displayed band comes from, so setting it lower\n"
    "# only makes the display more confident, not the estimate better.\n"
    "#\n"
    "# Why BLE needs its own line and a Wi-Fi access point mostly does not: a base\n"
    "# station's transmit power is regulated and roughly known, while a keyboard's\n"
    "# is whatever its antenna and the hand around it allow. Measured on one BLE\n"
    "# keyboard: about -80 dBm at 1 m, some 40 dB below what a 0 dBm transmitter\n"
    "# with a good antenna would give. A BLE peer that advertises its TX Power\n"
    "# Level needs none of this - its path loss is known directly.\n"
    "#\n"
    "# The two reference-uncertainty lines are how wrong the reference itself might\n"
    "# be, and they matter as much as sigmaDb: the displayed band adds them in\n"
    "# quadrature, so an uncalibrated BLE band comes out visibly wider than a Wi-Fi\n"
    "# one. That is deliberate - a band must not claim a confidence the reference\n"
    "# cannot support. Once you have measured the reference at 1 m, lower the\n"
    "# matching line to 2-3.\n"
    "\n"
    "wifiRssiAt1m2Ghz=-30\n"
    "wifiRssiAt1m5Ghz=-36\n"
    "bleRssiAt1m=-65\n"
    "pathLossExponent=2.7\n"
    "sigmaDb=6\n"
    "wifiReferenceUncertaintyDb=4\n"
    "bleReferenceUncertaintyDb=10\n";

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

    std::string path = getSettingsFilePath();
    const bool legacy = !file::isFile(path) && file::isFile(getLegacySettingsFilePath());
    if (legacy) {
        LOG_I(TAG, "%s not found - reading the Wi-Fi-only %s, which has no BLE reference",
            FILE_NAME, LEGACY_FILE_NAME);
        path = getLegacySettingsFilePath();
    }

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
    const char* key_wifi_2g = legacy ? LEGACY_KEY_WIFI_2G : KEY_WIFI_2G;
    const char* key_wifi_5g = legacy ? LEGACY_KEY_WIFI_5G : KEY_WIFI_5G;
    calibration.wifi2GhzAt1m = parseFloat(map, key_wifi_2g, calibration.wifi2GhzAt1m, -100.0f, -5.0f);
    calibration.wifi5GhzAt1m = parseFloat(map, key_wifi_5g, calibration.wifi5GhzAt1m, -100.0f, -5.0f);
    calibration.bleAt1m = parseFloat(map, KEY_BLE, calibration.bleAt1m, -100.0f, -5.0f);
    calibration.pathLossExponent = parseFloat(map, KEY_EXPONENT, calibration.pathLossExponent, 1.5f, 5.0f);
    calibration.sigmaDb = parseFloat(map, KEY_SIGMA, calibration.sigmaDb, 0.5f, 20.0f);
    calibration.wifiReferenceUncertaintyDb = parseFloat(map, KEY_WIFI_REF_UNCERTAINTY,
        calibration.wifiReferenceUncertaintyDb, 0.0f, 30.0f);
    calibration.bleReferenceUncertaintyDb = parseFloat(map, KEY_BLE_REF_UNCERTAINTY,
        calibration.bleReferenceUncertaintyDb, 0.0f, 30.0f);

    LOG_I(TAG, "Calibration from %s: Wi-Fi %g dBm @1m (2.4 GHz), %g dBm @1m (5 GHz), BLE %g dBm @1m, "
        "n=%g, sigma=%g dB, reference uncertainty %g/%g dB", legacy ? LEGACY_FILE_NAME : FILE_NAME,
        (double)calibration.wifi2GhzAt1m, (double)calibration.wifi5GhzAt1m, (double)calibration.bleAt1m,
        (double)calibration.pathLossExponent, (double)calibration.sigmaDb,
        (double)calibration.wifiReferenceUncertaintyDb, (double)calibration.bleReferenceUncertaintyDb);

    return calibration;
}

namespace {

// The shared tail of every estimate: turn a reference level and a reading into a distance, band it,
// and carry the uncertainty through.
//
// `referenceUncertaintyDb` is how wrong the reference itself might be, and it is added to the
// receiver's own spread in quadrature before the range is computed: a band that only counted the
// receiver would claim a confidence the reference cannot support. See the note on the field.
Estimate estimateFromReference(float referenceAt1m, float rssiDbm, const Calibration& calibration,
                               float referenceUncertaintyDb) {
    // Clamped so a missing or nonsense RSSI cannot produce an enormous exponent; 0.05 m to 1 km is
    // already far past anything the model says anything useful about.
    const float exponent = (referenceAt1m - rssiDbm) / (10.0f * calibration.pathLossExponent);
    float meters = std::pow(10.0f, exponent);
    if (meters < 0.05f) meters = 0.05f;
    if (meters > 1000.0f) meters = 1000.0f;

    // 1 sigma, in dB, then the same arithmetic with the RSSI moved by it.
    const float total_sigma_db = std::sqrt(
        calibration.sigmaDb * calibration.sigmaDb + referenceUncertaintyDb * referenceUncertaintyDb);
    const float sigma_factor = std::pow(10.0f, total_sigma_db / (10.0f * calibration.pathLossExponent));

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

} // namespace

Estimate estimate(float rssiDbm, Radio radio, const Calibration& calibration) {
    float reference;
    float reference_uncertainty;
    switch (radio) {
        case Radio::Wifi5Ghz:
            reference = calibration.wifi5GhzAt1m;
            reference_uncertainty = calibration.wifiReferenceUncertaintyDb;
            break;
        case Radio::Wifi2Ghz:
            reference = calibration.wifi2GhzAt1m;
            reference_uncertainty = calibration.wifiReferenceUncertaintyDb;
            break;
        case Radio::Ble:
        default:
            reference = calibration.bleAt1m;
            reference_uncertainty = calibration.bleReferenceUncertaintyDb;
            break;
    }
    return estimateFromReference(reference, rssiDbm, calibration, reference_uncertainty);
}

Radio wifiRadioForChannel(int32_t channel) {
    return (channel >= 32) ? Radio::Wifi5Ghz : Radio::Wifi2Ghz;
}

Estimate estimateBleWithTxPower(float rssiDbm, int8_t txPower, const Calibration& calibration) {
    if (txPower == 0x7F) {
        // "Not present" in the TX Power Level field. An empty band is the honest answer, and it lets
        // the caller tell "no reading" from "no reference".
        Estimate empty;
        empty.meters = 0.0f;
        empty.lowMeters = 0.0f;
        empty.highMeters = 0.0f;
        empty.band = "";
        return empty;
    }

    // Free-space loss at 2.45 GHz over 1 m: 20*log10(1) + 40.2 = 40.2 dB. So a peer transmitting at
    // txPower dBm reads txPower - 40.2 dBm at a metre, which is the reference the model wants - no
    // calibration walk involved, because the peer told us what it transmits.
    //
    // Its uncertainty is small but not zero: the field is an integer dBm and a peer may round it, and
    // its antenna gain is unknown. 2 dB covers that, against 10 dB for a peer that says nothing.
    constexpr float FREE_SPACE_LOSS_AT_1M = 40.2f;
    constexpr float ADVERTISED_TX_POWER_UNCERTAINTY_DB = 2.0f;
    return estimateFromReference(static_cast<float>(txPower) - FREE_SPACE_LOSS_AT_1M, rssiDbm,
                                 calibration, ADVERTISED_TX_POWER_UNCERTAINTY_DB);
}

void formatBand(const Estimate& value, char* out, size_t outSize) {
    const float sigma_factor = (value.lowMeters > 0.0f) ? (value.meters / value.lowMeters) : 1.0f;
    std::snprintf(out, outSize, "%s  %.1f-%.1f m  +/-%.1fx",
        value.band, (double)value.lowMeters, (double)value.highMeters, (double)sigma_factor);
}

void formatBandCompact(const Estimate& value, char* out, size_t outSize) {
    const float sigma_factor = (value.lowMeters > 0.0f) ? (value.meters / value.lowMeters) : 1.0f;
    std::snprintf(out, outSize, "~%.1f-%.1fm +/-%.1fx",
        (double)value.lowMeters, (double)value.highMeters, (double)sigma_factor);
}

void RssiFilter::add(int8_t rssi) {
    addFloat(static_cast<float>(rssi));
}

void RssiFilter::addFloat(float rssiDbm) {
    if (!valid) {
        valueDb = rssiDbm;
        valid = true;
        return;
    }
    valueDb += ALPHA * (rssiDbm - valueDb);
}

void RssiFilter::reset() {
    valueDb = 0.0f;
    valid = false;
}

} // namespace tt::ranging
