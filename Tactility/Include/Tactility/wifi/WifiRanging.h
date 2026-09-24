// SPDX-License-Identifier: Apache-2.0
#pragma once

/**
 * @brief Turns a Wi-Fi RSSI into a distance *band*, and says how uncertain it is.
 *
 * RSSI ranging is a model, not a measurement, and the model's error is large enough that a single
 * number is a lie: indoors the same stationary AP reads across 10-20 dB, which is a factor of two to
 * four in distance. What this offers instead is the honest version of the same arithmetic - a coarse
 * band ("same room"), the distance range that band came from, and the factor it is uncertain by - so
 * a caller cannot accidentally present an estimate as a measurement.
 *
 * The model is the usual log-distance path loss:
 *
 *     d = 10 ^ ((rssiAt1m - rssi) / (10 * pathLossExponent))
 *
 * with the uncertainty propagated from the spread of an indoor RSSI reading (sigmaDb):
 *
 *     d * 10^(-sigma / (10n))  ..  d * 10^(+sigma / (10n))
 *
 * Wi-Fi is the easier case of the two radios here because the transmitter is usually a base station
 * whose power is roughly known (and regulated), so the defaults below are usable without a
 * calibration walk. They are defaults, though: an AP with beamforming, a different antenna, or a
 * client rather than an AP all move the reference, which is what the calibration file is for.
 *
 * When a real measurement is available - 802.11mc FTM, which this board's radio supports - prefer it:
 * it measures a round-trip time rather than fitting a curve to a signal strength.
 */

#include <cstddef>
#include <cstdint>

namespace tt::wifi::ranging {

/**
 * The fitted constants. Defaults are for a base station seen by this board's antenna: 5 GHz is
 * referenced 6 dB lower than 2.4 GHz because free-space loss at the same distance is about that much
 * higher, and an indoor path-loss exponent of 2.7 sits between free space (2.0) and a furnished room
 * (3.5).
 */
struct Calibration {
    /** RSSI expected at 1 m from a 2.4 GHz access point, dBm. */
    float rssiAt1m2Ghz = -30.0f;
    /** RSSI expected at 1 m from a 5 GHz access point, dBm. */
    float rssiAt1m5Ghz = -36.0f;
    /** Path-loss exponent: 2.0 free space, 2.7-3.5 typical indoors. */
    float pathLossExponent = 2.7f;
    /** Spread of one indoor RSSI reading, dB (1 sigma). 4-8 dB is the usual range. */
    float sigmaDb = 6.0f;
};

/**
 * Reads the calibration from `<data>/settings/wifi-ranging.properties`, writing a commented template
 * there the first time so the knobs are discoverable. Missing or unparsable values fall back to the
 * defaults above, and the file is never required to exist.
 */
Calibration loadCalibration();

/** A distance estimate, its uncertainty, and the band it falls in. */
struct Estimate {
    /** Point estimate in metres. Deliberately not the thing to show a user on its own. */
    float meters = 0.0f;
    /** Lower end of the 1-sigma range, metres. */
    float lowMeters = 0.0f;
    /** Upper end of the 1-sigma range, metres. */
    float highMeters = 0.0f;
    /** Coarse band name, a static string: "touching" .. "far / other room". */
    const char* band = "";
};

/**
 * @param[in] rssiDbm a smoothed RSSI - a single reading varies far too much to band (see RssiFilter)
 * @param[in] channel the channel it was measured on, which selects the 2.4/5 GHz reference
 */
Estimate estimate(float rssiDbm, int32_t channel, const Calibration& calibration);

/**
 * Formats one line for a small display, e.g. "same room  1.4-4.6 m  +/-1.7x". The band comes first
 * because it is the part that is actually supported by the data; the range and the factor are there
 * so the reader can see how little that is.
 */
void formatBand(const Estimate& estimate, char* out, size_t outSize);

/**
 * Exponential moving average of an RSSI stream, one sample per received frame.
 *
 * With 4-6 dB of sample-to-sample variation, banding the raw value produces a label that changes
 * every frame. This smooths that out; it cannot remove the bias, which is exactly why the range is
 * shown next to the band rather than a single distance.
 */
class RssiFilter {
public:
    void add(int8_t rssi);
    void reset();
    bool hasValue() const { return valid; }
    float dBm() const { return valueDb; }

private:
    // ~5 samples of memory: fast enough to follow someone walking, slow enough to hold a band still
    // across the frame-to-frame jitter.
    static constexpr float ALPHA = 0.2f;
    float valueDb = 0.0f;
    bool valid = false;
};

} // namespace tt::wifi::ranging
