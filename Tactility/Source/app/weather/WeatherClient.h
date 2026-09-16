// SPDX-License-Identifier: Apache-2.0
//
// The network half of the weather app: geocoding through Nominatim, and observations plus forecasts
// from the National Weather Service.
#pragma once

#include <cmath>
#include <functional>
#include <string>
#include <vector>

namespace tt::app::weather {

/** A point on the globe in decimal degrees, negative for south and west. */
struct Coordinates {
    double latitude = NAN;
    double longitude = NAN;

    bool isValid() const {
        return !std::isnan(latitude) && !std::isnan(longitude);
    }
};

/**
 * The latest observation from the station NWS considers nearest to the requested point.
 *
 * Every measurement is NAN when the station does not report it. The API sends JSON null for those
 * fields, and the station really does omit them - showing a zero there would put a plausible
 * looking wrong number on the screen (0% humidity, 0 wind), which is worse than showing nothing.
 */
struct CurrentConditions {
    bool valid = false;
    float temperatureC = NAN;
    float windSpeedKph = NAN;
    float windDirectionDegrees = NAN;
    float relativeHumidityPercent = NAN;
    float pressurePa = NAN;
    std::string description;
};

/**
 * One forecast period. NWS alternates day and night periods, so the fourteen it returns describe
 * seven days; the first couple are the short-term outlook and the rest are the long-range one.
 */
struct ForecastPeriod {
    std::string name;
    int temperature = 0;
    std::string temperatureUnit;
    std::string shortForecast;
    std::string detailedForecast;
    std::string windSpeed;
    std::string windDirection;
    bool isDaytime = false;
};

struct Forecast {
    bool valid = false;
    std::vector<ForecastPeriod> periods;
};

/**
 * Everything the screen needs for one location. `valid` means the report can be shown; otherwise
 * `error` explains why not in a form that can go straight on the screen.
 */
struct WeatherReport {
    bool valid = false;
    std::string error;
    /** "City, ST" from the NWS point metadata, or empty if the API did not supply it. */
    std::string locationName;
    Coordinates coordinates;
    CurrentConditions current;
    Forecast forecast;
};

/**
 * Called between network steps; returning true abandons the fetch. A fetch is several sequential
 * HTTPS requests, and without this the app could not be closed until the slowest one timed out.
 */
using AbortCheck = std::function<bool()>;

/**
 * Resolve a US postal code to coordinates using Nominatim.
 * @return true on success; otherwise @a outError explains the failure
 */
bool geocodePostalCode(const std::string& postalCode, Coordinates& outCoordinates, std::string& outError);

/**
 * Fetch the location name, the current observation and the forecast for a point.
 *
 * The NWS API is followed the way it is meant to be: /points gives the grid forecast and station
 * list for a coordinate, the station list gives the nearest station, and that station gives the
 * latest observation. Any of those steps can fail independently, so a missing observation still
 * leaves a usable forecast and vice versa.
 *
 * @return true when at least the forecast was retrieved; otherwise @a outError explains the failure
 */
bool fetchReport(const Coordinates& coordinates, WeatherReport& outReport, const AbortCheck& shouldAbort, std::string& outError);

}
