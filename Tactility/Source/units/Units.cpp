// SPDX-License-Identifier: Apache-2.0
#include <Tactility/units/Units.h>

#include <Tactility/settings/SystemSettings.h>

#include <cctype>
#include <cmath>
#include <cstdlib>
#include <format>

namespace tt::units {

namespace {

constexpr float METRES_TO_FEET = 3.280839895f;
constexpr float KPH_TO_MPH = 0.621371192f;
constexpr float KNOTS_TO_KPH = 1.852f;      // exact by definition of the nautical mile
constexpr float PASCALS_TO_INHG = 0.00029529988f;

} // namespace

UnitSystem getSystem() {
    settings::SystemSettings settings;
    // loadSystemSettings() leaves the struct at its defaults when there is no readable file, and the
    // default is metric - what every internal value already is (metres, km/h, °C, pascals).
    if (!settings::loadSystemSettings(settings)) {
        return UnitSystem::Metric;
    }
    return settings.unitSystem;
}

void setSystem(UnitSystem system) {
    settings::SystemSettings settings;
    settings::loadSystemSettings(settings);
    settings.unitSystem = system;
    settings::saveSystemSettings(settings);
}

bool isImperial() {
    return getSystem() == UnitSystem::Imperial;
}

const char* toString(UnitSystem system) {
    switch (system) {
        case UnitSystem::Imperial: return "imperial";
        case UnitSystem::Metric:
        default:                   return "metric";
    }
}

std::string formatDistance(float metres, unsigned decimals) {
    if (std::isnan(metres)) {
        return "--";
    }
    if (!isImperial()) {
        return std::format("{:.{}f} m", (double)metres, decimals);
    }
    return std::format("{:.{}f} ft", (double)(metres * METRES_TO_FEET), decimals);
}

std::string formatDistanceRange(float lowMetres, float highMetres, unsigned decimals) {
    if (std::isnan(lowMetres) || std::isnan(highMetres)) {
        return "--";
    }
    if (!isImperial()) {
        return std::format("{:.{}f}-{:.{}f} m", (double)lowMetres, decimals, (double)highMetres, decimals);
    }
    return std::format("{:.{}f}-{:.{}f} ft", (double)(lowMetres * METRES_TO_FEET), decimals,
        (double)(highMetres * METRES_TO_FEET), decimals);
}

std::string formatSpeedFromKph(float kph, unsigned decimals) {
    if (std::isnan(kph)) {
        return "--";
    }
    if (!isImperial()) {
        return std::format("{:.{}f} km/h", (double)kph, decimals);
    }
    return std::format("{:.{}f} mph", (double)(kph * KPH_TO_MPH), decimals);
}

std::string formatSpeedFromKnots(float knots, unsigned decimals) {
    if (std::isnan(knots)) {
        return "--";
    }
    return formatSpeedFromKph(knots * KNOTS_TO_KPH, decimals);
}

std::string convertSpeedText(const std::string& text) {
    // The weather API sends phrases like "5 to 10 mph" or "10 mph", and it says which unit it used.
    // Only the two units this can recognise are handled; anything else is returned untouched, because
    // rewriting a phrase that was not understood is worse than showing it in the API's own units.
    const bool from_mph = text.find("mph") != std::string::npos;
    const bool from_kph = text.find("km/h") != std::string::npos;
    if (from_mph == from_kph) {
        return text;
    }

    const bool want_mph = isImperial();
    if (from_mph == want_mph) {
        return text;
    }

    // Every number in the string is a speed in the same unit ("5 to 10 mph"), so all of them are
    // converted and the unit word is swapped for the one the user reads.
    const float factor = want_mph ? KPH_TO_MPH : (1.0f / KPH_TO_MPH);
    std::string result;
    result.reserve(text.size() + 8);

    size_t i = 0;
    while (i < text.size()) {
        if (std::isdigit(static_cast<unsigned char>(text[i])) != 0) {
            size_t end = i;
            while (end < text.size() &&
                   (std::isdigit(static_cast<unsigned char>(text[end])) != 0 || text[end] == '.')) {
                ++end;
            }
            const float value = std::strtof(text.substr(i, end - i).c_str(), nullptr);
            result += std::format("{:.0f}", (double)(value * factor));
            i = end;
            continue;
        }
        result += text[i];
        ++i;
    }

    // Swap the unit word rather than appending one, so "5 to 10 mph" becomes "8 to 16 km/h".
    const std::string from = from_mph ? "mph" : "km/h";
    const std::string to = want_mph ? "mph" : "km/h";
    const size_t unit_pos = result.rfind(from);
    if (unit_pos != std::string::npos) {
        result.replace(unit_pos, from.size(), to);
    }
    return result;
}

std::string formatTemperatureFromCelsius(float celsius, unsigned decimals) {
    if (std::isnan(celsius)) {
        return "--";
    }
    if (!isImperial()) {
        return std::format("{:.{}f} \u00b0C", (double)celsius, decimals);
    }
    return std::format("{:.{}f} \u00b0F", (double)((celsius * 9.0f / 5.0f) + 32.0f), decimals);
}

std::string formatTemperature(float value, char unit, unsigned decimals) {
    if (std::isnan(value)) {
        return "--";
    }
    const bool source_is_fahrenheit = (unit == 'F' || unit == 'f');
    const float celsius = source_is_fahrenheit ? ((value - 32.0f) * 5.0f / 9.0f) : value;
    return formatTemperatureFromCelsius(celsius, decimals);
}

std::string formatPressureFromPascal(float pascals, unsigned decimals) {
    if (std::isnan(pascals)) {
        return "--";
    }
    if (!isImperial()) {
        // Hectopascals, which are what a weather report quotes (1 hPa = 100 Pa).
        return std::format("{:.{}f} hPa", (double)(pascals / 100.0f), decimals == 0 ? 1 : decimals);
    }
    return std::format("{:.2f} inHg", (double)(pascals * PASCALS_TO_INHG));
}

} // namespace tt::units
