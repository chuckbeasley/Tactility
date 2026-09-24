// SPDX-License-Identifier: Apache-2.0
#pragma once

/**
 * @brief Turns an RSSI into a distance *band*, and says how uncertain it is.
 *
 * RSSI ranging is a model, not a measurement, and the model's error is large enough that a single
 * number is a lie: indoors the same stationary peer reads across 10-20 dB, which is a factor of two
 * to four in distance. What this offers instead is the honest version of the same arithmetic - a
 * coarse band ("same room"), the distance range that band came from, and the factor it is uncertain
 * by - so a caller cannot accidentally present an estimate as a measurement.
 *
 * The model is the usual log-distance path loss:
 *
 *     d = 10 ^ ((rssiAt1m - rssi) / (10 * pathLossExponent))
 *
 * with the uncertainty propagated from the spread of an indoor RSSI reading (sigmaDb):
 *
 *     d * 10^(-sigma / (10n))  ..  d * 10^(+sigma / (10n))
 *
 * Two radios use this, and they differ in one important way. A Wi-Fi access point is a base station
 * whose transmit power is regulated and roughly known, so its reference is a reasonable default. A
 * BLE peer is a keyboard or a tag with a small antenna and a hand near it, whose transmit power is
 * whatever it feels like - and most of them do not advertise it (the TX Power Level field is absent,
 * 0x7F, in the common case) - so its reference has to be measured. That is what the calibration file
 * is for, and why there is a separate reference per radio.
 */

#include <cstddef>
#include <cstdint>

namespace tt::ranging {

/** Which radio a reading came from, which selects the reference level. */
enum class Radio {
    /** BLE, 2.4 GHz only. */
    Ble,
    /** Wi-Fi, 2.4 GHz. */
    Wifi2Ghz,
    /** Wi-Fi, 5 GHz - about 6 dB more loss at the same distance. */
    Wifi5Ghz,
};

/**
 * The fitted constants, one reference per radio.
 *
 * Wi-Fi defaults are for a base station seen by this board's antenna: 5 GHz referenced 6 dB below
 * 2.4 GHz because free-space loss at the same distance is about that much higher, and an indoor
 * path-loss exponent of 2.7 sitting between free space (2.0) and a furnished room (3.5).
 *
 * The BLE default is a guess with a reason rather than a measurement: a peer transmitting 0 dBm
 * would read about -40 dBm at 1 m in free space, and real keyboard-class peers land well below that
 * because of their antenna and how they are held. Measured here, one keyboard reads about -80 dBm at
 * 1 m, so its default is deliberately pessimistic - a band that says "further than it is" is safer
 * than one that promises a distance the radio cannot support.
 */
struct Calibration {
    float wifi2GhzAt1m = -30.0f;
    float wifi5GhzAt1m = -36.0f;
    float bleAt1m = -65.0f;
    /** Path-loss exponent: 2.0 free space, 2.7-3.5 typical indoors. */
    float pathLossExponent = 2.7f;
    /** Spread of one indoor RSSI reading, dB (1 sigma). 4-8 dB is the usual range. */
    float sigmaDb = 6.0f;
    /**
     * How wrong the *reference* may be, dB (1 sigma), per radio - and the reason the displayed
     * uncertainty is not just sigmaDb.
     *
     * sigmaDb describes the receiver: two readings of a stationary peer differ. The reference
     * describes the transmitter, and for a Wi-Fi access point it is nearly knowable (EIRP is
     * regulated and the vendor sets it within a few dB), while for a BLE peer it very much is not: a
     * keyboard that does not advertise its TX power can sit 10-40 dB below a nominal 0 dBm part
     * because of its antenna and the hand around it. Measured on one such keyboard: about -80 dBm at
     * 1 m where free space from 0 dBm would predict -40.
     *
     * The two add in quadrature, so an uncalibrated BLE band comes out visibly wider than a Wi-Fi
     * one - which is the honest outcome. Calibrating the reference is what narrows it, and that is
     * also the signal to lower this value (a measured reference deserves 2-3 dB).
     */
    float wifiReferenceUncertaintyDb = 4.0f;
    float bleReferenceUncertaintyDb = 10.0f;
};

/**
 * Reads the calibration from `<data>/settings/ranging.properties`, writing a commented template there
 * the first time so the knobs are discoverable. Missing or unparsable values fall back to the
 * defaults above, and the file is never required to exist.
 *
 * The file this replaced (`wifi-ranging.properties`, Wi-Fi only) is still read when the new one is
 * absent, so a calibration somebody already made is not thrown away by the rename.
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
 * @param[in] radio which reference level to use
 * @param[in] calibration the fitted constants
 */
Estimate estimate(float rssiDbm, Radio radio, const Calibration& calibration);

/** Maps a Wi-Fi channel to its band's reference: 14 is the top of 2.4 GHz, 5 GHz starts at 36. */
Radio wifiRadioForChannel(int32_t channel);
/**
 * Same, for a BLE peer that *does* advertise its TX power level - the case that needs no calibration,
 * because the path loss is then known directly: a peer transmitting at `txPower` reads
 * `txPower - 40.2` dBm at 1 m in free space (40.2 dB is the free-space loss at 2.45 GHz for 1 m), so
 * that is the reference this computes and feeds to the same model.
 *
 * @param[in] txPower the advertised TX power level in dBm; 0x7F means "not present"
 * @return an estimate whose band is empty when @a txPower is the not-present value
 */
Estimate estimateBleWithTxPower(float rssiDbm, int8_t txPower, const Calibration& calibration);

/**
 * Formats one line for a small display, e.g. "same room  1.4-4.6 m  +/-1.7x". The band comes first
 * because it is the part that is actually supported by the data; the range and the factor are there
 * so the reader can see how little that is.
 */
void formatBand(const Estimate& estimate, char* out, size_t outSize);

/**
 * The same numbers as @ref formatBand without the band's name, for a list row that already carries a
 * name, a percentage and a TX power: e.g. "~1.4-3.9m +/-1.7x". The uncertainty is not dropped for the
 * sake of room - it is the part that keeps the range from reading as a measurement.
 */
void formatBandCompact(const Estimate& estimate, char* out, size_t outSize);

/**
 * Exponential moving average of an RSSI stream, one sample per received frame or poll.
 *
 * With 4-6 dB of sample-to-sample variation, banding the raw value produces a label that changes
 * every frame. This smooths that out; it cannot remove the bias, which is exactly why the range is
 * shown next to the band rather than a single distance.
 */
class RssiFilter {
public:
    void add(int8_t rssi);
    /** Feeds a value that is not an integer dBm (a smoothed reading being re-smoothed, or a test). */
    void addFloat(float rssiDbm);
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

} // namespace tt::ranging
