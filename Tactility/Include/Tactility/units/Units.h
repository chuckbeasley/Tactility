// SPDX-License-Identifier: Apache-2.0
#pragma once

/**
 * @brief Metric or imperial, and the formatting that follows from it.
 *
 * The choice lives in the system settings (Region & Language owns it): `system.properties` on the
 * data partition, so an app asks this module rather than carrying its own preference. Everything a
 * user reads as a measurement goes through here, so that changing the setting changes all of it at
 * once - GPS altitude and speed, weather temperature, wind and pressure, and the RSSI distance bands.
 *
 * What deliberately does *not* convert: byte sizes (a KB is a KB), angles in degrees, and the SI
 * electrical units in the Power app (volts, milliamps). Those have no imperial counterpart to
 * convert to, and inventing one would be worse than leaving them alone.
 *
 * The conversions are exact scale factors, not approximations:
 *   1 m       = 3.280839895 ft
 *   1 km/h    = 0.621371192 mph
 *   1 kn      = 1.852 km/h
 *   °F        = °C * 9/5 + 32
 *   1 Pa      = 0.01 hPa = 0.00029529988 inHg
 */

#include <Tactility/settings/Units.h>

#include <cstddef>
#include <string>

namespace tt::units {

/** The choice itself is a settings value - see Tactility/settings/Units.h - used here as given. */
using settings::UnitSystem;

/** Reads the setting from the system settings; defaults to metric when it has never been set. */
UnitSystem getSystem();

/** Persists the setting and updates the in-process cache the formatters read. */
void setSystem(UnitSystem system);

/** True when the user asked for imperial units. */
bool isImperial();

/** @return "metric" or "imperial", for logs and any screen that names the current choice. */
const char* toString(UnitSystem system);

// ---- Distance ---------------------------------------------------------------------------------

/** e.g. "1.2 m" or "3.9 ft". */
std::string formatDistance(float metres, unsigned decimals = 1);

/** e.g. "0.3-2.1 m" or "1.0-6.9 ft" - the range form the RSSI bands use. */
std::string formatDistanceRange(float lowMetres, float highMetres, unsigned decimals = 1);

// ---- Speed ------------------------------------------------------------------------------------

/** From km/h: "12 km/h" or "7 mph". */
std::string formatSpeedFromKph(float kph, unsigned decimals = 0);

/** From knots, which is what NMEA reports: "5.2 km/h" or "3.2 mph". */
std::string formatSpeedFromKnots(float knots, unsigned decimals = 1);

/**
 * Rewrites a speed that arrives as text with its unit attached, which is what the weather API's
 * forecast sends ("5 to 10 mph"). Every number in @p text is converted and the unit word replaced;
 * text with no speed unit in it is returned unchanged, so a phrase this does not understand is left
 * alone rather than mangled.
 */
std::string convertSpeedText(const std::string& text);

// ---- Temperature ------------------------------------------------------------------------------

/** From °C: "21.5 °C" or "70.7 °F". */
std::string formatTemperatureFromCelsius(float celsius, unsigned decimals = 1);

/**
 * From a temperature whose unit came with the data (the weather API says which one it used), so it
 * can be converted when that is not the unit the user reads.
 * @param unit 'C' or 'F' (case-insensitive); anything else is treated as Celsius.
 */
std::string formatTemperature(float value, char unit, unsigned decimals = 1);

// ---- Pressure ---------------------------------------------------------------------------------

/** From pascals: "1013.2 hPa" or "29.92 inHg". */
std::string formatPressureFromPascal(float pascals, unsigned decimals = 0);

} // namespace tt::units
