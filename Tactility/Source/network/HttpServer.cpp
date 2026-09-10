#ifdef ESP_PLATFORM

#include <Tactility/network/HttpServer.h>

#include <Tactility/service/wifi/Wifi.h>

#include <tactility/log.h>

namespace tt::network {

constexpr auto* TAG = "HttpServer";

static constexpr size_t INTERNAL_URI_HANDLER_COUNT = 2;

bool HttpServer::startInternal() {
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.stack_size = stackSize;
    config.server_port = port;
    // Each HttpServer instance needs its own control socket port; HTTPD_DEFAULT_CONFIG()
    // hardcodes ctrl_port=32768, so a second server (e.g. Web Server on 80 alongside the
    // development service on 6666) would fail to bind its control socket.
    config.ctrl_port = (uint16_t)(port + 1);
    config.uri_match_fn = matchUri;
    config.max_uri_handlers = handlers.size() + INTERNAL_URI_HANDLER_COUNT;
    // HTTPD_DEFAULT_CONFIG() sets max_open_sockets to 7. This was lowered to 3 for low-memory
    // targets, but a long-lived connection (e.g. the /ws/remote WebSocket used for remote screen
    // interaction) holds one socket for its whole session, and a browser keeps keep-alive sockets
    // open alongside it - 3 left too little room for concurrent API requests. 6 fits comfortably
    // within CONFIG_LWIP_MAX_SOCKETS (12), including the second server on the development port.
    config.max_open_sockets = 6;

    if (httpd_start(&server, &config) != ESP_OK) {
        LOG_E(TAG, "Failed to start http server on port %u", (unsigned)port);
        return false;
    }

    bool allRegistered = true;
    for (std::vector<httpd_uri_t>::reference handler : handlers) {
        if (httpd_register_uri_handler(server, &handler) != ESP_OK) {
            LOG_E(TAG, "Failed to register URI handler: %s", handler.uri);
            allRegistered = false;
        }
    }
    if (!allRegistered) {
        httpd_stop(server);
        server = nullptr;
        return false;
    }

    LOG_I(TAG, "Started on port %u", (unsigned)config.server_port);
    return true;
}

void HttpServer::stopInternal() {
    LOG_I(TAG, "Stopping server");
    if (server != nullptr) {
        if (httpd_stop(server) == ESP_OK) {
            server = nullptr;
        } else {
            LOG_W(TAG, "Error while stopping");
        }
    }
}

bool HttpServer::start() {
    auto lock = mutex.asScopedLock();
    lock.lock();

    if (isStarted()) {
        LOG_W(TAG, "Already started");
        return true;
    }

    return startInternal();
}

void HttpServer::stop() {
    auto lock = mutex.asScopedLock();
    lock.lock();

    if (!isStarted()) {
        LOG_W(TAG, "Not started");
        return;
    }

    stopInternal();
}

}

#endif