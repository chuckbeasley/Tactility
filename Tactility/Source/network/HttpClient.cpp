// SPDX-License-Identifier: Apache-2.0
#include <Tactility/network/HttpClient.h>

#include <tactility/log.h>

#include <cJSON.h>

#include <algorithm>
#include <cstdlib>
#include <cstring>
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

/**
 * How much of the body to ask the client for at a time.
 *
 * Larger than the client's own receive buffer on purpose: esp_http_client_read fills the caller's
 * buffer completely before returning, working through its receive buffer in however many trips it
 * takes, so asking for more per call is what keeps a body from being picked up a few hundred bytes
 * at a time. Small enough to stay off the stack - this is handed to a read that runs on whatever
 * task made the request, and an app task here has 8 KB.
 */
constexpr size_t READ_CHUNK_BYTES = 4096;

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

/**
 * PSRAM first: a response body is the largest thing a request allocates, and internal RAM on this
 * board is a few tens of kilobytes that the Wi-Fi and TLS stacks have to live in too.
 *
 * The fallback to internal memory is deliberate rather than tidy: PSRAM is shared with the video
 * recorder, the Wi-Fi capture buffers and the radar's own frames, so a body that cannot have the
 * pool should still be able to complete, just more expensively.
 */
void* allocateBody(size_t bytes) {
#ifdef ESP_PLATFORM
    void* memory = heap_caps_malloc(bytes, MALLOC_CAP_SPIRAM);
    return memory != nullptr ? memory : heap_caps_malloc(bytes, MALLOC_CAP_8BIT);
#else
    return std::malloc(bytes);
#endif
}

void releaseBody(void* memory) {
    if (memory == nullptr) {
        return;
    }
#ifdef ESP_PLATFORM
    heap_caps_free(memory);
#else
    std::free(memory);
#endif
}

}

HttpBody::~HttpBody() {
    release();
}

HttpBody::HttpBody(HttpBody&& other) noexcept
    : buffer(other.buffer), length(other.length), room(other.room), declared(other.declared) {
    other.buffer = nullptr;
    other.length = 0;
    other.room = 0;
    other.declared = 0;
}

HttpBody& HttpBody::operator=(HttpBody&& other) noexcept {
    if (this != &other) {
        release();
        buffer = other.buffer;
        length = other.length;
        room = other.room;
        declared = other.declared;
        other.buffer = nullptr;
        other.length = 0;
        other.room = 0;
        other.declared = 0;
    }
    return *this;
}

void HttpBody::release() {
    releaseBody(buffer);
    buffer = nullptr;
    length = 0;
    room = 0;
    declared = 0;
}

void HttpBody::clear() {
    length = 0;
    declared = 0;
    if (buffer != nullptr) {
        buffer[0] = '\0';
    }
}

bool HttpBody::reserve(size_t bytes) {
    if (bytes <= room) {
        return true;
    }

    // The allocation is the room asked for plus the terminator c_str() promises.
    void* memory = allocateBody(bytes + 1);
    if (memory == nullptr) {
        return false;
    }
    if (length > 0) {
        std::memcpy(memory, buffer, length);
    }
    releaseBody(buffer);
    buffer = static_cast<uint8_t*>(memory);
    room = bytes;
    buffer[length] = '\0';
    return true;
}

void HttpBody::growBy(size_t bytes) {
    length += bytes;
    buffer[length] = '\0';
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
    HttpBody& outBody,
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

    // The length the response declares. With it the body is a single allocation of exactly the right
    // size, before any of it is read - so there is no growth to copy through and nothing of the body
    // ever lands in the internal heap. A chunked response declares none, and grows instead.
    const int64_t declared = esp_http_client_get_content_length(client);
    if (declared > 0) {
        if (static_cast<uint64_t>(declared) > maxResponseBytes) {
            outError = std::format("The response is larger than {} bytes", static_cast<unsigned>(maxResponseBytes));
            return false;
        }
        outBody.setDeclaredSize(static_cast<size_t>(declared));
        if (!outBody.reserve(static_cast<size_t>(declared))) {
            outError = std::format("Not enough memory for {} bytes", static_cast<unsigned>(declared));
            return false;
        }
    }

    while (true) {
        // A response that declared its length has said how much there is. Once that much has arrived
        // the body is whole, and going back for more only makes the buffer grow past what it needs -
        // measured before this check: an 87 KB base map came out with 175 KB reserved for it.
        if (outBody.declaredSize() > 0 && outBody.size() >= outBody.declaredSize()) {
            return true;
        }

        if (outBody.size() == outBody.capacity()) {
            size_t wanted = std::max(outBody.capacity() * 2, READ_CHUNK_BYTES);
            if (wanted > maxResponseBytes) {
                wanted = maxResponseBytes;
            }
            // The cap is what this is: a response that has filled the caller's allowance and still
            // has more to give, rather than one that cannot have the memory it asked for.
            if (wanted <= outBody.capacity()) {
                outError = std::format("The response is larger than {} bytes", static_cast<unsigned>(maxResponseBytes));
                outBody.clear();
                return false;
            }
            if (!outBody.reserve(wanted)) {
                outError = "Not enough memory for the response";
                outBody.clear();
                return false;
            }
        }

        const size_t room = outBody.capacity() - outBody.size();
        const int read = esp_http_client_read(
            client,
            reinterpret_cast<char*>(outBody.writable()) + outBody.size(),
            static_cast<int>(std::min(room, READ_CHUNK_BYTES))
        );
        if (read < 0) {
            outError = "Failed to read the response";
            outBody.clear();
            return false;
        }
        if (read == 0) {
            return true;
        }
        outBody.growBy(static_cast<size_t>(read));
    }
}

void logFetched(const std::string& url, const HttpBody& body) {    LOG_I(
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
    HttpBody& outBody,
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
    HttpBody& outBody,
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
        // The receive buffer reads from the transport into, and the only square feet this has: it is
        // a plain malloc, so it comes out of internal RAM, and it bounds how much is moved per trip
        // through the parser. Four times the 512-byte default trades 1.5 KB of internal heap per
        // session for four times fewer trips; the transmit side stays at the default, since requests
        // here are a few hundred bytes of headers.
        config.buffer_size = 2048;

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
    HttpBody& outBody,
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
    // Matches the session's receive buffer, for the same reason and with the same cost: this one is
    // a plain malloc out of internal RAM, so its size is a trade against trips through the parser.
    config.buffer_size = 2048;

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
