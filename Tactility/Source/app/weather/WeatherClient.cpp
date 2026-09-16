// SPDX-License-Identifier: Apache-2.0
#include "WeatherClient.h"

#include <cJSON.h>

#include <tactility/log.h>

#include <cstdlib>
#include <format>

#ifdef ESP_PLATFORM
#include <esp_crt_bundle.h>
#include <esp_heap_caps.h>
#include <esp_http_client.h>
#endif

namespace tt::app::weather {

namespace {

constexpr auto* TAG = "Weather";

/**
 * Both services require a User-Agent that identifies the caller: api.weather.gov rejects requests
 * without one outright, and Nominatim's usage policy requires it. Nominatim additionally asks that
 * the value identify the application rather than masquerade as a browser, and that clients stay at
 * or below one request per second - this app makes one geocoding request per location change, which
 * is well inside that.
 */
constexpr auto* USER_AGENT = "TactilityWeather/1.0 (Tactility OS; +https://github.com/chuckbeasley/Tactility)";

constexpr int HTTP_TIMEOUT_MS = 10000;

/** The largest response any of these endpoints returns is the seven-day forecast, a few tens of KB.
 *  This is a guard against a misbehaving server rather than a real limit. */
constexpr size_t MAX_RESPONSE_BYTES = 256 * 1024;

/** Only enough of an error body to read the explanation out of it. */
constexpr size_t MAX_ERROR_BODY_BYTES = 2048;

/** Defined below with the other JSON helpers; declared here because httpGet() reports errors with
 *  whatever the server said rather than just the status code. */
std::string describeApiError(const std::string& body, int status);

bool httpGet(const std::string& url, std::string& outBody, std::string& outError) {
#ifdef ESP_PLATFORM
    LOG_I(TAG, "GET %s (internal heap %u)", url.c_str(), static_cast<unsigned>(heap_caps_get_free_size(MALLOC_CAP_INTERNAL)));

    esp_http_client_config_t config = {};
    config.url = url.c_str();
    // The certificate bundle rather than one pinned certificate: this talks to two unrelated hosts
    // (api.weather.gov and nominatim.openstreetmap.org) whose chains have nothing in common. The
    // full Mozilla bundle is already compiled in - see CONFIG_MBEDTLS_CERTIFICATE_BUNDLE_DEFAULT_FULL.
    config.crt_bundle_attach = esp_crt_bundle_attach;
    config.timeout_ms = HTTP_TIMEOUT_MS;
    config.method = HTTP_METHOD_GET;

    esp_http_client_handle_t client = esp_http_client_init(&config);
    if (client == nullptr) {
        outError = "Failed to create HTTP client";
        return false;
    }

    bool opened = false;
    bool ok = false;

    do {
        esp_http_client_set_header(client, "User-Agent", USER_AGENT);
        // The geocoder answers JSON; the NWS endpoints answer either. Asking for both keeps one
        // code path for two services.
        esp_http_client_set_header(client, "Accept", "application/geo+json, application/json");

        if (esp_http_client_open(client, 0) != ESP_OK) {
            outError = "Failed to connect";
            break;
        }
        opened = true;

        // Reads the status line and headers. The return value is the content length, or -1 when the
        // server uses chunked encoding - neither is needed here, because the body is read until the
        // client reports it is finished.
        esp_http_client_fetch_headers(client);

        const int status = esp_http_client_get_status_code(client);
        if (status != 200) {
            // The NWS answers errors with a JSON body explaining the problem ("Data unavailable",
            // "Invalid request"), which is far more useful on screen than the status code alone.
            std::string errorBody;
            char errorBuffer[256];
            while (errorBody.size() < MAX_ERROR_BODY_BYTES) {
                const int read = esp_http_client_read(client, errorBuffer, sizeof(errorBuffer));
                if (read <= 0) {
                    break;
                }
                errorBody.append(errorBuffer, static_cast<size_t>(read));
            }
            outError = describeApiError(errorBody, status);
            break;
        }

        outBody.clear();
        char buffer[1024];
        while (true) {
            const int read = esp_http_client_read(client, buffer, sizeof(buffer));
            if (read < 0) {
                outError = "Failed to read response";
                outBody.clear();
                break;
            }
            if (read == 0) {
                ok = true;
                break;
            }
            if (outBody.size() + static_cast<size_t>(read) > MAX_RESPONSE_BYTES) {
                outError = "Response is too large";
                outBody.clear();
                break;
            }
            outBody.append(buffer, static_cast<size_t>(read));
        }
    } while (false);

    if (opened) {
        esp_http_client_close(client);
    }
    esp_http_client_cleanup(client);

    if (!ok && outError.empty()) {
        outError = "Empty response";
    }

    if (ok) {
        LOG_I(
            TAG,
            "  %u bytes (internal heap %u)",
            static_cast<unsigned>(outBody.size()),
            static_cast<unsigned>(heap_caps_get_free_size(MALLOC_CAP_INTERNAL))
        );
    }
    return ok;
#else
    (void)url;
    outBody.clear();
    outError = "Networking is unavailable on this platform";
    return false;
#endif
}

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

/** The NWS APIs answer errors with a JSON body describing the problem; prefer that to "HTTP 404". */
std::string describeApiError(const std::string& body, int status) {
    cJSON* json = cJSON_Parse(body.c_str());
    if (json != nullptr) {
        const cJSON* detail = cJSON_GetObjectItemCaseSensitive(json, "detail");
        if (cJSON_IsString(detail) && detail->valuestring != nullptr) {
            std::string text = detail->valuestring;
            cJSON_Delete(json);
            return text;
        }
        cJSON_Delete(json);
    }
    return std::format("Server returned {}", status);
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
