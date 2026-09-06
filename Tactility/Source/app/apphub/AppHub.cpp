#include <Tactility/app/apphub/AppHub.h>

#include <cstdio>
#include <format>

namespace tt::app::apphub {

constexpr auto* BASE_URL = "https://cdn.tactilityproject.org/apps";

// Bound the number of fallback lookups so a build that can't find any published manifest
// doesn't hammer the CDN: current version first, then a handful of earlier candidates.
constexpr size_t MAX_FALLBACK_ATTEMPTS = 4;

static std::string getVersionWithoutPostfix() {
    std::string version(TT_VERSION);
    auto index = version.find_first_of('-');
    if (index == std::string::npos) {
        return version;
    } else {
        return version.substr(0, index);
    }
}

static std::string appsJsonUrlForVersion(const std::string& version) {
    return std::format("{}/{}/apps.json", BASE_URL, version);
}

// Builds the ordered list of manifest versions to try, newest first. For the current
// version "M.m.p" this is: M.m.p, M.m.(p-1) ... M.m.0, then the final (.0) release of each
// earlier minor. A dev/unreleased build (e.g. "0.8.1") therefore falls back to the newest
// released manifest (e.g. "0.8.0").
static std::vector<std::string> getVersionCandidates(const std::string& version) {
    std::vector<std::string> candidates;

    int major = 0;
    int minor = 0;
    int patch = 0;
    const int parsed = std::sscanf(version.c_str(), "%d.%d.%d", &major, &minor, &patch);
    if (parsed < 2) {
        // Unexpected format; just try the exact version.
        candidates.push_back(version);
        return candidates;
    }

    // Same minor, patch descending to 0 (covers the common "one patch ahead" dev case).
    for (int p = patch; p >= 0; --p) {
        candidates.push_back(std::format("{}.{}.{}", major, minor, p));
    }

    // Earlier minors, at their .0 release (published manifests are typically at .0).
    for (int m = minor - 1; m >= 0; --m) {
        candidates.push_back(std::format("{}.{}.0", major, m));
    }

    if (candidates.size() > MAX_FALLBACK_ATTEMPTS) {
        candidates.resize(MAX_FALLBACK_ATTEMPTS);
    }
    return candidates;
}

std::string getAppsJsonUrl() {
    return appsJsonUrlForVersion(getVersionWithoutPostfix());
}

std::vector<std::string> getAppsJsonUrlCandidates() {
    std::vector<std::string> urls;
    for (const auto& version : getVersionCandidates(getVersionWithoutPostfix())) {
        urls.push_back(appsJsonUrlForVersion(version));
    }
    return urls;
}

std::string getDownloadUrl(const std::string& relativePath) {
    return std::format("{}/{}/{}", BASE_URL, getVersionWithoutPostfix(), relativePath);
}

}
