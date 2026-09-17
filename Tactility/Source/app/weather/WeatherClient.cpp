// SPDX-License-Identifier: Apache-2.0
#include "WeatherClient.h"

#include <Tactility/network/HttpClient.h>

#include <cJSON.h>

#include <tactility/log.h>

#include <cstdlib>
#include <format>

#ifdef ESP_PLATFORM
// For the heap figures logged around the forecast parse, which is the one place in this app whose
// cost lands in internal RAM rather than PSRAM.
#include <esp_heap_caps.h>
#endif

namespace tt::app::weather {

namespace {

constexpr auto* TAG = "Weather";

// Shared with the radar screen, which fetches its imagery through the same path.
using tt::network::httpGet;

// region JSON helpers

/** NWS wraps measurements as {"unitCode": "...", "value": <number or null>}. */
float measurement(const cJSON* parent, const char* name) {
    const cJSON* item = cJSON_GetObjectItemCaseSensitive(parent, name);
    if (!cJSON_IsObject(item)) {
        return NAN;
    }
    const cJSON* value = cJSON_GetObjectItemCaseSensitive(item, "value");
    // cJSON_IsNumber is false for JSON null, which is exactly the "station did not report this"
    // case, so it needs no special handling.
    if (!cJSON_IsNumber(value)) {
        return NAN;
    }
    return static_cast<float>(value->valuedouble);
}

std::string stringField(const cJSON* parent, const char* name) {
    const cJSON* item = cJSON_GetObjectItemCaseSensitive(parent, name);
    if (!cJSON_IsString(item) || item->valuestring == nullptr) {
        return {};
    }
    return item->valuestring;
}

const cJSON* objectField(const cJSON* parent, const char* name) {
    const cJSON* item = cJSON_GetObjectItemCaseSensitive(parent, name);
    return cJSON_IsObject(item) ? item : nullptr;
}

/** Wraps cJSON_Parse with the two checks that make it safe to use the result. */
cJSON* parseJson(const std::string& body, std::string& outError) {
    cJSON* json = cJSON_Parse(body.c_str());
    if (json == nullptr) {
        outError = "Failed to parse the server's response";
    }
    return json;
}

// endregion

bool fetchObservation(const std::string& stationsUrl, CurrentConditions& outConditions, const AbortCheck& shouldAbort) {
    std::string error;
    std::string body;

    if (shouldAbort()) {
        return false;
    }

    // limit=1 matters more than it looks. The unpaged station list for a grid point is a GeoJSON
    // collection of every station in the area - 57 KB for New York - and it is parsed with cJSON,
    // which allocates a small block per token. Parsing that to read the first entry dropped the
    // internal heap to 12 bytes on real hardware; asking the server for the one station this code
    // actually uses does the same job for about a kilobyte.
    if (!httpGet(stationsUrl + "?limit=1", body, error)) {
        LOG_W(TAG, "Station list failed: %s", error.c_str());
        return false;
    }

    cJSON* stations = parseJson(body, error);
    if (stations == nullptr) {
        LOG_W(TAG, "Station list: %s", error.c_str());
        return false;
    }

    const cJSON* features = cJSON_GetObjectItemCaseSensitive(stations, "features");
    if (!cJSON_IsArray(features) || cJSON_GetArraySize(features) == 0) {
        LOG_W(TAG, "No observation stations for this point");
        cJSON_Delete(stations);
        return false;
    }

    // The list is ordered by distance, so the first entry is the nearest station. Its "id" is the
    // station URL itself, and the observation hangs off the same URL.
    const cJSON* nearest = cJSON_GetArrayItem(features, 0);
    const std::string stationUrl = stringField(nearest, "id");
    cJSON_Delete(stations);

    if (stationUrl.empty()) {
        LOG_W(TAG, "Nearest station has no URL");
        return false;
    }

    if (shouldAbort()) {
        return false;
    }
    if (!httpGet(stationUrl + "/observations/latest", body, error)) {
        LOG_W(TAG, "Observation failed: %s", error.c_str());
        return false;
    }

    cJSON* observation = parseJson(body, error);
    if (observation == nullptr) {
        LOG_W(TAG, "Observation: %s", error.c_str());
        return false;
    }

    const cJSON* observationProperties = objectField(observation, "properties");
    if (observationProperties == nullptr) {
        cJSON_Delete(observation);
        LOG_W(TAG, "Observation has no properties");
        return false;
    }

    outConditions.temperatureC = measurement(observationProperties, "temperature");
    outConditions.windSpeedKph = measurement(observationProperties, "windSpeed");
    outConditions.windDirectionDegrees = measurement(observationProperties, "windDirection");
    outConditions.relativeHumidityPercent = measurement(observationProperties, "relativeHumidity");
    outConditions.pressurePa = measurement(observationProperties, "barometricPressure");
    outConditions.description = stringField(observationProperties, "textDescription");
    outConditions.valid = true;

    cJSON_Delete(observation);
    return true;
}

bool fetchForecast(const std::string& forecastUrl, Forecast& outForecast, const AbortCheck& shouldAbort) {
    if (shouldAbort()) {
        return false;
    }

    std::string error;
    std::string body;
    if (!httpGet(forecastUrl, body, error)) {
        LOG_W(TAG, "Forecast failed: %s", error.c_str());
        return false;
    }

    cJSON* forecast = parseJson(body, error);
    if (forecast == nullptr) {
        LOG_W(TAG, "Forecast: %s", error.c_str());
        return false;
    }

    // cJSON allocates one small block per token, and a seven-day forecast is a lot of tokens. That
    // puts the parse's cost in internal RAM specifically (small allocations do not prefer PSRAM),
    // which is the scarcest resource on this board - so the two numbers either side of it are worth
    // keeping in the log.
    LOG_I(
        TAG,
        "  parsed %u bytes of forecast JSON (internal heap %u)",
        static_cast<unsigned>(body.size()),
        static_cast<unsigned>(heap_caps_get_free_size(MALLOC_CAP_INTERNAL))
    );

    const cJSON* properties = objectField(forecast, "properties");
    const cJSON* periods = properties != nullptr ? cJSON_GetObjectItemCaseSensitive(properties, "periods") : nullptr;
    if (!cJSON_IsArray(periods)) {
        cJSON_Delete(forecast);
        LOG_W(TAG, "Forecast has no periods");
        return false;
    }

    outForecast.periods.clear();
    const int count = cJSON_GetArraySize(periods);
    outForecast.periods.reserve(static_cast<size_t>(count));
    for (int i = 0; i < count; i++) {
        const cJSON* period = cJSON_GetArrayItem(periods, i);
        if (!cJSON_IsObject(period)) {
            continue;
        }

        ForecastPeriod entry;
        entry.name = stringField(period, "name");
        entry.shortForecast = stringField(period, "shortForecast");
        entry.detailedForecast = stringField(period, "detailedForecast");
        entry.temperatureUnit = stringField(period, "temperatureUnit");
        entry.windSpeed = stringField(period, "windSpeed");
        entry.windDirection = stringField(period, "windDirection");

        const cJSON* temperature = cJSON_GetObjectItemCaseSensitive(period, "temperature");
        if (cJSON_IsNumber(temperature)) {
            entry.temperature = static_cast<int>(temperature->valuedouble);
        }

        const cJSON* isDaytime = cJSON_GetObjectItemCaseSensitive(period, "isDaytime");
        entry.isDaytime = cJSON_IsTrue(isDaytime);

        outForecast.periods.push_back(std::move(entry));
    }

    cJSON_Delete(forecast);
    outForecast.valid = !outForecast.periods.empty();
    return outForecast.valid;
}

}

bool geocodePostalCode(const std::string& postalCode, Coordinates& outCoordinates, std::string& outError) {
    if (postalCode.empty()) {
        outError = "No ZIP code set";
        return false;
    }

    const std::string url = std::format(
        "https://nominatim.openstreetmap.org/search?postalcode={}&countrycodes=us&format=json&limit=1",
        postalCode
    );

    std::string body;
    if (!httpGet(url, body, outError)) {
        return false;
    }

    cJSON* json = parseJson(body, outError);
    if (json == nullptr) {
        return false;
    }

    bool ok = false;
    if (cJSON_IsArray(json) && cJSON_GetArraySize(json) > 0) {
        const cJSON* entry = cJSON_GetArrayItem(json, 0);
        // Nominatim returns coordinates as strings, not numbers, which is why they are parsed here
        // rather than read with cJSON's numeric accessors.
        const std::string latitude = stringField(entry, "lat");
        const std::string longitude = stringField(entry, "lon");
        if (!latitude.empty() && !longitude.empty()) {
            outCoordinates.latitude = strtod(latitude.c_str(), nullptr);
            outCoordinates.longitude = strtod(longitude.c_str(), nullptr);
            ok = outCoordinates.isValid();
        }
    }

    cJSON_Delete(json);

    if (!ok) {
        outError = "ZIP code not found";
    }
    return ok;
}

bool fetchReport(const Coordinates& coordinates, WeatherReport& outReport, const AbortCheck& shouldAbort, std::string& outError) {
    // NWS rejects a request for a point outside its coverage with a 404 whose body names the
    // problem, and it is the first thing that can go wrong, so it is worth its own message.
    const std::string pointsUrl = std::format(
        "https://api.weather.gov/points/{:.4f},{:.4f}",
        coordinates.latitude,
        coordinates.longitude
    );

    std::string body;
    if (!httpGet(pointsUrl, body, outError)) {
        LOG_W(TAG, "Points lookup failed: %s", outError.c_str());
        return false;
    }

    cJSON* points = parseJson(body, outError);
    if (points == nullptr) {
        return false;
    }

    const cJSON* properties = objectField(points, "properties");
    if (properties == nullptr) {
        cJSON_Delete(points);
        outError = "Unexpected response from the weather service";
        return false;
    }

    const std::string forecastUrl = stringField(properties, "forecast");
    const std::string stationsUrl = stringField(properties, "observationStations");
    outReport.radarStation = stringField(properties, "radarStation");

    // "City, ST" for the screen. Both parts are optional in the response, so the separator is only
    // added when there is something on both sides of it.
    const cJSON* relativeLocation = objectField(properties, "relativeLocation");
    const cJSON* locationProperties = relativeLocation != nullptr ? objectField(relativeLocation, "properties") : nullptr;
    if (locationProperties != nullptr) {
        const std::string city = stringField(locationProperties, "city");
        const std::string state = stringField(locationProperties, "state");
        if (!city.empty() && !state.empty()) {
            outReport.locationName = std::format("{}, {}", city, state);
        } else {
            outReport.locationName = city.empty() ? state : city;
        }
    }

    cJSON_Delete(points);

    if (forecastUrl.empty()) {
        outError = "The weather service returned no forecast for this location";
        return false;
    }

    // The observation is optional on purpose: it comes from a different endpoint and a station
    // that is offline must not cost the user the forecast.
    if (!stationsUrl.empty()) {
        if (!fetchObservation(stationsUrl, outReport.current, shouldAbort)) {
            LOG_W(TAG, "Continuing without current conditions");
        }
    }

    if (!fetchForecast(forecastUrl, outReport.forecast, shouldAbort)) {
        outError = "Could not retrieve the forecast";
        return false;
    }

    outReport.coordinates = coordinates;
    outReport.valid = true;
    outError.clear();
    return true;
}

}
