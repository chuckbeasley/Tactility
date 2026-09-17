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
 * @param[in] url the URL to fetch
 * @param[out] outBody the response body on success, cleared on failure
 * @param[out] outError a short description of the failure, fit to show on screen
 * @param[in] timeoutMs how long the whole request may take
 * @return true on a 200 response
 */
bool httpGet(const std::string& url, std::string& outBody, std::string& outError, int32_t timeoutMs = 10000);

/**
 * Percent-encode everything outside the unreserved set, for use in a query string.
 *
 * Here rather than next to its caller because it is easy to get subtly wrong: '+' and '=' are
 * perfectly ordinary in base64 and mean something entirely different in a query string, where '+'
 * decodes to a space.
 */
std::string percentEncode(const std::string& input);

}
