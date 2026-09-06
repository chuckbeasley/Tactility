#ifdef ESP_PLATFORM

#include <Tactility/service/webserver/WebServerService.h>
#include <Tactility/service/ServiceManifest.h>
#include <Tactility/settings/WebServerSettings.h>
#include <Tactility/MountPoints.h>
#include <Tactility/file/File.h>
#include <Tactility/lvgl/Statusbar.h>
#include <Tactility/Mutex.h>
#include <Tactility/video/VideoRecorder.h>

#include <Tactility/DeprecatedPaths.h>
#include <Tactility/StringUtils.h>
#include <Tactility/TactilityConfig.h>
#include <Tactility/lvgl/Lvgl.h>
#include <Tactility/network/HttpdReq.h>
#include <Tactility/network/Url.h>
#include <Tactility/service/wifi/Wifi.h>
#include <Tactility/service/ServiceRegistration.h>

#include <tactility/check.h>
#include <tactility/filesystem/file_system.h>
#include <tactility/log.h>

#include <lvgl/lvgl.h>
#include <lvgl/icons/statusbar.h>

// The screenshot API (lv_snapshot_take / lv_draw_buf_*) is pulled in by <lvgl/lvgl.h> when
// CONFIG_LV_USE_SNAPSHOT is enabled, which is what makes TT_FEATURE_SCREENSHOT_API true.

#include "app/install.h"
#include "app/manager.h"


#include <atomic>
#include <cctype>
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <esp_chip_info.h>
#include <esp_flash.h>
#include <esp_heap_caps.h>
#include <esp_netif.h>
#include <esp_system.h>
#include <esp_vfs_fat.h>
#include <esp_wifi.h>
#include <esp_wifi_default.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <iomanip>
#include <lwip/ip4_addr.h>
#include <mbedtls/base64.h>
#include <ranges>
#include <sstream>
#include <vector>

namespace tt::service::webserver {

constexpr auto* TAG = "WebServerService";
extern const ServiceManifest manifest;

// Helper to convert chip model enum to human-readable string
static const char* getChipModelName(esp_chip_model_t model) {
    switch (model) {
        case CHIP_ESP32:   return "ESP32";
        case CHIP_ESP32S2: return "ESP32-S2";
        case CHIP_ESP32S3: return "ESP32-S3";
        case CHIP_ESP32C3: return "ESP32-C3";
        case CHIP_ESP32C2: return "ESP32-C2";
        case CHIP_ESP32C6: return "ESP32-C6";
        case CHIP_ESP32H2: return "ESP32-H2";
        case CHIP_ESP32P4: return "ESP32-P4";
        case CHIP_ESP32C5: return "ESP32-C5";
        case CHIP_ESP32C61: return "ESP32-C61";
        default:           return "Unknown";
    }
}

// Cached settings to avoid SD card reads on every HTTP request
static Mutex g_settingsMutex;
static settings::webserver::WebServerSettings g_cachedSettings;
static bool g_settingsCached = false;

// Global instance pointer for controlling the service (atomic to prevent TOCTOU races)
static std::atomic<WebServerService*> g_webServerInstance{nullptr};

constexpr int MAX_UPLOAD_SIZE = 10 * 1024 * 1024; // 10 MB limit

static void publish_event(WebServerService* webserver, WebServerEvent event) {
    webserver->getPubsub()->publish(event);
}

static WebServerService* ensureService() {
    WebServerService* webserver = g_webServerInstance.load();
    if (webserver != nullptr) {
        return webserver;
    }
    addService(manifest);
    webserver = g_webServerInstance.load();
    if (webserver == nullptr) {
        LOG_E(TAG, "Failed to lazy-start service");
    }
    return webserver;
}

std::shared_ptr<PubSub<WebServerEvent>> getPubsub() {
    WebServerService* webserver = ensureService();
    if (webserver == nullptr) {
        check(false, "Service not available");
    }

    return webserver->getPubsub();
}

static bool secureCompare(const std::string& a, const std::string& b) {
    size_t maxLen = std::max(a.size(), b.size());
    volatile unsigned char result = 0;
    result |= (a.size() != b.size());
    for (size_t i = 0; i < maxLen; ++i) {
        unsigned char ca = (i < a.size()) ? static_cast<unsigned char>(a[i]) : 0;
        unsigned char cb = (i < b.size()) ? static_cast<unsigned char>(b[i]) : 0;
        result |= ca ^ cb;
    }
    return result == 0;
}

// Helper to send 401 Unauthorized response with WWW-Authenticate header
static esp_err_t sendUnauthorized(httpd_req_t* request, const char* message) {
    httpd_resp_set_hdr(request, "WWW-Authenticate", "Basic realm=\"Tactility\"");
    httpd_resp_send_err(request, HTTPD_401_UNAUTHORIZED, message);
    return ESP_OK;  // Response was sent successfully
}

// Helper to validate HTTP Basic Auth on sensitive endpoints
// Returns ESP_OK with authPassed=true if auth succeeded or is disabled
// Returns ESP_OK with authPassed=false if auth failed (401 response already sent)
static esp_err_t validateRequestAuth(httpd_req_t* request, bool& authPassed) {
    authPassed = false;

    // Copy settings under lock to avoid race with settings update callback
    settings::webserver::WebServerSettings settings;
    {
        auto lock = g_settingsMutex.asScopedLock();
        lock.lock();
        settings = g_cachedSettings;
    }

    if (!settings.webServerAuthEnabled) {
        authPassed = true;
        return ESP_OK;  // Auth disabled, allow request
    }

    // Get Authorization header
    size_t auth_len = httpd_req_get_hdr_value_len(request, "Authorization");
    if (auth_len == 0) {
        return sendUnauthorized(request, "Authorization required");
    }

    std::string auth_header(auth_len + 1, '\0');
    if (httpd_req_get_hdr_value_str(request, "Authorization", auth_header.data(), auth_len + 1) != ESP_OK) {
        LOG_W(TAG, "Failed to read Authorization header");
        return sendUnauthorized(request, "Authorization required");
    }
    auth_header.resize(auth_len);  // Remove null terminator from string length

    // Check for "Basic " prefix
    if (auth_header.rfind("Basic ", 0) != 0) {
        LOG_W(TAG, "Authorization header is not Basic auth");
        return sendUnauthorized(request, "Basic authorization required");
    }

    // Extract base64 encoded credentials
    std::string base64_creds = auth_header.substr(6);

    // Decode base64 using mbedtls (available in ESP-IDF)
    size_t decoded_len = 0;
    // First pass to get length
    mbedtls_base64_decode(nullptr, 0, &decoded_len,
                          reinterpret_cast<const unsigned char*>(base64_creds.c_str()),
                          base64_creds.length());

    std::string decoded(decoded_len, '\0');
    size_t actual_len = 0;
    int ret = mbedtls_base64_decode(reinterpret_cast<unsigned char*>(decoded.data()),
                                     decoded_len, &actual_len,
                                     reinterpret_cast<const unsigned char*>(base64_creds.c_str()),
                                     base64_creds.length());
    if (ret != 0) {
        LOG_W(TAG, "Failed to decode base64 credentials");
        return sendUnauthorized(request, "Invalid credentials format");
    }
    decoded.resize(actual_len);

    // Parse username:password
    size_t colon_pos = decoded.find(':');
    if (colon_pos == std::string::npos) {
        LOG_W(TAG, "Invalid credentials format (no colon separator)");
        return sendUnauthorized(request, "Invalid credentials format");
    }

    std::string username = decoded.substr(0, colon_pos);
    std::string password = decoded.substr(colon_pos + 1);

    // Validate against cached settings
    bool usernameMatch = secureCompare(username, settings.webServerUsername);
    bool passwordMatch = secureCompare(password, settings.webServerPassword);
    if (!usernameMatch || !passwordMatch) {
        LOG_W(TAG, "Invalid credentials for user '%s'", username.c_str());
        return sendUnauthorized(request, "Invalid credentials");
    }

    authPassed = true;
    return ESP_OK;  // Auth successful
}

bool WebServerService::onStart(ServiceContext& service) {
    LOG_I(TAG, "Starting WebServer service...");

    // Register global instance
    g_webServerInstance.store(this);

    // Create statusbar icon (hidden initially, shown when server actually starts)
    statusbarIconId = lvgl::statusbar_icon_add();
    lvgl::statusbar_icon_set_visibility(statusbarIconId, false);

    // Read the persisted setting so an enabled server auto-starts on boot.
    bool serverEnabled;
    {
        auto lock = g_settingsMutex.asScopedLock();
        lock.lock();
        g_cachedSettings = settings::webserver::loadOrGetDefault();
        g_settingsCached = true;
        serverEnabled = g_cachedSettings.webServerEnabled;
    }
    // Subscribe to settings change events to refresh cache
    settingsEventSubscription = pubsub->subscribe([](WebServerEvent event) {
        if (event == WebServerEvent::WebServerSettingsChanged) {
            auto lock = g_settingsMutex.asScopedLock();
            lock.lock();
            g_cachedSettings = settings::webserver::loadOrGetDefault();
            g_settingsCached = true;
        }
    });

    // Start HTTP server only if enabled in settings (default: OFF to save memory)
    if (serverEnabled) {
        LOG_I(TAG, "WebServer enabled in settings, starting HTTP server...");
        setEnabled(true);
    } else {
        LOG_I(TAG, "WebServer disabled by default at startup, NOT starting HTTP server (saves ~10KB RAM)");
        setEnabled(false);
    }

    return true;
}

void WebServerService::onStop(ServiceContext& service) {
    g_webServerInstance.store(nullptr);

    pubsub->unsubscribe(settingsEventSubscription);
    settingsEventSubscription = 0;

    setEnabled(false);

    // Remove statusbar icon
    if (statusbarIconId >= 0) {
        lvgl::statusbar_icon_remove(statusbarIconId);
        statusbarIconId = -1;
    }
}

// region Enable/Disable

void WebServerService::setEnabled(bool enabled) {
    auto lock = mutex.asScopedLock();
    lock.lock();
    
    if (enabled) {
        if (!httpServer || !httpServer->isStarted()) {
            startServer();
        }
    } else {
        if (httpServer && httpServer->isStarted()) {
            stopServer();
        }
    }
}

bool WebServerService::isEnabled() const {
    auto lock = mutex.asScopedLock();
    lock.lock();
    return httpServer && httpServer->isStarted();
}

// region AP Mode WiFi Management

bool WebServerService::startApMode() {
    // Copy settings locally
    settings::webserver::WebServerSettings settings;
    {
        auto lock = g_settingsMutex.asScopedLock();
        lock.lock();
        settings = g_cachedSettings;
    }

    if (settings.wifiMode != settings::webserver::WiFiMode::AccessPoint) {
        LOG_I(TAG, "Not in AP mode, skipping AP WiFi initialization");
        return true;  // Not an error, just not needed
    }

    LOG_I(TAG, "Starting WiFi in Access Point mode...");

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    if (esp_wifi_init(&cfg) != ESP_OK) {
        LOG_E(TAG, "esp_wifi_init() failed");
        return false;
    }
    apWifiInitialized = true;

    // Create the AP network interface
    apNetif = esp_netif_create_default_wifi_ap();
    if (apNetif == nullptr) {
        LOG_E(TAG, "esp_netif_create_default_wifi_ap() failed");
        esp_wifi_deinit();
        apWifiInitialized = false;
        return false;
    }

    if (esp_wifi_set_mode(WIFI_MODE_AP) != ESP_OK) {
        LOG_E(TAG, "esp_wifi_set_mode(AP) failed");
        stopApMode();
        return false;
    }

    // Configure static IP for AP: 192.168.4.1/24
    esp_netif_ip_info_t ip_info;
    memset(&ip_info, 0, sizeof(esp_netif_ip_info_t));
    ip_info.ip.addr = ipaddr_addr("192.168.4.1");
    ip_info.gw.addr = ipaddr_addr("192.168.4.1");
    ip_info.netmask.addr = ipaddr_addr("255.255.255.0");

    if (esp_netif_dhcps_stop(apNetif) != ESP_OK) {
        LOG_E(TAG, "esp_netif_dhcps_stop() failed");
        stopApMode();
        return false;
    }

    if (esp_netif_set_ip_info(apNetif, &ip_info) != ESP_OK) {
        LOG_E(TAG, "esp_netif_set_ip_info() failed");
        stopApMode();
        return false;
    }

    if (esp_netif_dhcps_start(apNetif) != ESP_OK) {
        LOG_E(TAG, "esp_netif_dhcps_start() failed");
        stopApMode();
        return false;
    }

    // Configure WiFi AP settings
    wifi_config_t wifi_config;
    memset(&wifi_config, 0, sizeof(wifi_config_t));

    // Set SSID
    strncpy(reinterpret_cast<char*>(wifi_config.ap.ssid), settings.apSsid.c_str(), sizeof(wifi_config.ap.ssid) - 1);
    wifi_config.ap.ssid[sizeof(wifi_config.ap.ssid) - 1] = '\0';
    wifi_config.ap.ssid_len = static_cast<uint8_t>(settings.apSsid.length());

    // Set password and auth mode
    if (settings.apOpenNetwork) {
        // User explicitly chose an open network
        wifi_config.ap.authmode = WIFI_AUTH_OPEN;
        LOG_I(TAG, "AP configured with OPEN authentication (user choice)");
    } else if (settings.apPassword.length() >= 8 && settings.apPassword.length() <= 63) {
        wifi_config.ap.authmode = WIFI_AUTH_WPA2_PSK;
        strncpy(reinterpret_cast<char*>(wifi_config.ap.password), settings.apPassword.c_str(), sizeof(wifi_config.ap.password) - 1);
        wifi_config.ap.password[sizeof(wifi_config.ap.password) - 1] = '\0';
        LOG_I(TAG, "AP configured with WPA2-PSK authentication");
    } else {
        if (!settings.apPassword.empty()) {
            LOG_W(TAG, "AP password invalid (must be 8-63 chars, got %zu) - using OPEN mode", settings.apPassword.length());
        }
        wifi_config.ap.authmode = WIFI_AUTH_OPEN;
        LOG_W(TAG, "AP configured with OPEN authentication (no password)");
    }

    wifi_config.ap.max_connection = 4;
    wifi_config.ap.channel = settings.apChannel;

    if (esp_wifi_set_config(WIFI_IF_AP, &wifi_config) != ESP_OK) {
        LOG_E(TAG, "esp_wifi_set_config(AP) failed");
        stopApMode();
        return false;
    }

    if (esp_wifi_start() != ESP_OK) {
        LOG_E(TAG, "esp_wifi_start() failed");
        stopApMode();
        return false;
    }

    LOG_I(TAG, "WiFi AP started - SSID: '%s', Channel: %u, IP: 192.168.4.1", settings.apSsid.c_str(), (unsigned)settings.apChannel);
    return true;
}

void WebServerService::stopApMode() {
    if (apWifiInitialized) {
        esp_err_t err;
        if (apNetif != nullptr) {
            esp_wifi_clear_default_wifi_driver_and_handlers(apNetif);
        }
        err = esp_wifi_stop();
        if (err != ESP_OK && err != ESP_ERR_WIFI_NOT_STARTED) {
            LOG_W(TAG, "esp_wifi_stop() in cleanup: %s", esp_err_to_name(err));
        }
        LOG_I(TAG, "WiFi AP stopped");

        err = esp_wifi_set_mode(WIFI_MODE_STA);
        if (err != ESP_OK) {
            LOG_W(TAG, "esp_wifi_set_mode() in cleanup: %s", esp_err_to_name(err));
        }
        LOG_I(TAG, "Wifi mode set back to STA");

        apWifiInitialized = false;
    }

    if (apNetif != nullptr) {
        esp_netif_destroy(apNetif);
        apNetif = nullptr;
    }
}

// endregion

bool WebServerService::startServer() {
    // Copy settings locally to minimize lock duration
    settings::webserver::WebServerSettings settings;
    {
        auto lock = g_settingsMutex.asScopedLock();
        lock.lock();
        settings = g_cachedSettings;
    }

    // Start AP mode WiFi if configured
    if (settings.wifiMode == settings::webserver::WiFiMode::AccessPoint) {
        if (!startApMode()) {
            LOG_E(TAG, "Failed to start AP mode WiFi - HTTP server will not start");
            return false;
        }
    }

    // NOTE: If you see 'no slots left for registering handler', increase CONFIG_HTTPD_MAX_URI_HANDLERS in sdkconfig (default is 8, 16+ recommended for many endpoints)
    void* ctx = this;  // Avoid IDE warnings about 'this' in designated initializers
    std::vector<httpd_uri_t> handlers = {
        {
            .uri       = "/",
            .method    = HTTP_GET,
            .handler   = handleRoot,
            .user_ctx  = ctx
        },
        // Note: /upload removed in favor of POST /fs/upload handled by /fs/* dispatcher
        {
            .uri       = "/filebrowser",
            .method    = HTTP_GET,
            .handler   = handleFileBrowser,
            .user_ctx  = ctx
        },
        // Consolidated /fs/* handlers (dispatch internally) to save uri handler slots
        {
            .uri       = "/fs/*",
            .method    = HTTP_GET,
            .handler   = handleFsGenericGet,
            .user_ctx  = ctx
        },
        {
            .uri       = "/fs/*",
            .method    = HTTP_POST,
            .handler   = handleFsGenericPost,
            .user_ctx  = ctx
        },
        // Consolidated admin POST endpoints to save handler slots
        {
            .uri       = "/admin/*",
            .method    = HTTP_POST,
            .handler   = handleAdminPost,
            .user_ctx  = ctx
        },
        // API endpoints for system info, apps, wifi, etc
        {
            .uri       = "/api/*",
            .method    = HTTP_GET,
            .handler   = handleApiGet,
            .user_ctx  = ctx
        },
        {
            .uri       = "/api/*",
            .method    = HTTP_POST,
            .handler   = handleApiPost,
            .user_ctx  = ctx
        },
        {
            .uri       = "/api/*",
            .method    = HTTP_PUT,
            .handler   = handleApiPut,
            .user_ctx  = ctx
        },
        {
            .uri       = "/*",  // Catch-all for dynamic assets
            .method    = HTTP_GET,
            .handler   = handleAssets,
            .user_ctx  = ctx
        }
    };
    
    httpServer = std::make_unique<network::HttpServer>(
        settings.webServerPort,
        "0.0.0.0",
        handlers,
        8192  // Stack size
    );
    
    httpServer->start();
    if (!httpServer->isStarted()) {
        LOG_E(TAG, "Failed to start HTTP server on port %u", (unsigned)settings.webServerPort);
        httpServer.reset();
        return false;
    }

    LOG_I(TAG, "HTTP server started successfully on port %u", (unsigned)settings.webServerPort);
    publish_event(this, WebServerEvent::WebServerStarted);

    // Show statusbar icon
    if (statusbarIconId >= 0) {
        lvgl::statusbar_icon_set_image(statusbarIconId, LVGL_ICON_STATUSBAR_CLOUD);
        lvgl::statusbar_icon_set_visibility(statusbarIconId, true);
        LOG_I(TAG, "WebServer statusbar icon shown (%s mode)",
                 settings.wifiMode == settings::webserver::WiFiMode::AccessPoint ? "AP" : "Station");
    }

    return true;
}

void WebServerService::stopServer() {
    if (!httpServer) {
        return;
    }

    httpServer->stop();
    httpServer.reset();

    // Stop AP mode WiFi if we started it
    if (apWifiInitialized || apNetif != nullptr) {
        stopApMode();
    }

    LOG_I(TAG, "HTTP server stopped");
    publish_event(this, WebServerEvent::WebServerStopped);

    if (statusbarIconId >= 0) {
        lvgl::statusbar_icon_set_visibility(statusbarIconId, false);
    }
}

// region Endpoints



esp_err_t WebServerService::handleRoot(httpd_req_t* request) {
    LOG_I(TAG, "GET / -> redirecting to /dashboard.html");
    httpd_resp_set_status(request, "302 Found");
    httpd_resp_set_hdr(request, "Location", "/dashboard.html");
    return httpd_resp_send(request, nullptr, 0);
}

// region File Browser helpers & handlers

// Helper to determine content type from file extension
static const char* getContentType(const std::string& path) {
    // Check from the end to avoid matching extensions in directory names
    auto endsWith = [&path](const char* ext) {
        size_t extLen = strlen(ext);
        return path.length() >= extLen &&
               path.compare(path.length() - extLen, extLen, ext) == 0;
    };

    // HTML/Text
    if (endsWith(".html") || endsWith(".htm")) return "text/html";
    if (endsWith(".css")) return "text/css";
    if (endsWith(".js")) return "application/javascript";
    if (endsWith(".json")) return "application/json";
    if (endsWith(".xml")) return "application/xml";
    if (endsWith(".txt")) return "text/plain";

    // Images
    if (endsWith(".png")) return "image/png";
    if (endsWith(".jpg") || endsWith(".jpeg")) return "image/jpeg";
    if (endsWith(".gif")) return "image/gif";
    if (endsWith(".svg")) return "image/svg+xml";
    if (endsWith(".ico")) return "image/x-icon";
    if (endsWith(".webp")) return "image/webp";

    // Fonts
    if (endsWith(".woff")) return "font/woff";
    if (endsWith(".woff2")) return "font/woff2";
    if (endsWith(".ttf")) return "font/ttf";
    if (endsWith(".otf")) return "font/otf";
    if (endsWith(".eot")) return "application/vnd.ms-fontobject";

    // Audio/Video
    if (endsWith(".mp3")) return "audio/mpeg";
    if (endsWith(".wav")) return "audio/wav";
    if (endsWith(".ogg")) return "audio/ogg";
    if (endsWith(".mp4")) return "video/mp4";
    if (endsWith(".webm")) return "video/webm";

    // Archives/Documents
    if (endsWith(".pdf")) return "application/pdf";
    if (endsWith(".zip")) return "application/zip";
    if (endsWith(".gz")) return "application/gzip";

    // Default
    return "application/octet-stream";
}

static bool isAllowedBasePath(const std::string& path, bool allowRoot = false) {
    if (path.empty()) return false;
    // Check for ".." as a complete path component
    if (path == ".." || path.starts_with("../") ||
        path.find("/../") != std::string::npos || path.ends_with("/..")) {
        return false;
    }
    if (allowRoot && path == "/") return true;
    return path.starts_with("/data") || path.starts_with("/system/app/WebServer") || path.starts_with("/sdcard");
}

// Normalize client-supplied path: URL-decode, trim quotes/control chars, ensure leading slash, collapse duplicate slashes
static std::string normalizePath(const std::string& raw) {
    // Helper: hex to int
    auto hexVal = [](char c)->int {
        if (c >= '0' && c <= '9') return c - '0';
        if (c >= 'a' && c <= 'f') return 10 + (c - 'a');
        if (c >= 'A' && c <= 'F') return 10 + (c - 'A');
        return -1;
    };

    std::string s = raw;
    // Remove surrounding single or double quotes
    if (s.size() >= 2 && ((s.front() == '\'' && s.back() == '\'') || (s.front() == '"' && s.back() == '"'))) {
        s = s.substr(1, s.size() - 2);
    }

    // URL-decode: %xx and '+' -> ' '
    std::string decoded;
    decoded.reserve(s.size());
    for (size_t i = 0; i < s.size(); ++i) {
        char c = s[i];
        if (c == '%') {
            if (i + 2 < s.size()) {
                int hi = hexVal(s[i+1]);
                int lo = hexVal(s[i+2]);
                if (hi >= 0 && lo >= 0) {
                    decoded.push_back(static_cast<char>((hi << 4) | lo));
                    i += 2;
                    continue;
                }
            }
            // malformed %, keep it
            decoded.push_back(c);
        } else if (c == '+') {
            decoded.push_back(' ');
        } else {
            // strip control characters
            if (static_cast<unsigned char>(c) > 31) decoded.push_back(c);
        }
    }

    // Trim whitespace from ends
    size_t start = 0;
    while (start < decoded.size() && isspace((unsigned char)decoded[start])) ++start;
    size_t end = decoded.size();
    while (end > start && isspace((unsigned char)decoded[end-1])) --end;
    std::string trimmed = decoded.substr(start, end - start);

    // Ensure leading slash
    if (!trimmed.empty() && trimmed.front() != '/') trimmed = '/' + trimmed;
    if (trimmed.empty()) trimmed = "/";

    // Collapse duplicate slashes
    std::string out;
    out.reserve(trimmed.size());
    bool lastSlash = false;
    for (char c : trimmed) {
        if (c == '/') {
            if (!lastSlash) { out.push_back(c); lastSlash = true; }
        } else { out.push_back(c); lastSlash = false; }
    }

    return out;
}

static std::string escapeJson(const std::string& s) {
    std::ostringstream o;
    for (char c : s) {
        switch (c) {
            case '"': o << "\\\""; break;
            case '\\': o << "\\\\"; break;
            case '\n': o << "\\n"; break;
            case '\r': o << "\\r"; break;
            case '\t': o << "\\t"; break;
            default:
                if (static_cast<unsigned char>(c) < 0x20) {
                    o << "\\u" << std::hex << std::setw(4) << std::setfill('0') << (int)c;
                } else {
                    o << c;
                }
        }
    }
    return o.str();
}

static bool getQueryParam(httpd_req_t* req, const char* key, std::string& out) {
    size_t len = httpd_req_get_url_query_len(req) + 1;
    if (len <= 1) return false;
    std::unique_ptr<char[]> buf(new char[len]);
    if (httpd_req_get_url_query_str(req, buf.get(), len) != ESP_OK) return false;
    // Allocate buffer large enough for the entire query string (worst case)
    std::unique_ptr<char[]> value(new char[len]);
    if (httpd_query_key_value(buf.get(), key, value.get(), len) == ESP_OK) {
        out = value.get();
        return true;
    }
    return false;
}

static bool uriMatches(const char* uri, const char* route) {
    const size_t n = strlen(route);
    return strncmp(uri, route, n) == 0 && (uri[n] == '\0' || uri[n] == '?' || uri[n] == '/');
}

esp_err_t WebServerService::handleFileBrowser(httpd_req_t* request) {
    LOG_I(TAG, "GET /filebrowser -> redirecting to /dashboard.html#files");
    httpd_resp_set_status(request, "302 Found");
    httpd_resp_set_hdr(request, "Location", "/dashboard.html#files");
    return httpd_resp_send(request, nullptr, 0);
}

esp_err_t WebServerService::handleFsList(httpd_req_t* request) {
    std::string path;
    // Log raw query string for diagnostics
    size_t qlen = httpd_req_get_url_query_len(request) + 1;
    if (qlen > 1) {
        std::unique_ptr<char[]> qbuf(new char[qlen]);
        if (httpd_req_get_url_query_str(request, qbuf.get(), qlen) == ESP_OK) {
            LOG_I(TAG, "GET /fs/list raw query: %s", qbuf.get());
        }
    }

    if (!getQueryParam(request, "path", path) || path.empty()) path = "/";
    std::string norm = normalizePath(path);
    LOG_I(TAG, "GET /fs/list decoded path: '%s' normalized: '%s'", path.c_str(), norm.c_str());

    // Allow root path for listing mount points
    if (!isAllowedBasePath(norm, true)) {
        LOG_W(TAG, "GET /fs/list - invalid path requested: '%s' normalized: '%s'", path.c_str(), norm.c_str());
        httpd_resp_set_type(request, "application/json");
        httpd_resp_sendstr(request, "{\"error\":\"invalid path\"}");
        return ESP_OK;
    }

    std::ostringstream json;
    json << "{\"path\":\"" << norm << "\",\"entries\":[";
    struct FsIterContext {
        std::ostringstream& json;
        uint16_t count = 0;
    };
    FsIterContext fs_iter_context { json };
    // Special handling for root: show available mount points
    if (norm == "/") {
        file_system_for_each(&fs_iter_context, [] (auto* fs, void* context) {
            auto* fs_iter_context = static_cast<FsIterContext*>(context);
            char path[128];
            if (file_system_is_mounted(fs) && file_system_get_path(fs, path, sizeof(path)) == ERROR_NONE && strcmp(path, "/system") != 0) {
                fs_iter_context->count++;
                if (fs_iter_context->count != 1) fs_iter_context->json << ","; // add separator between json array entries
                fs_iter_context->json << "{\"name\":\"" << path << "\",\"type\":\"dir\",\"size\":0}";
            }
            return true;
        });

        json << "]}";
    } else {
        std::vector<dirent> entries;
        int res = file::scandir(norm, entries, file::direntFilterDotEntries, nullptr);
        if (res < 0) {
            httpd_resp_set_type(request, "application/json");
            httpd_resp_sendstr(request, "{\"error\":\"scan failed\"}");
            return ESP_OK;
        }
        bool first = true;
        for (auto& e : entries) {
            if (!first) json << ','; else first = false;
            std::string name = e.d_name;
            bool is_dir = (e.d_type == file::TT_DT_DIR || e.d_type == file::TT_DT_CHR);
            std::string full = norm + "/" + name;
            long size = 0;
            if (!is_dir) {
                struct stat st;
                if (stat(full.c_str(), &st) == 0) {
                    size = st.st_size;
                }
            }
            json << "{\"name\":\"" << escapeJson(name) << "\",\"type\":\"" << (is_dir?"dir":"file") << "\",\"size\":" << size << "}";
        }
        json << "]}";
    }

    httpd_resp_set_type(request, "application/json");
    httpd_resp_sendstr(request, json.str().c_str());
    return ESP_OK;
}

esp_err_t WebServerService::handleFsDownload(httpd_req_t* request) {
    std::string path;
    if (!getQueryParam(request, "path", path) || path.empty()) {
        httpd_resp_send_err(request, HTTPD_400_BAD_REQUEST, "path required");
        return ESP_FAIL;
    }
    std::string norm = normalizePath(path);
    if (!isAllowedBasePath(norm) || !file::isFile(norm)) {
        LOG_W(TAG, "GET /fs/download - not found or invalid path: '%s' normalized: '%s'", path.c_str(), norm.c_str());
        httpd_resp_send_err(request, HTTPD_404_NOT_FOUND, "not found");
        return ESP_FAIL;
    }
    httpd_resp_set_type(request, getContentType(norm));
    // Suggest download - build header into a local string so it remains valid
    std::string fname = file::getLastPathSegment(norm);
    std::string disposition = std::string("attachment; filename=\"") + fname + "\"";
    // RFC5987 fallback (filename*): percent-encode UTF-8 bytes for wider browser compatibility
    auto pctEncode = [](const std::string& s)->std::string{
        std::ostringstream oss;
        for (unsigned char c : s) {
            if (std::isalnum(c) || c=='-' || c=='.' || c=='_' || c=='~') {
                oss << c;
            } else {
                oss << '%';
                std::ostringstream hex;
                hex << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << (int)c;
                oss << hex.str();
            }
        }
        return oss.str();
    };
    std::string pct = pctEncode(fname);
    if (!pct.empty()) {
        disposition += std::string("; filename*=UTF-8''") + pct;
    }
    // Set single Content-Disposition header (avoid adding duplicate headers)
    httpd_resp_set_hdr(request, "Content-Disposition", disposition.c_str());
    FILE* fp = fopen(norm.c_str(), "rb");
    if (!fp) { httpd_resp_send_err(request, HTTPD_500_INTERNAL_SERVER_ERROR, "open failed"); return ESP_FAIL; }
    char buf[512]; size_t n;
    while ((n = fread(buf,1,sizeof(buf),fp))>0) {
        if (httpd_resp_send_chunk(request, buf, n) != ESP_OK) { fclose(fp); return ESP_FAIL; }
    }
    fclose(fp);
    httpd_resp_send_chunk(request, nullptr, 0);
    return ESP_OK;
}

esp_err_t WebServerService::handleFsUpload(httpd_req_t* request) {
    std::string path;

    // Log raw query and decoded path for diagnostics
    size_t qlen = httpd_req_get_url_query_len(request) + 1;
    if (qlen > 1) {
        std::unique_ptr<char[]> qbuf(new char[qlen]);
        if (httpd_req_get_url_query_str(request, qbuf.get(), qlen) == ESP_OK) {
            LOG_I(TAG, "POST /fs/upload raw query: %s", qbuf.get());
        }
    }

    if (!getQueryParam(request, "path", path) || path.empty()) {
        httpd_resp_send_err(request, HTTPD_400_BAD_REQUEST, "path required");
        return ESP_FAIL;
    }

    // Log decoded path and headers
    char content_type[64] = {0};
    httpd_req_get_hdr_value_str(request, "Content-Type", content_type, sizeof(content_type));
    std::string norm = normalizePath(path);
    LOG_I(TAG, "POST /fs/upload decoded path: '%s' normalized: '%s' Content-Length: %d Content-Type: %s", path.c_str(), norm.c_str(), (int)request->content_len, content_type[0] ? content_type : "(null)");

    if (!isAllowedBasePath(norm)) {
        LOG_W(TAG, "POST /fs/upload - invalid path requested: '%s' normalized: '%s'", path.c_str(), norm.c_str());
        httpd_resp_send_err(request, HTTPD_403_FORBIDDEN, "invalid path");
        return ESP_FAIL;
    }

    if (request->content_len > MAX_UPLOAD_SIZE) {
        httpd_resp_send_err(request, HTTPD_400_BAD_REQUEST, "file too large");
        return ESP_FAIL;
    }

    // Ensure parent directory exists (after size check to avoid creating dirs for rejected uploads)
    if (!file::findOrCreateParentDirectory(norm, 0755)) {
        httpd_resp_send_err(request, HTTPD_500_INTERNAL_SERVER_ERROR, "failed to create parent directory");
        return ESP_FAIL;
    }
    FILE* fp = fopen(norm.c_str(), "wb");
    if (!fp) { httpd_resp_send_err(request, HTTPD_500_INTERNAL_SERVER_ERROR, "open failed"); return ESP_FAIL; }
    char buf[512]; int remaining = request->content_len; int received=0;
    constexpr int MAX_TIMEOUT_RETRIES = 5;
    int timeout_retries = 0;
    while (remaining > 0) {
        int to_read = remaining > (int)sizeof(buf) ? (int)sizeof(buf) : remaining;
        int ret = httpd_req_recv(request, buf, to_read);
        if (ret == HTTPD_SOCK_ERR_TIMEOUT) {
            // Timeout - retry with backoff
            timeout_retries++;
            if (timeout_retries >= MAX_TIMEOUT_RETRIES) {
                LOG_E(TAG, "Upload recv timeout after %d retries", timeout_retries);
                fclose(fp);
                remove(norm.c_str());  // Clean up partial file
                httpd_resp_send_err(request, HTTPD_500_INTERNAL_SERVER_ERROR, "recv timeout");
                return ESP_FAIL;
            }
            LOG_W(TAG, "Upload recv timeout, retry %d/%d", timeout_retries, MAX_TIMEOUT_RETRIES);
            vTaskDelay(pdMS_TO_TICKS(100 * timeout_retries)); // Linear backoff
            continue;
        }
        if (ret <= 0) {
            LOG_E(TAG, "Upload recv failed with error %d", ret);
            fclose(fp);
            remove(norm.c_str());  // Clean up partial file
            httpd_resp_send_err(request, HTTPD_500_INTERNAL_SERVER_ERROR, "recv failed");
            return ESP_FAIL;
        }
        // Successful read - reset timeout counter
        timeout_retries = 0;
        size_t written = fwrite(buf, 1, ret, fp);
        if (written != (size_t)ret) {
            fclose(fp);
            remove(norm.c_str());
            httpd_resp_send_err(request, HTTPD_500_INTERNAL_SERVER_ERROR, "write failed");
            return ESP_FAIL;
        }
        remaining -= ret;
        received += ret;
    }
    fclose(fp);
    httpd_resp_set_type(request, "text/plain");
    std::string msg = std::string("Uploaded ") + std::to_string(received) + " bytes";
    httpd_resp_sendstr(request, msg.c_str());
    return ESP_OK;
}

// Generic GET dispatcher for /fs/* URIs
esp_err_t WebServerService::handleFsGenericGet(httpd_req_t* request) {
    // Auth check for all /fs/* endpoints (file system access is sensitive)
    bool authPassed = false;
    esp_err_t authResult = validateRequestAuth(request, authPassed);
    if (!authPassed) {
        return authResult;
    }

    const char* uri = request->uri;
    if (uriMatches(uri, "/fs/list")) return handleFsList(request);
    if (uriMatches(uri, "/fs/download")) return handleFsDownload(request);
    if (uriMatches(uri, "/fs/tree")) return handleFsTree(request);
    LOG_W(TAG, "GET %s - not found in fs generic dispatcher", uri);
    httpd_resp_send_err(request, HTTPD_404_NOT_FOUND, "not found");
    return ESP_FAIL;
}

// Generic POST dispatcher for /fs/* URIs
esp_err_t WebServerService::handleFsGenericPost(httpd_req_t* request) {
    // Auth check for all /fs/* endpoints (file system access is sensitive)
    bool authPassed = false;
    esp_err_t authResult = validateRequestAuth(request, authPassed);
    if (!authPassed) {
        return authResult;
    }

    const char* uri = request->uri;
    if (uriMatches(uri, "/fs/mkdir")) return handleFsMkdir(request);
    if (uriMatches(uri, "/fs/delete")) return handleFsDelete(request);
    if (uriMatches(uri, "/fs/rename")) return handleFsRename(request);
    if (uriMatches(uri, "/fs/upload")) return handleFsUpload(request);
    LOG_W(TAG, "POST %s - not found in fs generic dispatcher", uri);
    httpd_resp_send_err(request, HTTPD_404_NOT_FOUND, "not found");
    return ESP_FAIL;
}

// Admin dispatcher for consolidated small POST endpoints (e.g. sync, reboot)
esp_err_t WebServerService::handleAdminPost(httpd_req_t* request) {
    // Auth check for all /admin/* endpoints (admin actions are sensitive)
    bool authPassed = false;
    esp_err_t authResult = validateRequestAuth(request, authPassed);
    if (!authPassed) {
        return authResult;
    }

    const char* uri = request->uri;
    if (strncmp(uri, "/admin/reboot", 13) == 0) return handleReboot(request);
    LOG_I(TAG, "POST %s - not found in admin dispatcher", uri);
    httpd_resp_send_err(request, HTTPD_404_NOT_FOUND, "not found");
    return ESP_FAIL;
}

// API GET dispatcher - returns JSON system information
// Note: /api/sysinfo is intentionally public for monitoring use cases
esp_err_t WebServerService::handleApiGet(httpd_req_t* request) {
    const char* uri = request->uri;

    // Public endpoint: sysinfo (basic device info for monitoring)
    if (strncmp(uri, "/api/sysinfo", 12) == 0) {
        return handleApiSysinfo(request);
    }

    // Protected endpoints require authentication
    bool authPassed = false;
    esp_err_t authResult = validateRequestAuth(request, authPassed);
    if (!authPassed) {
        return authResult;
    }

    // Auth-protected endpoints
    if (strncmp(uri, "/api/apps", 9) == 0) {
        return handleApiApps(request);
    }
    if (strncmp(uri, "/api/wifi", 9) == 0) {
        return handleApiWifi(request);
    }
    if (strncmp(uri, "/api/screenshot", 15) == 0) {
        return handleApiScreenshot(request);
    }

    LOG_W(TAG, "GET %s - not found in api dispatcher", uri);
    httpd_resp_send_err(request, HTTPD_404_NOT_FOUND, "not found");
    return ESP_FAIL;
}

// API POST dispatcher - all POST endpoints require authentication
esp_err_t WebServerService::handleApiPost(httpd_req_t* request) {
    bool authPassed = false;
    esp_err_t authResult = validateRequestAuth(request, authPassed);
    if (!authPassed) {
        return authResult;
    }

    const char* uri = request->uri;
    if (strncmp(uri, "/api/apps/run", 13) == 0) {
        return handleApiAppsRun(request);
    }
    if (strncmp(uri, "/api/apps/uninstall", 19) == 0) {
        return handleApiAppsUninstall(request);
    }
    if (strncmp(uri, "/api/video/start", 15) == 0) {
        return handleApiVideoStart(request);
    }
    if (strncmp(uri, "/api/video/stop", 15) == 0) {
        return handleApiVideoStop(request);
    }

    LOG_W(TAG, "POST %s - not found in api dispatcher", uri);
    httpd_resp_send_err(request, HTTPD_404_NOT_FOUND, "not found");
    return ESP_FAIL;
}

// API PUT dispatcher - all PUT endpoints require authentication
esp_err_t WebServerService::handleApiPut(httpd_req_t* request) {
    bool authPassed = false;
    esp_err_t authResult = validateRequestAuth(request, authPassed);
    if (!authPassed) {
        return authResult;
    }

    const char* uri = request->uri;
    if (strncmp(uri, "/api/apps/install", 17) == 0) {
        return handleApiAppsInstall(request);
    }

    LOG_W(TAG, "PUT %s - not found in api dispatcher", uri);
    httpd_resp_send_err(request, HTTPD_404_NOT_FOUND, "not found");
    return ESP_FAIL;
}

esp_err_t WebServerService::handleApiSysinfo(httpd_req_t* request) {
    LOG_I(TAG, "GET /api/sysinfo");

    std::ostringstream json;
    json << "{";

    // Firmware info
    json << "\"firmware\":{";
    json << "\"version\":\"" << TT_VERSION << "\",";
    json << "\"idf_version\":\"" << ESP_IDF_VERSION_MAJOR << "." << ESP_IDF_VERSION_MINOR << "." << ESP_IDF_VERSION_PATCH << "\"";
    json << "},";

    // Chip info
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    json << "\"chip\":{";
    json << "\"model\":\"" << getChipModelName(chip_info.model) << "\",";
    json << "\"cores\":" << (int)chip_info.cores << ",";
    json << "\"revision\":" << (int)chip_info.revision << ",";

    // Decode features into an array of strings
    json << "\"features\":[";
    bool first_feature = true;
    if (chip_info.features & CHIP_FEATURE_EMB_FLASH) {
        json << "\"Embedded Flash\"";
        first_feature = false;
    }
    if (chip_info.features & CHIP_FEATURE_WIFI_BGN) {
        if (!first_feature) json << ",";
        json << "\"WiFi 2.4GHz\"";
        first_feature = false;
    }
    if (chip_info.features & CHIP_FEATURE_BLE) {
        if (!first_feature) json << ",";
        json << "\"BLE\"";
        first_feature = false;
    }
    if (chip_info.features & CHIP_FEATURE_BT) {
        if (!first_feature) json << ",";
        json << "\"Bluetooth Classic\"";
        first_feature = false;
    }
    if (chip_info.features & CHIP_FEATURE_IEEE802154) {
        if (!first_feature) json << ",";
        json << "\"IEEE 802.15.4\"";
        first_feature = false;
    }
    if (chip_info.features & CHIP_FEATURE_EMB_PSRAM) {
        if (!first_feature) json << ",";
        json << "\"Embedded PSRAM\"";
    }
    json << "],";

    // Internal flash size
    uint32_t flash_size = 0;
    esp_flash_get_size(nullptr, &flash_size);
    json << "\"flash_size\":" << flash_size;
    json << "},";

    // Memory - Internal heap
    size_t heap_free = heap_caps_get_free_size(MALLOC_CAP_INTERNAL);
    size_t heap_total = heap_caps_get_total_size(MALLOC_CAP_INTERNAL);
    size_t heap_min_free = heap_caps_get_minimum_free_size(MALLOC_CAP_INTERNAL);
    size_t heap_largest = heap_caps_get_largest_free_block(MALLOC_CAP_INTERNAL);

    json << "\"heap\":{";
    json << "\"free\":" << heap_free << ",";
    json << "\"total\":" << heap_total << ",";
    json << "\"min_free\":" << heap_min_free << ",";
    json << "\"largest_block\":" << heap_largest;
    json << "},";

    // Memory - PSRAM (external)
    size_t psram_free = heap_caps_get_free_size(MALLOC_CAP_SPIRAM);
    size_t psram_total = heap_caps_get_total_size(MALLOC_CAP_SPIRAM);
    size_t psram_min_free = heap_caps_get_minimum_free_size(MALLOC_CAP_SPIRAM);
    size_t psram_largest = heap_caps_get_largest_free_block(MALLOC_CAP_SPIRAM);

    json << "\"psram\":{";
    json << "\"free\":" << psram_free << ",";
    json << "\"total\":" << psram_total << ",";
    json << "\"min_free\":" << psram_min_free << ",";
    json << "\"largest_block\":" << psram_largest;
    json << "},";

    // Storage info
    json << "\"storage\":{";
    uint64_t storage_total = 0, storage_free = 0;

    struct FsIterContext {
        std::ostringstream& json;
        uint16_t count = 0;
    };
    FsIterContext fs_iter_context { json };
    file_system_for_each(&fs_iter_context, [] (auto* fs, void* context) {
        char mount_path[128] = "";
        if (file_system_get_path(fs, mount_path, sizeof(mount_path)) != ERROR_NONE) return true;
        if (strcmp(mount_path, "/system") == 0) return true; // Hide system partition

        bool mounted = file_system_is_mounted(fs);
        auto* fs_iter_context = static_cast<FsIterContext*>(context);
        auto& json_context = fs_iter_context->json;
        std::string mount_path_cpp = mount_path;

        fs_iter_context->count++;
        if (fs_iter_context->count != 1) json_context << ","; // add separator between json array entries
        json_context << "\"" << mount_path_cpp.substr(1) << "\":{";

        uint64_t storage_total = 0, storage_free = 0;
        if (esp_vfs_fat_info(mount_path, &storage_total, &storage_free) == ESP_OK) {
            json_context << "\"free\":" << storage_free << ",";
            json_context << "\"total\":" << storage_total << ",";
        } else {
            json_context << "\"free\":0,";
            json_context << "\"total\":0,";
        }

        json_context << "\"mounted\":" << (mounted ? "true" : "false") << "";
        json_context << "}";
        return true;
    });

    json << "},";  // end storage

    // Uptime (in seconds)
    TickType_t ticks = xTaskGetTickCount();
    float uptime_sec = static_cast<float>(ticks) / configTICK_RATE_HZ;
    json << "\"uptime\":" << static_cast<int>(uptime_sec) << ",";

    // Task count
    UBaseType_t task_count = uxTaskGetNumberOfTasks();
    json << "\"task_count\":" << task_count << ",";

    // Feature flags
    json << "\"features_enabled\":{";
#if TT_FEATURE_SCREENSHOT_API
    json << "\"screenshot\":true";
#else
    json << "\"screenshot\":false";
#endif
    json << "}";

    json << "}";

    httpd_resp_set_type(request, "application/json");
    httpd_resp_sendstr(request, json.str().c_str());
    return ESP_OK;
}

// GET /api/apps - List installed apps
esp_err_t WebServerService::handleApiApps(httpd_req_t* request) {
    LOG_I(TAG, "GET /api/apps");

    std::vector<const ::AppManifest*> manifests;
    app_manager_for_each_manifest([](const ::AppManifest* manifest, void* context) {
        static_cast<std::vector<const ::AppManifest*>*>(context)->push_back(manifest);
    }, &manifests);

    std::ostringstream json;
    json << "{\"apps\":[";

    bool first = true;
    for (const auto* manifest : manifests) {
        if (!first) json << ",";
        first = false;

        json << "{";
        json << "\"id\":\"" << escapeJson(manifest->id) << "\",";
        json << "\"name\":\"" << escapeJson(manifest->name) << "\",";

        const char* category = "user";
        if (manifest->category == APP_CATEGORY_SYSTEM) category = "system";
        else if (manifest->category == APP_CATEGORY_SETTINGS) category = "settings";
        json << "\"category\":\"" << category << "\",";

        json << "\"isExternal\":" << (manifest->location.type == APP_LOCATION_PATH ? "true" : "false") << ",";
        json << "\"hidden\":" << ((manifest->flags & APP_MANIFEST_FLAG_HIDDEN) ? "true" : "false");

        json << "}";
    }

    json << "]}";

    httpd_resp_set_type(request, "application/json");
    httpd_resp_sendstr(request, json.str().c_str());
    return ESP_OK;
}

// POST /api/apps/run?id=xxx - Run an app
esp_err_t WebServerService::handleApiAppsRun(httpd_req_t* request) {
    LOG_I(TAG, "POST /api/apps/run");

    std::string appId;
    if (!getQueryParam(request, "id", appId) || appId.empty()) {
        httpd_resp_send_err(request, HTTPD_400_BAD_REQUEST, "id parameter required");
        return ESP_FAIL;
    }

    AppManifest manifest;
    if (app_manager_find_manifest(appId.c_str(), &manifest) != ERROR_NONE) {
        httpd_resp_send_err(request, HTTPD_404_NOT_FOUND, "app not found");
        return ESP_FAIL;
    }

    // Every app instance gets its own task now, so there's no "stop the existing one first" -
    // this just starts a fresh instance alongside whatever's already running.
    AppInstanceId instance_id = 0;
    app_manager_start(appId.c_str(), &instance_id);

    LOG_I(TAG, "[200] /api/apps/run %s", appId.c_str());
    httpd_resp_sendstr(request, "ok");
    return ESP_OK;
}

// POST /api/apps/uninstall?id=xxx - Uninstall an app
esp_err_t WebServerService::handleApiAppsUninstall(httpd_req_t* request) {
    LOG_I(TAG, "POST /api/apps/uninstall");

    std::string appId;
    if (!getQueryParam(request, "id", appId) || appId.empty()) {
        httpd_resp_send_err(request, HTTPD_400_BAD_REQUEST, "id parameter required");
        return ESP_FAIL;
    }

    AppManifest manifest;
    if (app_manager_find_manifest(appId.c_str(), &manifest) != ERROR_NONE) {
        LOG_I(TAG, "[200] /api/apps/uninstall %s (app wasn't installed)", appId.c_str());
        httpd_resp_sendstr(request, "ok");
        return ESP_OK;
    }

    // Only allow uninstalling external (side-loaded) apps
    if (manifest.location.type != APP_LOCATION_PATH) {
        httpd_resp_send_err(request, HTTPD_403_FORBIDDEN, "cannot uninstall system apps");
        return ESP_FAIL;
    }

    if (app_uninstall(appId.c_str()) == ERROR_NONE) {
        LOG_I(TAG, "[200] /api/apps/uninstall %s", appId.c_str());
        httpd_resp_sendstr(request, "ok");
        return ESP_OK;
    } else {
        LOG_W(TAG, "[500] /api/apps/uninstall %s", appId.c_str());
        httpd_resp_send_err(request, HTTPD_500_INTERNAL_SERVER_ERROR, "uninstall failed");
        return ESP_FAIL;
    }
}

// PUT /api/apps/install - Install an app from multipart form upload
esp_err_t WebServerService::handleApiAppsInstall(httpd_req_t* request) {
    LOG_I(TAG, "PUT /api/apps/install");

    std::string boundary;
    if (!network::getMultiPartBoundaryOrSendError(request, boundary)) {
        return ESP_FAIL;
    }

    size_t content_left = request->content_len;
    constexpr size_t MAX_APP_UPLOAD_SIZE = 20 * 1024 * 1024;

    // Read headers until empty line (skip boundary line first)
    auto content_headers_data = network::receiveTextUntil(request, "\r\n\r\n");
    content_left -= content_headers_data.length();

    // Split headers into lines and filter empty ones
    auto content_headers = string::split(content_headers_data, "\r\n")
        | std::views::filter([](const std::string& line) {
            return line.length() > 0;
        })
        | std::ranges::to<std::vector>();

    auto content_disposition_map = network::parseContentDisposition(content_headers);
    if (content_disposition_map.empty()) {
        LOG_W(TAG, "parseContentDisposition returned empty map for: %s", content_headers_data.c_str());
        httpd_resp_send_err(request, HTTPD_400_BAD_REQUEST, "invalid content disposition");
        return ESP_FAIL;
    }

    auto filename_entry = content_disposition_map.find("filename");
    if (filename_entry == content_disposition_map.end()) {
        LOG_W(TAG, "filename not found in content disposition map");
        httpd_resp_send_err(request, HTTPD_400_BAD_REQUEST, "filename parameter missing");
        return ESP_FAIL;
    }

    // Calculate file size
    auto boundary_and_newlines_after_file = std::format("\r\n--{}--\r\n", boundary);
    if (content_left <= boundary_and_newlines_after_file.length()) {
        httpd_resp_send_err(request, HTTPD_400_BAD_REQUEST, "invalid multipart payload");
        return ESP_FAIL;
    }

    auto file_size = content_left - boundary_and_newlines_after_file.length();
    if (file_size == 0 || file_size > MAX_APP_UPLOAD_SIZE) {
        httpd_resp_send_err(request, HTTPD_400_BAD_REQUEST, "file too large");
        return ESP_FAIL;
    }

    // Create tmp directory
    const std::string tmp_path = getTempPath();
    if (!file::findOrCreateDirectory(tmp_path, 0777)) {
        httpd_resp_send_err(request, HTTPD_500_INTERNAL_SERVER_ERROR, "failed to create temp directory");
        return ESP_FAIL;
    }

    std::string safe_name = file::getLastPathSegment(filename_entry->second);
    if (safe_name.empty() || safe_name.find("..") != std::string::npos ||
        safe_name.find('/') != std::string::npos || safe_name.find('\\') != std::string::npos) {
        httpd_resp_send_err(request, HTTPD_400_BAD_REQUEST, "invalid filename");
        return ESP_FAIL;
    }
    auto file_path = std::format("{}/{}", tmp_path, safe_name);

    if (network::receiveFile(request, file_size, file_path) != file_size) {
        file::deleteFile(file_path);
        httpd_resp_send_err(request, HTTPD_500_INTERNAL_SERVER_ERROR, "failed to save file");
        return ESP_FAIL;
    }

    content_left -= file_size;

    // Read and discard trailing boundary
    if (!network::readAndDiscardOrSendError(request, boundary_and_newlines_after_file)) {
        return ESP_FAIL;
    }

    // Install the app
    if (app_install(file_path.c_str()) != ERROR_NONE) {
        file::deleteFile(file_path);
        httpd_resp_send_err(request, HTTPD_500_INTERNAL_SERVER_ERROR, "installation failed");
        return ESP_FAIL;
    }

    // Cleanup temp file
    if (!file::deleteFile(file_path)) {
        LOG_W(TAG, "Failed to delete temp file %s", file_path.c_str());
    }

    LOG_I(TAG, "[200] /api/apps/install -> %s", file_path.c_str());
    httpd_resp_sendstr(request, "ok");
    return ESP_OK;
}

// Helper to convert radio state to string
static const char* radioStateToJsonString(wifi::RadioState state) {
    switch (state) {
        case wifi::RadioState::On: return "on";
        case wifi::RadioState::OnPending: return "turning_on";
        case wifi::RadioState::Off: return "off";
        case wifi::RadioState::OffPending: return "turning_off";
        case wifi::RadioState::ConnectionPending: return "connecting";
        case wifi::RadioState::ConnectionActive: return "connected";
        default: return "unknown";
    }
}

// GET /api/wifi - WiFi status
esp_err_t WebServerService::handleApiWifi(httpd_req_t* request) {
    LOG_I(TAG, "GET /api/wifi");

    auto state = wifi::getRadioState();
    auto ip = wifi::getIp();
    auto ssid = wifi::getConnectionTarget();
    auto rssi = wifi::getRssi();
    bool secure = wifi::isConnectionSecure();

    std::ostringstream json;
    json << "{";
    json << "\"state\":\"" << radioStateToJsonString(state) << "\",";
    json << "\"ip\":\"" << escapeJson(ip) << "\",";
    json << "\"ssid\":\"" << escapeJson(ssid) << "\",";
    json << "\"rssi\":" << rssi << ",";
    json << "\"secure\":" << (secure ? "true" : "false");
    json << "}";

    httpd_resp_set_type(request, "application/json");
    httpd_resp_sendstr(request, json.str().c_str());
    return ESP_OK;
}

// GET /api/screenshot - Capture the active LVGL screen and return it as an image.
// Defaults to PNG (truecolor RGB, compressed with miniz's deflate). Pass ?format=bmp for a
// 24-bit BMP instead. Captures via LVGL's built-in snapshot API (lv_snapshot_take).
static void screenshot_write_u32_be(std::vector<uint8_t>& v, uint32_t val) {
    v.push_back((val >> 24) & 0xFF);
    v.push_back((val >> 16) & 0xFF);
    v.push_back((val >> 8) & 0xFF);
    v.push_back(val & 0xFF);
}

static uint32_t screenshot_crc32(uint32_t crc, const uint8_t* data, size_t len) {
    static uint32_t table[256];
    static bool table_ready = false;
    if (!table_ready) {
        for (uint32_t i = 0; i < 256; ++i) {
            uint32_t c = i;
            for (int k = 0; k < 8; ++k) {
                c = (c & 1) ? (0xEDB88320u ^ (c >> 1)) : (c >> 1);
            }
            table[i] = c;
        }
        table_ready = true;
    }
    for (size_t i = 0; i < len; ++i) {
        crc = table[(crc ^ data[i]) & 0xFF] ^ (crc >> 8);
    }
    return crc;
}

static uint32_t screenshot_adler32(const uint8_t* data, size_t len) {
    uint32_t a = 1;
    uint32_t b = 0;
    for (size_t i = 0; i < len; ++i) {
        a = (a + data[i]) % 65521u;
        b = (b + a) % 65521u;
    }
    return (b << 16) | a;
}

static void screenshot_png_chunk(std::vector<uint8_t>& out, const char* type, const uint8_t* data, size_t len) {
    screenshot_write_u32_be(out, static_cast<uint32_t>(len));
    // PNG CRC-32 is over the chunk type then the chunk data.
    uint32_t crc = screenshot_crc32(0xFFFFFFFFu, reinterpret_cast<const uint8_t*>(type), 4);
    if (len != 0) {
        crc = screenshot_crc32(crc, data, len);
    }
    crc ^= 0xFFFFFFFFu;
    out.insert(out.end(), type, type + 4);
    if (len != 0) {
        out.insert(out.end(), data, data + len);
    }
    screenshot_write_u32_be(out, crc);
}

// A small, self-contained deflate compressor (RFC 1951, fixed Huffman + LZ77 hash-chain) used for
// the PNG IDAT payload. The ROM miniz deflate returned 0 on this chip, so this guarantees a small
// PNG without depending on it. Produces a raw deflate stream (no zlib wrapper) with a single fixed
// (BTYPE=01) block. Only fast enough for occasional screenshots.
static bool screenshot_deflate(const uint8_t* src, size_t src_len, std::vector<uint8_t>& out) {
    struct BitWriter {
        std::vector<uint8_t> buf;
        uint32_t bitbuf = 0;
        int bitcount = 0;
        void writeCode(uint32_t code, int num_bits) {
            for (int i = num_bits - 1; i >= 0; --i) { // Huffman code: MSB first
                if ((code >> i) & 1) bitbuf |= (1u << bitcount);
                if (++bitcount == 8) { buf.push_back(static_cast<uint8_t>(bitbuf)); bitbuf = 0; bitcount = 0; }
            }
        }
        void writeBits(uint32_t value, int num_bits) {
            for (int i = 0; i < num_bits; ++i) { // extra bits: LSB first
                if ((value >> i) & 1) bitbuf |= (1u << bitcount);
                if (++bitcount == 8) { buf.push_back(static_cast<uint8_t>(bitbuf)); bitbuf = 0; bitcount = 0; }
            }
        }
        void flush() {
            if (bitcount > 0) { buf.push_back(static_cast<uint8_t>(bitbuf)); bitbuf = 0; bitcount = 0; }
        }
    };

    // Fixed Huffman (RFC 1951 3.2.6): symbol -> (code, bits).
    auto fixed_huff = [](uint32_t sym, uint32_t& bits) -> uint32_t {
        if (sym <= 143) { bits = 8; return 0x30 + sym; }
        if (sym <= 255) { bits = 9; return 0x190 + (sym - 144); }
        if (sym <= 279) { bits = 7; return sym - 256; }
        if (sym <= 287) { bits = 8; return 0xC0 + (sym - 280); }
        bits = 5; return sym; // distance code 0-31
    };

    auto length_code = [](uint32_t len, uint32_t& code, uint32_t& extra, uint32_t& extra_bits) {
        if (len <= 10) { code = 257 + len - 3; extra = 0; extra_bits = 0; }
        else if (len <= 18) { code = 265 + (len - 11) / 2; extra = (len - 11) % 2; extra_bits = 1; }
        else if (len <= 34) { code = 269 + (len - 19) / 4; extra = (len - 19) % 4; extra_bits = 2; }
        else if (len <= 66) { code = 273 + (len - 35) / 8; extra = (len - 35) % 8; extra_bits = 3; }
        else if (len <= 130) { code = 277 + (len - 67) / 16; extra = (len - 67) % 16; extra_bits = 4; }
        else if (len <= 257) { code = 281 + (len - 131) / 32; extra = (len - 131) % 32; extra_bits = 5; }
        else { code = 285; extra = 0; extra_bits = 0; }
    };

    auto dist_code = [](uint32_t dist, uint32_t& code, uint32_t& extra, uint32_t& extra_bits) {
        if (dist <= 4) { code = dist - 1; extra = 0; extra_bits = 0; }
        else if (dist <= 8) { code = 4 + (dist - 5) / 2; extra = (dist - 5) % 2; extra_bits = 1; }
        else if (dist <= 16) { code = 6 + (dist - 9) / 4; extra = (dist - 9) % 4; extra_bits = 2; }
        else if (dist <= 32) { code = 8 + (dist - 17) / 8; extra = (dist - 17) % 8; extra_bits = 3; }
        else if (dist <= 64) { code = 10 + (dist - 33) / 16; extra = (dist - 33) % 16; extra_bits = 4; }
        else if (dist <= 128) { code = 12 + (dist - 65) / 32; extra = (dist - 65) % 32; extra_bits = 5; }
        else if (dist <= 256) { code = 14 + (dist - 129) / 64; extra = (dist - 129) % 64; extra_bits = 6; }
        else if (dist <= 512) { code = 16 + (dist - 257) / 128; extra = (dist - 257) % 128; extra_bits = 7; }
        else if (dist <= 1024) { code = 18 + (dist - 513) / 256; extra = (dist - 513) % 256; extra_bits = 8; }
        else if (dist <= 2048) { code = 20 + (dist - 1025) / 512; extra = (dist - 1025) % 512; extra_bits = 9; }
        else if (dist <= 4096) { code = 22 + (dist - 2049) / 1024; extra = (dist - 2049) % 1024; extra_bits = 10; }
        else if (dist <= 8192) { code = 24 + (dist - 4097) / 2048; extra = (dist - 4097) % 2048; extra_bits = 11; }
        else if (dist <= 16384) { code = 26 + (dist - 8193) / 4096; extra = (dist - 8193) % 4096; extra_bits = 12; }
        else { code = 28 + (dist - 16385) / 8192; extra = (dist - 16385) % 8192; extra_bits = 13; }
    };

    constexpr uint32_t HASH_BITS = 15;
    constexpr uint32_t HASH_SIZE = 1u << HASH_BITS;
    constexpr uint32_t HASH_MASK = HASH_SIZE - 1;
    std::vector<int> head(HASH_SIZE, -1);
    std::vector<int> prev(src_len, -1); // ~2 bytes/byte of input; fine for occasional screenshots

    auto hash3 = [HASH_MASK](const uint8_t* d, size_t i, size_t n) -> uint32_t {
        if (i + 2 >= n) return 0;
        return (((uint32_t)d[i] << 10) ^ ((uint32_t)d[i + 1] << 5) ^ d[i + 2]) & HASH_MASK;
    };

    BitWriter bw;
    bw.writeBits(3, 3); // BFINAL=1, BTYPE=01 (fixed Huffman, single block)

    size_t pos = 0;
    while (pos < src_len) {
        const uint32_t h = hash3(src, pos, src_len);
        int best_len = 0;
        int best_dist = 0;
        int cand = head[h];
        int chain = 0;
        while (cand >= 0 && chain < 64 && static_cast<size_t>(cand) + 3 <= src_len) {
            const size_t dist = pos - static_cast<size_t>(cand);
            if (dist > 32768) break;
            int len = 0;
            while (pos + static_cast<size_t>(len) < src_len && len < 258 &&
                   src[cand + len] == src[pos + static_cast<size_t>(len)]) {
                ++len;
            }
            if (len >= 3 && len > best_len) {
                best_len = len;
                best_dist = static_cast<int>(dist);
            }
            cand = prev[cand];
            ++chain;
        }

        if (best_len >= 3) {
            uint32_t lc, le, leb;
            length_code(static_cast<uint32_t>(best_len), lc, le, leb);
            uint32_t clen;
            bw.writeCode(fixed_huff(lc, clen), static_cast<int>(clen));
            bw.writeBits(le, static_cast<int>(leb));
            // Distance codes 0-31 use an FIXED 5-bit code equal to the code's own value (not a
            // literal/length Huffman code, which is what fixed_huff() would otherwise produce).
            uint32_t dc, de, deb;
            dist_code(static_cast<uint32_t>(best_dist), dc, de, deb);
            bw.writeCode(dc, 5);
            bw.writeBits(de, static_cast<int>(deb));
            for (int k = 0; k < best_len; ++k) {
                const uint32_t hh = hash3(src, pos + static_cast<size_t>(k), src_len);
                prev[pos + static_cast<size_t>(k)] = head[hh];
                head[hh] = static_cast<int>(pos + static_cast<size_t>(k));
            }
            pos += static_cast<size_t>(best_len);
        } else {
            uint32_t clen;
            bw.writeCode(fixed_huff(src[pos], clen), static_cast<int>(clen));
            prev[pos] = head[h];
            head[h] = static_cast<int>(pos);
            ++pos;
        }
    }

    uint32_t ebits;
    bw.writeCode(fixed_huff(256, ebits), static_cast<int>(ebits)); // end of block
    bw.flush();
    out.swap(bw.buf);
    return true;
}

static bool screenshot_encode_png(const uint8_t* rgb, uint32_t w, uint32_t h, std::vector<uint8_t>& out) {
    // Each scanline is prefixed by a filter byte (0 = None) for PNG filtering.
    const size_t raw_len = static_cast<size_t>(h) * (1 + static_cast<size_t>(w) * 3);
    std::vector<uint8_t> raw;
    raw.reserve(raw_len);
    for (uint32_t y = 0; y < h; ++y) {
        raw.push_back(0);
        raw.insert(raw.end(), rgb + y * w * 3, rgb + (y + 1) * w * 3);
    }

    // IDAT payload: a zlib stream (zlib header + deflate + adler32). Compress the filtered scanlines
    // with the self-contained deflate compressor; on the (rare) failure path, fall back to stored
    // (uncompressed) deflate blocks so the PNG is always valid.
    std::vector<uint8_t> zlib;
    zlib.reserve(raw_len + raw_len / 65535 * 5 + 8);

    std::vector<uint8_t> raw_deflate;
    const bool compressed = screenshot_deflate(raw.data(), raw.size(), raw_deflate);

    if (compressed && !raw_deflate.empty()) {
        zlib.push_back(0x78); // CMF: deflate, 32K window
        zlib.push_back(0x01); // FLG: fastest, valid check value
        zlib.insert(zlib.end(), raw_deflate.begin(), raw_deflate.end());
        const uint32_t adler = screenshot_adler32(raw.data(), raw.size());
        zlib.push_back(static_cast<uint8_t>((adler >> 24) & 0xFF));
        zlib.push_back(static_cast<uint8_t>((adler >> 16) & 0xFF));
        zlib.push_back(static_cast<uint8_t>((adler >> 8) & 0xFF));
        zlib.push_back(static_cast<uint8_t>(adler & 0xFF));
    } else {
        zlib.push_back(0x78); // CMF
        zlib.push_back(0x01); // FLG
        size_t pos = 0;
        while (pos < raw.size()) {
            const size_t chunk = (raw.size() - pos > 65535) ? 65535 : (raw.size() - pos);
            const bool last = (pos + chunk == raw.size());
            zlib.push_back(last ? 0x01 : 0x00); // BFINAL + BTYPE=00 => stored
            zlib.push_back(static_cast<uint8_t>(chunk & 0xFF));
            zlib.push_back(static_cast<uint8_t>((chunk >> 8) & 0xFF));
            const uint16_t nlen = static_cast<uint16_t>(~chunk & 0xFFFF);
            zlib.push_back(static_cast<uint8_t>(nlen & 0xFF));
            zlib.push_back(static_cast<uint8_t>((nlen >> 8) & 0xFF));
            zlib.insert(zlib.end(), raw.begin() + pos, raw.begin() + pos + chunk);
            pos += chunk;
        }
        const uint32_t adler = screenshot_adler32(raw.data(), raw.size());
        zlib.push_back(static_cast<uint8_t>((adler >> 24) & 0xFF));
        zlib.push_back(static_cast<uint8_t>((adler >> 16) & 0xFF));
        zlib.push_back(static_cast<uint8_t>((adler >> 8) & 0xFF));
        zlib.push_back(static_cast<uint8_t>(adler & 0xFF));
    }

    static const uint8_t png_sig[8] = { 0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A };
    out.insert(out.end(), png_sig, png_sig + 8);

    uint8_t ihdr[13];
    ihdr[0] = (w >> 24) & 0xFF; ihdr[1] = (w >> 16) & 0xFF; ihdr[2] = (w >> 8) & 0xFF; ihdr[3] = w & 0xFF;
    ihdr[4] = (h >> 24) & 0xFF; ihdr[5] = (h >> 16) & 0xFF; ihdr[6] = (h >> 8) & 0xFF; ihdr[7] = h & 0xFF;
    ihdr[8] = 8;  // bit depth
    ihdr[9] = 2;  // color type: truecolor RGB
    ihdr[10] = 0; // compression method
    ihdr[11] = 0; // filter method
    ihdr[12] = 0; // interlace
    screenshot_png_chunk(out, "IHDR", ihdr, 13);
    screenshot_png_chunk(out, "IDAT", zlib.data(), zlib.size());
    screenshot_png_chunk(out, "IEND", nullptr, 0);
    return true;
}

static bool screenshot_encode_bmp(const uint8_t* rgb, uint32_t w, uint32_t h, std::vector<uint8_t>& out) {
    const uint32_t row_bytes = w * 3;
    const uint32_t row_pad = (4 - (row_bytes % 4)) % 4;
    const uint32_t pixel_offset = 54;
    const uint32_t file_size = pixel_offset + (row_bytes + row_pad) * h;
    out.assign(file_size, 0);

    auto write_u16 = [&out](uint32_t off, uint16_t val) { out[off] = val & 0xFF; out[off + 1] = (val >> 8) & 0xFF; };
    auto write_u32 = [&out](uint32_t off, uint32_t val) {
        out[off] = val & 0xFF; out[off + 1] = (val >> 8) & 0xFF; out[off + 2] = (val >> 16) & 0xFF; out[off + 3] = (val >> 24) & 0xFF;
    };

    out[0] = 'B'; out[1] = 'M';
    write_u32(2, file_size);
    write_u16(6, 0);
    write_u16(8, 0);
    write_u32(10, pixel_offset);
    write_u32(14, 40);
    write_u32(18, w);
    write_u32(22, h); // positive height -> bottom-up
    write_u16(26, 1); // planes
    write_u16(28, 24); // bits per pixel
    write_u32(30, 0); // compression
    write_u32(34, row_bytes * h);
    write_u32(38, 0);
    write_u32(42, 0);
    write_u32(46, 0);
    write_u32(50, 0);

    uint32_t dst = pixel_offset;
    for (uint32_t y = h; y > 0; --y) {
        const uint8_t* row = rgb + (y - 1) * w * 3;
        for (uint32_t x = 0; x < w; ++x) {
            out[dst++] = row[x * 3 + 2]; // blue
            out[dst++] = row[x * 3 + 1]; // green
            out[dst++] = row[x * 3 + 0]; // red
        }
        dst += row_pad;
    }
    return true;
}

esp_err_t WebServerService::handleApiScreenshot(httpd_req_t* request) {
    LOG_I(TAG, "GET /api/screenshot");

#if TT_FEATURE_SCREENSHOT_API
    std::string format = "png";
    std::string format_param;
    if (getQueryParam(request, "format", format_param) && !format_param.empty()) {
        format = format_param;
    }

    uint32_t w = 0;
    uint32_t h = 0;
    std::vector<uint8_t> rgb; // R,G,B bytes, row-major, top-down

    if (lvgl_try_lock(pdMS_TO_TICKS(100))) {
        lv_draw_buf_t* draw_buf = lv_snapshot_take(lv_scr_act(), LV_COLOR_FORMAT_RGB565);
        if (draw_buf == nullptr) {
            lvgl_unlock();
            httpd_resp_send_err(request, HTTPD_500_INTERNAL_SERVER_ERROR, "screenshot capture failed");
            return ESP_FAIL;
        }

        w = draw_buf->header.w;
        h = draw_buf->header.h;
        const uint32_t stride = draw_buf->header.stride;
        const uint8_t* data = draw_buf->data;

        rgb.reserve(static_cast<size_t>(w) * h * 3);
        for (uint32_t y = 0; y < h; ++y) {
            const uint8_t* row = data + y * stride;
            for (uint32_t x = 0; x < w; ++x) {
                const uint16_t px = static_cast<uint16_t>(row[x * 2] | (row[x * 2 + 1] << 8));
                const uint8_t r5 = (px >> 11) & 0x1F;
                const uint8_t g6 = (px >> 5) & 0x3F;
                const uint8_t b5 = px & 0x1F;
                // The ST7796 panel senses BGR while LVGL renders RGB565, so the panel shows every
                // pixel with red and blue swapped. Swap R/B here so the screenshot matches the
                // colors the panel actually displays (blue -> yellow-orange, dark stays dark).
                rgb.push_back(static_cast<uint8_t>((b5 << 3) | (b5 >> 2))); // red
                rgb.push_back(static_cast<uint8_t>((g6 << 2) | (g6 >> 4))); // green
                rgb.push_back(static_cast<uint8_t>((r5 << 3) | (r5 >> 2))); // blue
            }
        }

        lv_draw_buf_destroy(draw_buf);
        lvgl_unlock();
    } else {
        LOG_E(TAG, "Could not acquire LVGL lock within 100ms");
        httpd_resp_send_err(request, HTTPD_500_INTERNAL_SERVER_ERROR, "could not acquire LVGL lock");
        return ESP_FAIL;
    }

    std::vector<uint8_t> image;
    bool ok = false;
    if (format == "bmp") {
        ok = screenshot_encode_bmp(rgb.data(), w, h, image);
        httpd_resp_set_type(request, "image/bmp");
    } else {
        ok = screenshot_encode_png(rgb.data(), w, h, image);
        httpd_resp_set_type(request, "image/png");
    }
    if (!ok) {
        httpd_resp_send_err(request, HTTPD_500_INTERNAL_SERVER_ERROR, "image encoding failed");
        return ESP_FAIL;
    }

    httpd_resp_send_chunk(request, reinterpret_cast<const char*>(image.data()), image.size());
    httpd_resp_send_chunk(request, nullptr, 0);
    LOG_I(TAG, "[200] /api/screenshot as %s (%u bytes)", format.c_str(), static_cast<unsigned>(image.size()));
    return ESP_OK;
#else
    httpd_resp_send_err(request, HTTPD_501_METHOD_NOT_IMPLEMENTED, "screenshot feature not enabled");
    return ESP_FAIL;
#endif
}

// POST /api/video/start?fps=N - Start recording the active screen
esp_err_t WebServerService::handleApiVideoStart(httpd_req_t* request) {
    LOG_I(TAG, "POST /api/video/start");

    uint32_t fps = 5;
    std::string fps_param;
    if (getQueryParam(request, "fps", fps_param) && !fps_param.empty()) {
        fps = static_cast<uint32_t>(std::atoi(fps_param.c_str()));
        if (fps == 0 || fps > 30) {
            fps = 5;
        }
    }

    // Default to uncompressed RGB so Windows Media Player can decode it. 'mjpeg' opt-in for small files.
    uint32_t codec = TT_VIDEO_CODEC_RGB;
    std::string codec_param;
    if (getQueryParam(request, "codec", codec_param) && !codec_param.empty()) {
        if (codec_param == "mjpeg") {
            codec = TT_VIDEO_CODEC_MJPEG;
        }
    }

    if (tt_video_start(fps, codec)) {
        LOG_I(TAG, "video recording started (%u fps, codec=%u)", fps, codec);
        httpd_resp_sendstr(request, "ok");
        return ESP_OK;
    }

    httpd_resp_send_err(request, HTTPD_400_BAD_REQUEST, "already recording");
    return ESP_FAIL;
}

// POST /api/video/stop - Stop recording and return the Motion-JPEG AVI
esp_err_t WebServerService::handleApiVideoStop(httpd_req_t* request) {
    LOG_I(TAG, "POST /api/video/stop");

    std::vector<uint8_t> avi;
    uint32_t frames = 0;
    if (!tt_video_stop(&avi, &frames) || avi.empty()) {
        httpd_resp_send_err(request, HTTPD_500_INTERNAL_SERVER_ERROR, "video not available");
        return ESP_FAIL;
    }

    httpd_resp_set_type(request, "video/x-msvideo"); // .avi
    httpd_resp_send_chunk(request, reinterpret_cast<const char*>(avi.data()), avi.size());
    httpd_resp_send_chunk(request, nullptr, 0);
    LOG_I(TAG, "[200] /api/video/stop %u frames (%u bytes)", frames, static_cast<unsigned>(avi.size()));
    return ESP_OK;
}

esp_err_t WebServerService::handleFsTree(httpd_req_t* request) {

    LOG_I(TAG, "GET /fs/tree");

    std::ostringstream json;
    json << "{";
    // Gather mount points
    auto mounts = file::getFileSystemDirents();
    json << "\"mounts\": [";
    bool firstMount = true;
    for (auto& m : mounts) {
        if (!firstMount) json << ','; else firstMount = false;
        std::string name = m.d_name;
        std::string path = (name == std::string("data") || name == std::string("/data")) ? std::string("/data") : std::string("/") + name;
        // normalize possible duplicate slash
        if (!path.starts_with("/")) path = std::string("/") + path;
        json << "{\"name\":\"" << escapeJson(name) << "\",\"path\":\"" << escapeJson(path) << "\",\"entries\": [";

        std::vector<dirent> entries;
        int res = file::scandir(path, entries, file::direntFilterDotEntries, nullptr);
        if (res > 0) {
            bool first = true;
            for (auto& e : entries) {
                if (!first) json << ','; else first = false;
                std::string en = e.d_name;
                bool is_dir = (e.d_type == file::TT_DT_DIR || e.d_type == file::TT_DT_CHR);
                json << "{\"name\":\"" << escapeJson(en) << "\",\"type\":\"" << (is_dir?"dir":"file") << "\"}";
            }
        }

        json << "]}";
    }
    json << "]}";

    httpd_resp_set_type(request, "application/json");
    httpd_resp_sendstr(request, json.str().c_str());
    return ESP_OK;
}

// Create a directory at the specified path (POST /fs/mkdir?path=/data/newdir)
esp_err_t WebServerService::handleFsMkdir(httpd_req_t* request) {
    std::string path;
    if (!getQueryParam(request, "path", path) || path.empty()) {
        httpd_resp_send_err(request, HTTPD_400_BAD_REQUEST, "path required");
        return ESP_FAIL;
    }
    std::string norm = normalizePath(path);
    LOG_I(TAG, "POST /fs/mkdir requested: '%s' normalized: '%s'", path.c_str(), norm.c_str());
    if (!isAllowedBasePath(norm)) {
        httpd_resp_send_err(request, HTTPD_403_FORBIDDEN, "invalid path");
        return ESP_FAIL;
    }
    bool ok = file::findOrCreateDirectory(norm, 0755);
    if (!ok) { httpd_resp_send_err(request, HTTPD_500_INTERNAL_SERVER_ERROR, "mkdir failed"); return ESP_FAIL; }
    httpd_resp_sendstr(request, "ok");
    return ESP_OK;
}

static bool isRootMountPoint(const std::string& path) {
    return path == "/data" || path == "/sdcard";
}

// Delete a file or directory (POST /fs/delete?path=/data/foo)
esp_err_t WebServerService::handleFsDelete(httpd_req_t* request) {
    std::string path;
    if (!getQueryParam(request, "path", path) || path.empty()) {
        httpd_resp_send_err(request, HTTPD_400_BAD_REQUEST, "path required");
        return ESP_FAIL;
    }
    std::string norm = normalizePath(path);
    LOG_I(TAG, "POST /fs/delete requested: '%s' normalized: '%s'", path.c_str(), norm.c_str());
    if (!isAllowedBasePath(norm)) {
        httpd_resp_send_err(request, HTTPD_403_FORBIDDEN, "invalid path");
        return ESP_FAIL;
    }
    if (isRootMountPoint(norm)) {
        httpd_resp_send_err(request, HTTPD_403_FORBIDDEN, "cannot delete mount point");
        return ESP_FAIL;
    }
    bool ok = true;
    if (file::isDirectory(norm)) ok = file::deleteRecursively(norm);
    else if (file::isFile(norm)) ok = file::deleteFile(norm);
    else ok = false;
    if (!ok) { httpd_resp_send_err(request, HTTPD_500_INTERNAL_SERVER_ERROR, "delete failed"); return ESP_FAIL; }
    httpd_resp_sendstr(request, "ok");
    return ESP_OK;
}

// Rename a file or folder (POST /fs/rename?path=/data/oldname&newName=newname)
esp_err_t WebServerService::handleFsRename(httpd_req_t* request) {
    std::string path;
    std::string newName;
    if (!getQueryParam(request, "path", path) || path.empty()) {
        httpd_resp_send_err(request, HTTPD_400_BAD_REQUEST, "path required");
        return ESP_FAIL;
    }
    if (!getQueryParam(request, "newName", newName) || newName.empty()) {
        httpd_resp_send_err(request, HTTPD_400_BAD_REQUEST, "newName required");
        return ESP_FAIL;
    }
    std::string norm = normalizePath(path);
    LOG_I(TAG, "POST /fs/rename requested: '%s' normalized: '%s' -> newName: '%s'", path.c_str(), norm.c_str(), newName.c_str());
    if (!isAllowedBasePath(norm)) {
        httpd_resp_send_err(request, HTTPD_403_FORBIDDEN, "invalid path");
        return ESP_FAIL;
    }

    // Basic validation of newName: must not contain path separators or '..'
    // Trim whitespace from newName
    auto trim = [](std::string& s){ size_t st=0; while (st<s.size() && isspace((unsigned char)s[st])) ++st; size_t ed=s.size(); while (ed>st && isspace((unsigned char)s[ed-1])) --ed; s = s.substr(st, ed-st); };
    trim(newName);
    if (newName.empty() || newName.find('/') != std::string::npos || newName.find('\\') != std::string::npos || newName.find("..") != std::string::npos) {
        httpd_resp_send_err(request, HTTPD_400_BAD_REQUEST, "invalid newName");
        return ESP_FAIL;
    }

    // compute parent directory
    std::string parent = "/";
    size_t pos = norm.find_last_of('/');
    if (pos != std::string::npos) {
        parent = (pos == 0) ? std::string("/") : norm.substr(0, pos);
    }

    if (!isAllowedBasePath(parent)) {
        httpd_resp_send_err(request, HTTPD_403_FORBIDDEN, "invalid target parent");
        return ESP_FAIL;
    }

    std::string target = file::getChildPath(parent, newName);

    // Prevent overwrite: fail if target exists
    if (file::isFile(target) || file::isDirectory(target)) {
        httpd_resp_send_err(request, HTTPD_400_BAD_REQUEST, "target exists");
        return ESP_FAIL;
    }

    // perform rename
    int r = rename(norm.c_str(), target.c_str());
    if (r != 0) {
        int e = errno;
        LOG_W(TAG, "rename failed errno=%d (%s) -> %s -> %s", e, strerror(e), norm.c_str(), target.c_str());
        // Return errno string to client to aid debugging
        std::string msg = std::string("rename failed: ") + strerror(e);
        httpd_resp_send_err(request, HTTPD_500_INTERNAL_SERVER_ERROR, msg.c_str());
        return ESP_FAIL;
    }
    httpd_resp_sendstr(request, "ok");
    return ESP_OK;
}

// endregion

esp_err_t WebServerService::handleReboot(httpd_req_t* request) {
    
    LOG_I(TAG, "POST /reboot");
    httpd_resp_sendstr(request, "Rebooting...");
    
    // Reboot after a short delay to allow response to be sent
    vTaskDelay(pdMS_TO_TICKS(2000));
    esp_restart();

    return ESP_OK; // Unreachable, but satisfies function signature
}

esp_err_t WebServerService::handleAssets(httpd_req_t* request) {
    // Auth check for UI access control
    bool authPassed = false;
    esp_err_t authResult = validateRequestAuth(request, authPassed);
    if (!authPassed) {
        return authResult;
    }

    const char* uri = request->uri;
    LOG_I(TAG, "GET %s", uri);

    // Special case: serve favicon from system assets
    if (strcmp(uri, "/favicon.ico") == 0) {
        const char* faviconPath = "/system/spinner.png";
        if (file::isFile(faviconPath)) {
            httpd_resp_set_type(request, "image/png");
            httpd_resp_set_hdr(request, "Cache-Control", "public, max-age=86400");

            file::FileMutexGuard guard(faviconPath);

            FILE* fp = fopen(faviconPath, "rb");
            if (fp) {
                char buffer[512];
                size_t bytesRead;
                while ((bytesRead = fread(buffer, 1, sizeof(buffer), fp)) > 0) {
                    if (httpd_resp_send_chunk(request, buffer, bytesRead) != ESP_OK) {
                        fclose(fp);
                        return ESP_FAIL;
                    }
                }
                fclose(fp);
                httpd_resp_send_chunk(request, nullptr, 0);
                LOG_I(TAG, "[200] %s (favicon)", uri);
                return ESP_OK;
            }
        }
        // If favicon not found, return 404 silently (browsers handle this gracefully)
        httpd_resp_send_err(request, HTTPD_404_NOT_FOUND, "Not found");
        return ESP_FAIL;
    }

    // Special case: if requesting dashboard.html but it doesn't exist, serve default.html
    std::string requestedPath = uri;
    if (auto qpos = requestedPath.find('?'); qpos != std::string::npos) {
        requestedPath = requestedPath.substr(0, qpos);
    }
    requestedPath = normalizePath(requestedPath);
    if (requestedPath == "/.." || requestedPath.ends_with("/..") || requestedPath.find("/../") != std::string::npos) {
        httpd_resp_send_err(request, HTTPD_400_BAD_REQUEST, "invalid path");
        return ESP_FAIL;
    }

    std::string dataPath = std::string("/system/app/WebServer") + requestedPath;
    
    if (requestedPath == "/dashboard.html" && !file::isFile(dataPath.c_str())) {
        LOG_I(TAG, "dashboard.html not found, serving default.html");
    }
    
    // Try to serve from Data partition first
    if (file::isFile(dataPath.c_str())) {
        httpd_resp_set_type(request, getContentType(dataPath));
        
        // Read and send file using standard C FILE* operations
        file::FileMutexGuard guard(dataPath);

        FILE* fp = fopen(dataPath.c_str(), "rb");
        if (fp) {
            char buffer[512];
            size_t bytesRead;
            while ((bytesRead = fread(buffer, 1, sizeof(buffer), fp)) > 0) {
                if (httpd_resp_send_chunk(request, buffer, bytesRead) != ESP_OK) {
                    fclose(fp);
                    return ESP_FAIL;
                }
            }
            fclose(fp);

            httpd_resp_send_chunk(request, nullptr, 0);  // End of chunks
            LOG_I(TAG, "[200] %s (from Data)", uri);
            return ESP_OK;
        }
    }

    // Fallback to SD card
    std::string sdPath = std::string("/sdcard/tactility/webserver") + requestedPath;
    if (file::isFile(sdPath.c_str())) {
        httpd_resp_set_type(request, getContentType(sdPath));
        
        file::FileMutexGuard guard(sdPath);

        FILE* fp = fopen(sdPath.c_str(), "rb");
        if (fp) {
            char buffer[512];
            size_t bytesRead;
            while ((bytesRead = fread(buffer, 1, sizeof(buffer), fp)) > 0) {
                if (httpd_resp_send_chunk(request, buffer, bytesRead) != ESP_OK) {
                    fclose(fp);
                    return ESP_FAIL;
                }
            }
            fclose(fp);

            httpd_resp_send_chunk(request, nullptr, 0);  // End of chunks
            LOG_I(TAG, "[200] %s (from SD)", uri);
            return ESP_OK;
        }
    }
    
    // File not found
    LOG_W(TAG, "[404] %s", uri);
    httpd_resp_send_err(request, HTTPD_404_NOT_FOUND, "File not found");
    return ESP_FAIL;
}

extern const ServiceManifest manifest = {
    .id = "WebServer",
    .createService = create<WebServerService>
};

void setWebServerEnabled(bool enabled) {
    WebServerService* instance = ensureService();
    if (instance != nullptr) {
        instance->setEnabled(enabled);
        // Don't log here - startServer()/stopServer() already log the actual result
    } else {
        LOG_W(TAG, "WebServer service not available, cannot %s", enabled ? "start" : "stop");
    }
}

bool isWebServerEnabled() {
    WebServerService* instance = g_webServerInstance.load();
    return instance != nullptr && instance->isEnabled();
}

} // namespace

#endif // ESP_PLATFORM
