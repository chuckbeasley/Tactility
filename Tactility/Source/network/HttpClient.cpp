// SPDX-License-Identifier: Apache-2.0
#include <Tactility/network/HttpClient.h>

#include <tactility/log.h>

#include <cJSON.h>

#include <format>

#ifdef ESP_PLATFORM
#include <esp_crt_bundle.h>
#include <esp_heap_caps.h>
#include <esp_http_client.h>
#endif

namespace tt::network {

namespace {

constexpr auto* TAG = "HttpClient";

/**
 * Services that ask for an identifying User-Agent reject requests without one - api.weather.gov
 * does so outright, and Nominatim's usage policy requires it. Nominatim additionally asks that the
 * value identify the application rather than masquerade as a browser, and that clients stay at or
 * below one request per second, which the callers of this function do.
 */
constexpr auto* USER_AGENT = "Tactility/1.0 (+https://github.com/chuckbeasley/Tactility)";

/** A guard against a misbehaving server rather than a working limit: the largest legitimate
 *  response here is a seven-day forecast or a radar image, tens of kilobytes either way. */
constexpr size_t MAX_RESPONSE_BYTES = 512 * 1024;

/** Only enough of an error body to read the explanation out of it. */
constexpr size_t MAX_ERROR_BODY_BYTES = 2048;

/** APIs in this tree answer errors with a JSON body naming the problem; prefer that to a status
 *  code, because "Data unavailable" is something a user can act on and "HTTP 500" is not. */
std::string describeError(const std::string& body, int status) {
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

}

bool httpGet(const std::string& url, std::string& outBody, std::string& outError, int32_t timeoutMs) {
#ifdef ESP_PLATFORM
    LOG_I(TAG, "GET %s (internal heap %u)", url.c_str(), static_cast<unsigned>(heap_caps_get_free_size(MALLOC_CAP_INTERNAL)));

    esp_http_client_config_t config = {};
    config.url = url.c_str();
    config.crt_bundle_attach = esp_crt_bundle_attach;
    config.timeout_ms = timeoutMs;
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
        esp_http_client_set_header(client, "Accept", "application/geo+json, application/json, image/gif");

        if (esp_http_client_open(client, 0) != ESP_OK) {
            outError = "Failed to connect";
            break;
        }
        opened = true;

        // Reads the status line and headers. The return value is the content length, or -1 when the
        // server uses chunked encoding; neither is needed, because the body is read until the client
        // reports it finished.
        esp_http_client_fetch_headers(client);

        const int status = esp_http_client_get_status_code(client);
        if (status != 200) {
            std::string errorBody;
            char errorBuffer[256];
            while (errorBody.size() < MAX_ERROR_BODY_BYTES) {
                const int read = esp_http_client_read(client, errorBuffer, sizeof(errorBuffer));
                if (read <= 0) {
                    break;
                }
                errorBody.append(errorBuffer, static_cast<size_t>(read));
            }
            outError = describeError(errorBody, status);
            break;
        }

        outBody.clear();
        char buffer[1024];
        while (true) {
            const int read = esp_http_client_read(client, buffer, sizeof(buffer));
            if (read < 0) {
                outError = "Failed to read the response";
                outBody.clear();
                break;
            }
            if (read == 0) {
                ok = true;
                break;
            }
            if (outBody.size() + static_cast<size_t>(read) > MAX_RESPONSE_BYTES) {
                outError = "The response is too large";
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
    (void)timeoutMs;
    outBody.clear();
    outError = "Networking is unavailable on this platform";
    return false;
#endif
}

bool isUnreserved(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') ||
        c == '-' || c == '_' || c == '.' || c == '~';
}

std::string percentEncode(const std::string& input) {
    std::string output;
    output.reserve(input.size() * 2);
    for (const char raw : input) {
        const auto c = static_cast<unsigned char>(raw);
        if (isUnreserved(static_cast<char>(c))) {
            output += static_cast<char>(c);
        } else {
            output += std::format("%{:02X}", c);
        }
    }
    return output;
}

}
