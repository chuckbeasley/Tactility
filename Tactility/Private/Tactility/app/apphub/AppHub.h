#pragma once

#include <string>
#include <vector>

namespace tt::app::apphub {

constexpr auto* CERTIFICATE_PATH = "/system/certificates/WE1.pem";

std::string getAppsJsonUrl();

/**
 * Returns the app-manifest URLs to try, in priority order. The first is for the current
 * OS version; the rest are fallbacks for progressively earlier versions. This lets a
 * dev/unreleased build (e.g. "0.8.1") fall back to the newest released manifest (e.g.
 * "0.8.0") when the exact version isn't published on the CDN yet.
 */
std::vector<std::string> getAppsJsonUrlCandidates();

std::string getDownloadUrl(const std::string& relativePath);

}