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

#ifdef ESP_PLATFORM

/**
 * Reads the body of an opened request, or the explanation out of a failed one.
 *
 * Shared by the one-off call and the session, because the reading is where the subtleties are: the
 * return value of the read is -1 for an error and 0 for end of body, and a status that is not 200
 * still has a body worth reading, since these APIs name the problem there.
 */
bool readBody(
    esp_http_client_handle_t client,
    std::string& outBody,
    std::string& outError,
    size_t maxResponseBytes
) {
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
        return false;
    }

    outBody.clear();
    char buffer[1024];
    while (true) {
        const int read = esp_http_client_read(client, buffer, sizeof(buffer));
        if (read < 0) {
            outError = "Failed to read the response";
            outBody.clear();
            return false;
        }
        if (read == 0) {
            return true;
        }
        if (outBody.size() + static_cast<size_t>(read) > maxResponseBytes) {
            outError = std::format("The response is larger than {} bytes", static_cast<unsigned>(maxResponseBytes));
            outBody.clear();
            return false;
        }
        outBody.append(buffer, static_cast<size_t>(read));
    }
}

void logFetched(const std::string& url, const std::string& body) {
    LOG_I(
        TAG,
        "  %s: %u bytes (internal heap %u)",
        url.c_str(),
        static_cast<unsigned>(body.size()),
        static_cast<unsigned>(heap_caps_get_free_size(MALLOC_CAP_INTERNAL))
    );
}

#endif

HttpSession::~HttpSession() {
    close();
}

void HttpSession::close() {
#ifdef ESP_PLATFORM
    if (client != nullptr) {
        esp_http_client_close(client);
        esp_http_client_cleanup(client);
        client = nullptr;
    }
#endif
}

bool HttpSession::get(
    const std::string& url,
    std::string& outBody,
    std::string& outError,
    int32_t timeoutMs,
    size_t maxResponseBytes
) {
#ifdef ESP_PLATFORM
    // Twice, because the first attempt can fail for a reason that says nothing about the request: a
    // kept-alive connection that the server has since closed looks exactly like a broken one, and a
    // request written onto that dead socket simply waits. The failed attempt closes the connection,
    // so the retry is a new one. Measured before this: one request in six stalled for 32 seconds and
    // then failed, while its neighbours on the same session answered in under a second.
    for (int32_t attempt = 0; attempt < 2; attempt++) {
        if (attempt > 0) {
            LOG_I(TAG, "Retrying %s on a new connection", url.c_str());
        }
        if (getOnce(url, outBody, outError, timeoutMs, maxResponseBytes)) {
            return true;
        }
    }
    return false;
#else
    (void)url;
    (void)outBody;
    (void)timeoutMs;
    (void)maxResponseBytes;
    outError = "Networking is unavailable on this platform";
    return false;
#endif
}

bool HttpSession::getOnce(
    const std::string& url,
    std::string& outBody,
    std::string& outError,
    int32_t timeoutMs,
    size_t maxResponseBytes
) {
#ifdef ESP_PLATFORM
    if (client == nullptr) {
        LOG_I(TAG, "GET %s (new connection)", url.c_str());

        esp_http_client_config_t config = {};
        config.url = url.c_str();
        config.crt_bundle_attach = esp_crt_bundle_attach;
        config.timeout_ms = timeoutMs;
        config.method = HTTP_METHOD_GET;
        // What makes a session worth having: without this the connection is torn down when the
        // request completes and the next one pays for the handshake again.
        config.keep_alive_enable = true;

        client = esp_http_client_init(&config);
        if (client == nullptr) {
            outError = "Failed to create HTTP client";
            return false;
        }
        esp_http_client_set_header(client, "User-Agent", USER_AGENT);
        esp_http_client_set_header(client, "Accept", "application/json, image/png");
    } else {
        LOG_I(TAG, "GET %s (same connection)", url.c_str());
        if (esp_http_client_set_url(client, url.c_str()) != ESP_OK) {
            outError = "Failed to set the URL";
            close();
            return false;
        }
    }

    if (esp_http_client_open(client, 0) != ESP_OK) {
        outError = "Failed to connect";
        close();
        return false;
    }

    esp_http_client_fetch_headers(client);

    if (!readBody(client, outBody, outError, maxResponseBytes)) {
        // The socket may be half-read, so it is not reused: the next request starts a new one.
        close();
        return false;
    }

    logFetched(url, outBody);
    return true;
#else
    (void)url;
    (void)outBody;
    (void)timeoutMs;
    (void)maxResponseBytes;
    outError = "Networking is unavailable on this platform";
    return false;
#endif
}

bool httpGet(
    const std::string& url,
    std::string& outBody,
    std::string& outError,
    int32_t timeoutMs,
    size_t maxResponseBytes
) {
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

    bool ok = false;

    do {
        esp_http_client_set_header(client, "User-Agent", USER_AGENT);
        esp_http_client_set_header(client, "Accept", "application/geo+json, application/json, image/gif");

        if (esp_http_client_open(client, 0) != ESP_OK) {
            outError = "Failed to connect";
            break;
        }

        // Reads the status line and headers. The return value is the content length, or -1 when the
        // server uses chunked encoding; neither is needed, because the body is read until the client
        // reports it finished.
        esp_http_client_fetch_headers(client);

        ok = readBody(client, outBody, outError, maxResponseBytes);
    } while (false);

    esp_http_client_close(client);
    esp_http_client_cleanup(client);

    if (!ok && outError.empty()) {
        outError = "Empty response";
    }

    if (ok) {
        logFetched(url, outBody);
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
