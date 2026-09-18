// SPDX-License-Identifier: Apache-2.0
#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>

/** The ESP-IDF HTTP client handle's pointee, named without pulling the ESP headers into this one. */
struct esp_http_client;

namespace tt::network {

/**
 * A response body: one buffer, in PSRAM, sized from what the server says it is sending.
 *
 * The body used to be a std::string filled by appending each piece of the response as it arrived.
 * That makes the string double and copy itself as it grows, and the sizes it passes through are
 * exactly the ones ESP-IDF's allocator prefers to take from internal RAM: the radar screen's 87 KB
 * base map went through about seven reallocations and 250 KB of copying on the way in. Internal RAM
 * on this board runs at a few tens of kilobytes free and is where the Wi-Fi and TLS stacks allocate,
 * so a response body has no business being there.
 *
 * So the length is taken from the response's Content-Length when the server sends one - both
 * services the radar screen uses do - and the body is read straight into that one buffer, in PSRAM.
 * A chunked response declares no length, and grows its buffer in PSRAM instead.
 *
 * The buffer keeps a terminator, which is what makes c_str() usable for JSON parsing and is the
 * reason it is one byte larger than the capacity it reports.
 */
class HttpBody {
public:
    HttpBody() = default;
    ~HttpBody();

    HttpBody(HttpBody&& other) noexcept;
    HttpBody& operator=(HttpBody&& other) noexcept;
    HttpBody(const HttpBody&) = delete;
    HttpBody& operator=(const HttpBody&) = delete;

    const uint8_t* data() const { return buffer; }
    size_t size() const { return length; }
    bool empty() const { return length == 0; }
    /** How much room this body has: one allocation serves a whole response. */
    size_t capacity() const { return room; }
    /** How many bytes the response declared, or 0 when it declared none. */
    size_t declaredSize() const { return declared; }
    /** The body as text, always terminated, so a parser can take it as a C string. */
    const char* c_str() const { return buffer != nullptr ? reinterpret_cast<const char*>(buffer) : ""; }
    std::string_view view() const { return std::string_view(c_str(), length); }

    /** Empties the body but keeps the allocation, so a reused body does not allocate again. */
    void clear();

    // The four below are the client filling the body, not the caller reading it.
    /** @internal Records the size the response declared, before any of it is read. */
    void setDeclaredSize(size_t bytes) { declared = bytes; }
    /** @internal Makes room for @a bytes, in PSRAM where there is room for it. */
    bool reserve(size_t bytes);
    /** @internal Declares @a bytes more as read. */
    void growBy(size_t bytes);
    /** @internal The buffer itself, so the client has somewhere to read into. */
    uint8_t* writable() { return buffer; }

private:
    /** Frees whatever this body holds, without touching its length. */
    void release();

    uint8_t* buffer = nullptr;
    size_t length = 0;
    size_t room = 0;
    size_t declared = 0;
};

/**
 * GET a URL into memory over HTTPS.
 *
 * Identifies itself in the User-Agent header, which the services this tree talks to require, and
 * verifies the server against the compiled-in certificate bundle rather than a pinned certificate,
 * because these are unrelated hosts whose chains have nothing in common.
 *
 * The response is read until the client reports the body finished, so a chunked response works the
 * same as one with a Content-Length. The body is bytes rather than text because it can be a binary
 * payload such as an image, and it is an HttpBody rather than a std::string because of where it is
 * allocated - see HttpBody.
 *
 * The size cap is the caller's, because the sizes are the caller's business: the radar loop is
 * around a megabyte when there is weather to see, while a forecast is tens of kilobytes and a
 * megabyte of JSON would be a problem rather than a product. It exists to stop a misbehaving server
 * filling memory, not to limit a legitimate response.
 *
 * @param[in] url the URL to fetch
 * @param[out] outBody the response body on success, emptied on failure
 * @param[out] outError a short description of the failure, fit to show on screen
 * @param[in] timeoutMs how long a single read may take - not a cap on the whole transfer, so a slow
 *                      but steady download is allowed to finish
 * @param[in] maxResponseBytes largest body to accept before giving up
 * @return true on a 200 response
 */
bool httpGet(
    const std::string& url,
    HttpBody& outBody,
    std::string& outError,
    int32_t timeoutMs = 10000,
    size_t maxResponseBytes = 512 * 1024
);

/**
 * A series of GETs over one connection.
 *
 * httpGet() opens its own connection, which means its own TLS handshake. That is the right cost for a
 * one-off request and the wrong one for a series: the radar screen fetches six frames from the same
 * host, and measured on this board each request cost about 3.8 seconds for sixteen kilobytes - almost
 * all of it handshake and server render rather than transfer. A session keeps the connection and
 * issues the requests over it, so a series pays for one handshake per session instead of one per
 * request.
 *
 * A session is for one thread: two of them are two connections. It is not thread safe, by design -
 * the point is to have several, not to share one.
 *
 * If a request fails the connection is dropped and the next one starts a new one, because a session
 * that has just seen an error cannot know what state the socket is in.
 */
class HttpSession {
public:
    HttpSession() = default;
    ~HttpSession();

    HttpSession(const HttpSession&) = delete;
    HttpSession& operator=(const HttpSession&) = delete;

    /** Same contract as httpGet(), over this session's connection. */
    bool get(
        const std::string& url,
        HttpBody& outBody,
        std::string& outError,
        int32_t timeoutMs = 10000,
        size_t maxResponseBytes = 512 * 1024
    );

    /** Drops the connection; the next get() starts a new one. */
    void close();

private:
    /** One attempt; get() wraps this so a dead kept-alive connection costs one retry, not a frame. */
    bool getOnce(
        const std::string& url,
        HttpBody& outBody,
        std::string& outError,
        int32_t timeoutMs,
        size_t maxResponseBytes
    );

    esp_http_client* client = nullptr;
};

/**
 * Percent-encode everything outside the unreserved set, for use in a query string.
 *
 * Here rather than next to its caller because it is easy to get subtly wrong: '+' and '=' are
 * perfectly ordinary in base64 and mean something entirely different in a query string, where '+'
 * decodes to a space.
 */
std::string percentEncode(const std::string& input);

}
