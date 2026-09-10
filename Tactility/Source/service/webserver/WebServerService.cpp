#ifdef ESP_PLATFORM

#include <Tactility/service/webserver/WebServerService.h>
#include <Tactility/service/webserver/RemoteInput.h>
#include <Tactility/service/ServiceManifest.h>

#include <app/start.h>
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
#include <Tactility/Timer.h>
#include <Tactility/service/wifi/Wifi.h>
#include <Tactility/service/ServiceRegistration.h>

#include <tactility/check.h>
#include <tactility/filesystem/file_system.h>
#include <tactility/log.h>
#include <tactility/time.h>

#include <lvgl/lvgl.h>
#include <lvgl/icons/statusbar.h>

#if TT_FEATURE_SCREENSHOT_ENABLED
#include <lv_screenshot.h>
#endif

#include "app/install.h"
#include "app/manager.h"


#include <atomic>
#include <cctype>
#include <cerrno>
#include <cstdio>
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
#include <memory>
#include <lwip/ip4_addr.h>
#include <lwip/sockets.h>
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

// ---- Remote screen interaction sessions ----------------------------------------------------------
//
// /ws/remote can't use validateRequestAuth(): httpd does not route the WebSocket upgrade request to
// the handler (verified - the handler only ever sees received frames), so there is no Authorization
// header to inspect, and WebSocket frames don't carry one either. The client therefore authenticates
// with its first message, "a<base64(username:password)>", and the connection is remembered by socket
// fd here.
//
// Only the httpd task runs the handler, so this table needs no lock.
//
// Remote interaction is refused outright while the web server has no authentication configured.
// Watching someone's screen and driving it is far more sensitive than the read-only endpoints, so it
// stays explicitly opt-in (enable "Require Authentication" in Settings > Web Server) rather than
// being open to anyone who can reach the port.
constexpr size_t REMOTE_SESSION_CAPACITY = 4;

struct RemoteSession {
    int fd = -1;
    // The client's address and ephemeral port. Together with the fd these identify the connection:
    // see remoteSessionForFd() for why the fd alone is not enough.
    uint32_t peer_addr = 0;
    uint16_t peer_port = 0;
    bool authenticated = false;
};

RemoteSession remoteSessions[REMOTE_SESSION_CAPACITY];
// The authenticated connection currently allowed to inject input. A newer authentication takes
// control over ("last authenticator wins"), and every other authenticated client is view-only.
int remoteControllerFd = -1;

// Turn Nagle off on a mirror socket.
//
// WebSocket replies are written as two separate sends - the 2-byte frame header, then the payload -
// and httpd_ws_send_frame_async() issues them through the session's raw send_fn. That is the one
// path in httpd that does not go through httpd_send()/httpd_send_all(), which is where httpd
// temporarily enables TCP_NODELAY around every write. So a WebSocket reply inherits whatever the
// socket's options are, and with Nagle left on the payload is held back until the client's delayed
// ACK for the header arrives.
//
// Measured on this device, one round trip while the WiFi low-latency window was armed:
//   ICMP                          5-9 ms   (not TCP at all)
//   HTTP GET, same instant        16.8 ms  (httpd_send_all toggles TCP_NODELAY)
//   WebSocket ping -> pong        62 ms    (raw send_fn, Nagle applies)
//   WebSocket whole-frame JPEG    34 ms of overhead over the device's own 193 ms
// The last line is the tell: a large payload is not delayed, because LWIP still pushes full MSS
// segments while Nagle holds only the trailing short one. Only the small replies suffered - and
// "same", the reply the mirror sends most often, is the smallest of them.
static void remoteSetNoDelay(int fd) {
    const int enabled = 1;
    if (setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &enabled, sizeof(enabled)) < 0) {
        LOG_W(TAG, "/ws/remote: could not disable Nagle on fd %d (errno %d)", fd, errno);
    }
}

/** Frees the entry holding `fd`, if any, and drops it as the input controller. */
void remoteDropSession(int fd) {
    for (auto& session : remoteSessions) {
        if (session.fd == fd) {
            session.fd = -1;
            session.peer_addr = 0;
            session.peer_port = 0;
            session.authenticated = false;
            break;
        }
    }
    if (remoteControllerFd == fd) {
        remoteControllerFd = -1;
    }
}

/** Reads the socket's peer address. False when the connection is already gone. */
bool remotePeerIdentity(int fd, uint32_t& addr, uint16_t& port) {
    struct sockaddr_in peer = {};
    socklen_t length = sizeof(peer);
    if (getpeername(fd, reinterpret_cast<struct sockaddr*>(&peer), &length) != 0) {
        return false;
    }
    addr = peer.sin_addr.s_addr;
    port = peer.sin_port;
    return true;
}

RemoteSession* remoteSessionForFd(int fd) {
    // Applied on every message rather than once per connection, idempotently. One small syscall per
    // frame is nothing next to producing the frame, and doing it here means it cannot be missed by
    // any of the paths below.
    remoteSetNoDelay(fd);

    uint32_t peer_addr = 0;
    uint16_t peer_port = 0;
    const bool have_peer = remotePeerIdentity(fd, peer_addr, peer_port);

    // A socket fd is not a connection. The stack hands out the lowest free number, so a client that
    // reconnects - a page refresh, a tab reopened - very often gets the fd the previous client just
    // released. Looking a session up by fd alone therefore finds the *old* connection's entry and,
    // with it, its authenticated flag: the new client would be served without credentials and would
    // inherit control of the device. Verified before this fix existed: a second connection that sent
    // no credentials received a frame, where an unauthenticated one should have been told "auth".
    //
    // The peer address and port are what actually distinguish connections, so an entry whose peer
    // does not match the socket belongs to somebody who has gone.
    for (auto& session : remoteSessions) {
        if (session.fd != fd) {
            continue;
        }
        if (have_peer && session.peer_addr == peer_addr && session.peer_port == peer_port) {
            return &session;
        }
        LOG_I(TAG, "/ws/remote: fd %d belongs to a new connection, dropping the old session", fd);
        remoteDropSession(fd);
        break;
    }

    for (auto& session : remoteSessions) {
        if (session.fd == -1) {
            session.fd = fd;
            session.peer_addr = peer_addr;
            session.peer_port = peer_port;
            session.authenticated = false;
            return &session;
        }
    }
    return nullptr;  // Table full: further connections stay unauthenticated.
}

// --- Mirror latency: WiFi power save ------------------------------------------------------------
//
// Power save parks the radio between access point beacons, so a client only gets heard once per
// beacon interval. Measured on the mirror: a reply that did no frame work at all ("same") still
// took ~103 ms, matching the 102.4 ms beacon interval this AP logs at boot - a latency floor well
// above what producing a frame costs, and paid on every round trip.
//
// The station therefore stays awake while a client is actually asking for frames. There is no
// "viewer went away" callback to hang this on (a closed socket does not reliably reach the
// handler), so the window is kept alive by the frame requests themselves and lapses on its own a
// few seconds after the last one - which also covers a client that just backgrounds the page.
// Re-enabling is idempotent, so a lapsed window that is then re-requested is harmless.
constexpr uint32_t MIRROR_LOW_LATENCY_TIMEOUT_MS = 4000;
bool mirrorLowLatencyActive = false;
std::unique_ptr<Timer> mirrorLowLatencyTimer;

void mirrorEndLowLatency() {
    if (!mirrorLowLatencyActive) {
        return;
    }
    mirrorLowLatencyActive = false;
    tt::service::wifi::setPowerSaveEnabled(true);
    LOG_I(TAG, "/ws/remote: no frames for %u ms, WiFi power save restored",
        (unsigned)MIRROR_LOW_LATENCY_TIMEOUT_MS);
}

/** Marks a frame request: keeps WiFi awake and restarts the idle window. */
void mirrorRequestLowLatency() {
    if (mirrorLowLatencyTimer == nullptr) {
        // Created lazily: most devices never open a mirror, and a timer costs a daemon slot.
        mirrorLowLatencyTimer = std::make_unique<Timer>(
            Timer::Type::Once,
            millis_to_ticks(MIRROR_LOW_LATENCY_TIMEOUT_MS),
            mirrorEndLowLatency
        );
    }

    if (!mirrorLowLatencyActive) {
        mirrorLowLatencyActive = true;
        tt::service::wifi::setPowerSaveEnabled(false);
        LOG_I(TAG, "/ws/remote: frame requested, WiFi power save disabled");
    }

    // reset() also (re)starts a timer that already fired, which is what extends the window.
    mirrorLowLatencyTimer->reset(millis_to_ticks(MIRROR_LOW_LATENCY_TIMEOUT_MS));
}

bool webServerAuthEnabled() {
    auto lock = g_settingsMutex.asScopedLock();
    lock.lock();
    return g_cachedSettings.webServerAuthEnabled;
}

// Validate a decoded username:password pair against the configured web server credentials.
bool validateCredentials(const std::string& username, const std::string& password) {
    settings::webserver::WebServerSettings settings;
    {
        auto lock = g_settingsMutex.asScopedLock();
        lock.lock();
        settings = g_cachedSettings;
    }
    return secureCompare(username, settings.webServerUsername) &&
        secureCompare(password, settings.webServerPassword);
}

// Decodes the "a<base64(username:password)>" payload. Returns false when it is malformed.
bool decodeRemoteCredentials(const uint8_t* payload, size_t len, std::string& username, std::string& password) {
    if (len < 2) {
        return false;
    }
    const std::string base64(reinterpret_cast<const char*>(payload + 1), len - 1);

    size_t decoded_len = 0;
    mbedtls_base64_decode(nullptr, 0, &decoded_len,
                          reinterpret_cast<const unsigned char*>(base64.c_str()),
                          base64.length());
    if (decoded_len == 0) {
        return false;
    }

    std::string decoded(decoded_len, '\0');
    size_t actual_len = 0;
    if (mbedtls_base64_decode(reinterpret_cast<unsigned char*>(decoded.data()), decoded_len, &actual_len,
                              reinterpret_cast<const unsigned char*>(base64.c_str()),
                              base64.length()) != 0) {
        return false;
    }
    decoded.resize(actual_len);

    const size_t colon = decoded.find(':');
    if (colon == std::string::npos) {
        return false;
    }
    username = decoded.substr(0, colon);
    password = decoded.substr(colon + 1);
    return true;
}

// Sends a short text reply on the WebSocket.
esp_err_t remoteReplyText(httpd_req_t* request, const char* text) {
    httpd_ws_frame_t reply = {};
    reply.type = HTTPD_WS_TYPE_TEXT;
    reply.payload = reinterpret_cast<uint8_t*>(const_cast<char*>(text));
    reply.len = std::strlen(text);
    const esp_err_t result = httpd_ws_send_frame(request, &reply);
    if (result != ESP_OK) {
        // A reply that could not be delivered means the client is gone: release its slot now
        // instead of leaving the entry to be reclaimed by whoever reuses the fd.
        remoteDropSession(httpd_req_to_sockfd(request));
    }
    return result;
}

bool WebServerService::onStart(ServiceContext& service) {
    LOG_I(TAG, "Starting WebServer service...");

    // Register global instance
    g_webServerInstance.store(this);

    // Create statusbar icon (hidden initially, shown when server actually starts)
    statusbarIconId = lvgl::statusbar_icon_add();
    lvgl::statusbar_icon_set_visibility(statusbarIconId, false);

    // Load the saved enable state so the Web Server stays enabled across reboots.
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
        // WebSocket for remote screen interaction. Registered before the "/*" catch-all below,
        // which would otherwise claim this URI as a static asset.
        {
            .uri          = "/ws/remote",
            .method       = HTTP_GET,
            .handler      = handleRemoteWebSocket,
            .user_ctx     = ctx,
            .is_websocket = true
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
#if TT_FEATURE_SCREENSHOT_ENABLED
    json << "\"screenshot\":true";
#else
    json << "\"screenshot\":false";
#endif
    json << "},";

    // Remote-screen capture cost, so the mirror's frame time can be read without a serial console
    // (a per-frame log backs the UART up and perturbs the very path being measured).
    {
        TtVideoGrabStats stats = {};
        tt_video_get_grab_stats(&stats);
        json << "\"mirror\":{"
             << "\"frames\":" << stats.frames << ","
             << "\"capture_ms\":" << stats.capture_ms << ","
             << "\"lock_wait_ms\":" << stats.lock_wait_ms << ","
             << "\"copy_ms\":" << stats.copy_ms << ","
             << "\"swap_ms\":" << stats.swap_ms << ","
             << "\"encode_ms\":" << stats.encode_ms << ","
             << "\"encode_open_ms\":" << stats.encode_open_ms << ","
             << "\"shadow_frame\":" << (stats.used_shadow_frame ? "true" : "false") << ","
             << "\"resolution_w\":" << stats.resolution_w << ","
             << "\"resolution_h\":" << stats.resolution_h << ","
             << "\"quality\":" << stats.quality << ","
             << "\"scale\":" << stats.scale << ","
             << "\"output_w\":" << stats.output_w << ","
             << "\"output_h\":" << stats.output_h << ","
             << "\"delta_frames\":" << stats.delta_frames << ","
             << "\"region_jpeg_frames\":" << stats.region_jpeg_frames << ","
             << "\"full_frames\":" << stats.full_frames << ","
             << "\"same_frames\":" << stats.same_frames
             << "}";
    }

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
    app_start(appId.c_str(), 0, nullptr, &instance_id);

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

// GET /api/screenshot - Capture and return screenshot as PNG
// Screenshots are saved to SD card root (if available) or /data with incrementing numbers
esp_err_t WebServerService::handleApiScreenshot(httpd_req_t* request) {
    LOG_I(TAG, "GET /api/screenshot");

#if TT_FEATURE_SCREENSHOT_ENABLED
    // Determine save location: prefer SD card root if mounted, otherwise /data
    std::string save_path = getDataPath();

    // Find next available filename with incrementing number
    std::string screenshot_path;
    bool found_slot = false;
    for (int i = 1; i <= 9999; ++i) {
        screenshot_path = std::format("{}/webscreenshot{}.png", save_path, i);
        if (!file::isFile(screenshot_path)) {
            found_slot = true;
            break;
        }
    }
    if (!found_slot) {
        httpd_resp_send_err(request, HTTPD_500_INTERNAL_SERVER_ERROR, "no available screenshot slots");
        return ESP_FAIL;
    }

    LOG_I(TAG, "Screenshot will be saved to: %s", screenshot_path.c_str());

    // LVGL's lodepng uses lv_fs which requires the "A:" prefix
    std::string lvgl_screenshot_path = lvgl::PATH_PREFIX + screenshot_path;

    // Capture screenshot using LVGL
    if (lvgl_try_lock(pdMS_TO_TICKS(100))) {
        bool success = lv_screenshot_create(lv_scr_act(), LV_100ASK_SCREENSHOT_SV_PNG, lvgl_screenshot_path.c_str());
        lvgl_unlock();

        if (!success) {
            LOG_E(TAG, "lv_screenshot_create failed for path: %s", lvgl_screenshot_path.c_str());
            httpd_resp_send_err(request, HTTPD_500_INTERNAL_SERVER_ERROR, "screenshot capture failed");
            return ESP_FAIL;
        }
        LOG_I(TAG, "Screenshot captured successfully");
    } else {
        LOG_E(TAG, "Could not acquire LVGL lock within 100ms");
        httpd_resp_send_err(request, HTTPD_500_INTERNAL_SERVER_ERROR, "could not acquire LVGL lock");
        return ESP_FAIL;
    }

    // Send the file (use regular path for fopen, not LVGL path)
    httpd_resp_set_type(request, "image/png");

    FILE* fp = fopen(screenshot_path.c_str(), "rb");
    if (!fp) {
        httpd_resp_send_err(request, HTTPD_500_INTERNAL_SERVER_ERROR, "failed to open screenshot");
        return ESP_FAIL;
    }

    char buf[512];
    size_t n;
    while ((n = fread(buf, 1, sizeof(buf), fp)) > 0) {
        if (httpd_resp_send_chunk(request, buf, n) != ESP_OK) {
            fclose(fp);
            return ESP_FAIL;
        }
    }
    fclose(fp);
    httpd_resp_send_chunk(request, nullptr, 0);

    // File is kept on storage (not deleted) for user access
    LOG_I(TAG, "[200] /api/screenshot -> %s", screenshot_path.c_str());
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

// GET /ws/remote - WebSocket endpoint for remote screen interaction.
//
// httpd calls this once for the upgrade request (method == HTTP_GET), then once per received data
// frame. Control frames (ping/pong/close) are handled inside httpd because the handler is
// registered without handle_ws_control_frames.
//
// The endpoint carries both directions: a client sends "f" and receives one JPEG back as a binary
// frame (viewing), and sends pointer/key events that get injected into LVGL (interaction).
esp_err_t WebServerService::handleRemoteWebSocket(httpd_req_t* request) {
    // Register the remote pointer indev here rather than only in the handshake branch below: this
    // httpd does not appear to invoke the handler for the upgrade request itself (only for received
    // frames), so the handshake branch never ran and the indev was never created. The call is
    // idempotent and cheap once the indev exists.
    remoteInputEnsureIndev();

    if (request->method == HTTP_GET) {
        // Handshake, when httpd does route it here: httpd performs the upgrade itself
        // (is_websocket = true), so there is nothing to send from this side.
        LOG_I(TAG, "GET /ws/remote (websocket handshake)");
        return ESP_OK;
    }

    // Cap the payload we're willing to buffer: a control/hello message is tiny, and an unbounded
    // frame would let a client exhaust heap.
    constexpr size_t MAX_REMOTE_WS_FRAME = 4096;

    // First call with len 0 only fills in the frame header (type + payload length).
    httpd_ws_frame_t frame = {};
    frame.type = HTTPD_WS_TYPE_TEXT;
    esp_err_t result = httpd_ws_recv_frame(request, &frame, 0);
    if (result != ESP_OK) {
        LOG_W(TAG, "/ws/remote: failed to read frame header");
        return result;
    }

    if (frame.len > MAX_REMOTE_WS_FRAME) {
        LOG_W(TAG, "/ws/remote: frame too large (%u bytes)", (unsigned)frame.len);
        return ESP_ERR_INVALID_SIZE;
    }

    // +1 so an empty frame still hands httpd a valid (non-null) buffer.
    std::vector<uint8_t> payload(frame.len + 1);
    frame.payload = payload.data();
    result = httpd_ws_recv_frame(request, &frame, frame.len);
    if (result != ESP_OK) {
        LOG_W(TAG, "/ws/remote: failed to read frame payload");
        return result;
    }

    LOG_I(TAG, "/ws/remote: received %u byte(s)", (unsigned)frame.len);

    const int fd = httpd_req_to_sockfd(request);
    RemoteSession* session = remoteSessionForFd(fd);

    // The upgrade request never reaches this handler (see the session notes above), so the first
    // message is what authenticates: "a<base64(username:password)>".
    if (frame.type == HTTPD_WS_TYPE_TEXT && frame.len >= 1 && payload[0] == 'a') {
        if (!webServerAuthEnabled()) {
            LOG_W(TAG, "/ws/remote: refusing interaction - web server authentication is disabled");
            return remoteReplyText(request, "disabled");
        }

        std::string username;
        std::string password;
        if (session != nullptr &&
            decodeRemoteCredentials(payload.data(), frame.len, username, password) &&
            validateCredentials(username, password)) {
            session->authenticated = true;
            remoteControllerFd = fd;  // Newest authenticated client takes control.
            LOG_I(TAG, "/ws/remote: client authenticated and now controls the device");
            return remoteReplyText(request, "ok");
        }

        LOG_W(TAG, "/ws/remote: authentication failed");
        return remoteReplyText(request, "auth");
    }

    // Everything else needs an authenticated session. Answering "auth" (rather than dropping the
    // frame) lets a client tell "not authenticated" apart from "no reply at all".
    if (session == nullptr || !session->authenticated) {
        return remoteReplyText(request, "auth");
    }

    // Pointers and keys are only honoured from the controlling client; other authenticated clients
    // stay view-only. Their input is dropped silently - it is not an error.
    const bool isController = (fd == remoteControllerFd);

    // Text commands. "f" asks for one screen frame: the client requests the next only after it has
    // displayed the previous one, so pacing (and back-pressure) live on the client side. "ping" is
    // a liveness check. Anything else falls through to the echo below.
    if (frame.type == HTTPD_WS_TYPE_TEXT) {
        // Remote pointer events: "<kind><x>,<y>" - "p12,34" press, "m13,35" move, "r13,35" release.
        // Fire-and-forget: the client sees the effect in the next frame it asks for.
        if (frame.len >= 3 && (payload[0] == 'p' || payload[0] == 'm' || payload[0] == 'r')) {
            if (!isController) {
                return ESP_OK;
            }
            int x = 0;
            int y = 0;
            if (std::sscanf(reinterpret_cast<const char*>(payload.data()) + 1, "%d,%d", &x, &y) == 2) {
                const RemoteInputType type = (payload[0] == 'p') ? RemoteInputType::Press
                    : ((payload[0] == 'm') ? RemoteInputType::Move : RemoteInputType::Release);
                remoteInputPush(type, x, y);
                return ESP_OK;
            }
        }

        // Remote key events: "k<code>" carrying an LVGL key code - printable ASCII directly, and
        // the LV_KEY_* values for the rest (8 backspace, 10 enter, 27 escape, 127 delete, and
        // 17..20 for the arrow keys).
        if (frame.len >= 2 && payload[0] == 'k') {
            if (!isController) {
                return ESP_OK;
            }
            int code = 0;
            if (std::sscanf(reinterpret_cast<const char*>(payload.data()) + 1, "%d", &code) == 1 && code > 0) {
                remoteInputPushKey(static_cast<uint32_t>(code));
                return ESP_OK;
            }
        }

        if (frame.len >= 1 && payload[0] == 'f') {
            // A client asking for frames is a mirror in use: keep the radio awake for it.
            mirrorRequestLowLatency();

            // "f" optionally carries the JPEG quality, a downscale factor and a "send a whole frame"
            // flag: "f", "f40", "f40,2", "f40,2,1". Zero means "use the default", so the client only
            // sends what it wants to override.
            int quality = 0;
            int scale = 0;
            int force_full = 0;
            if (frame.len > 1) {
                if (std::sscanf(reinterpret_cast<const char*>(payload.data()) + 1, "%d,%d,%d",
                                &quality, &scale, &force_full) < 1) {
                    quality = 0;
                    scale = 0;
                    force_full = 0;
                }
            }

            // Change-only frame first: most UI updates touch a small part of the screen, and sending
            // just that raw skips the full-frame copy and encode entirely. "same" means nothing moved
            // since the previous request. A client that asks for whole frames (a recorder, or a
            // measurement of the full-frame path) skips the diff and always gets a JPEG.
            const uint8_t* delta = nullptr;
            size_t delta_size = 0;
            uint32_t delta_x = 0;
            uint32_t delta_y = 0;
            uint32_t delta_w = 0;
            uint32_t delta_h = 0;
            const TtVideoFrameKind kind = (force_full != 0)
                ? TT_VIDEO_FRAME_FULL
                : tt_video_grab_delta(&delta, &delta_size, &delta_x, &delta_y, &delta_w, &delta_h);
            if (kind == TT_VIDEO_FRAME_NONE) {
                return remoteReplyText(request, "same");
            }
            if (kind == TT_VIDEO_FRAME_DELTA || kind == TT_VIDEO_FRAME_DELTA_JPEG) {
                // The text header tells the client what the following binary frame covers and how
                // it is encoded: "D" is raw RGB565, "J" is a JPEG of that region. A binary frame
                // with no preceding header is a whole-frame JPEG.
                const bool region_is_jpeg = (kind == TT_VIDEO_FRAME_DELTA_JPEG);
                char header[48];
                std::snprintf(header, sizeof(header), "%c %u %u %u %u",
                    region_is_jpeg ? 'J' : 'D',
                    (unsigned)delta_x, (unsigned)delta_y, (unsigned)delta_w, (unsigned)delta_h);
                const esp_err_t header_result = remoteReplyText(request, header);
                if (header_result != ESP_OK) {
                    return header_result;
                }
                httpd_ws_frame_t delta_reply = {};
                delta_reply.type = HTTPD_WS_TYPE_BINARY;
                delta_reply.payload = const_cast<uint8_t*>(delta);
                delta_reply.len = delta_size;
                return httpd_ws_send_frame(request, &delta_reply);
            }

            const uint8_t* jpeg = nullptr;
            size_t jpeg_size = 0;
            uint32_t width = 0;
            uint32_t height = 0;
            if (!tt_video_grab_jpeg(quality, scale, &jpeg, &jpeg_size, &width, &height)) {
                // Capture failed (commonly: the LVGL lock was busy). Answer so the client can retry
                // instead of blocking forever waiting for a frame that isn't coming.
                return remoteReplyText(request, "err");
            }

            httpd_ws_frame_t reply = {};
            reply.type = HTTPD_WS_TYPE_BINARY;
            reply.payload = const_cast<uint8_t*>(jpeg);
            reply.len = jpeg_size;
            const esp_err_t send_result = httpd_ws_send_frame(request, &reply);
            if (send_result != ESP_OK) {
                LOG_W(TAG, "/ws/remote: frame send failed (client disconnected?)");
            }
            return send_result;
        }

        if (frame.len == 4 && std::memcmp(payload.data(), "ping", 4) == 0) {
            return remoteReplyText(request, "pong");
        }
    }

    // Default: echo the frame back on the same connection.
    return httpd_ws_send_frame(request, &frame);
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
    .id = "tactility.webserver",
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
