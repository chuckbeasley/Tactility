// SPDX-License-Identifier: Apache-2.0
#pragma once

#include <cstdint>
#include <string>

/** The ESP-IDF HTTP client handle's pointee, named without pulling the ESP headers into this one. */
struct esp_http_client;

namespace tt::network {

/**
 * GET a URL into memory over HTTPS.
 *
 * Identifies itself in the User-Agent header, which the services this tree talks to require, and
 * verifies the server against the compiled-in certificate bundle rather than a pinned certificate,
 * because these are unrelated hosts whose chains have nothing in common.
 *
 * The response is read until the client reports the body finished, so a chunked response works the
 * same as one with a Content-Length - which is also why the body is returned as a string rather
 * than a buffer: it can be a binary payload such as an image.
 *
 * The size cap is the caller's, because the sizes are the caller's business: the radar loop is
 * around a megabyte when there is weather to see, while a forecast is tens of kilobytes and a
 * megabyte of JSON would be a problem rather than a product. It exists to stop a misbehaving server
 * filling memory, not to limit a legitimate response.
 *
 * @param[in] url the URL to fetch
 * @param[out] outBody the response body on success, cleared on failure
 * @param[out] outError a short description of the failure, fit to show on screen
 * @param[in] timeoutMs how long a single read may take - not a cap on the whole transfer, so a slow
 *                      but steady download is allowed to finish
 * @param[in] maxResponseBytes largest body to accept before giving up
 * @return true on a 200 response
 */
bool httpGet(
    const std::string& url,
    std::string& outBody,
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
        std::string& outBody,
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
        std::string& outBody,
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
