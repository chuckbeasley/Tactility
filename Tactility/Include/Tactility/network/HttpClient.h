// SPDX-License-Identifier: Apache-2.0
#pragma once

#include <cstdint>
#include <string>

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
 * Percent-encode everything outside the unreserved set, for use in a query string.
 *
 * Here rather than next to its caller because it is easy to get subtly wrong: '+' and '=' are
 * perfectly ordinary in base64 and mean something entirely different in a query string, where '+'
 * decodes to a space.
 */
std::string percentEncode(const std::string& input);

}
