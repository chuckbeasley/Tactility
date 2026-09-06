#ifdef ESP_PLATFORM
#include <sdkconfig.h>
#endif

#include <Tactility/DeprecatedPaths.h>
#include <Tactility/Tactility.h>
#include <Tactility/Mutex.h>
#include <Tactility/Timer.h>
#include <Tactility/service/wifi/Wifi.h>

#include "PcapWriter.h"

#include <app/event.h>
#include <app/manager.h>
#include <app/manifest.h>

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
#include <cstring>
#include <format>
#include <memory>
#include <string>

#if defined(CONFIG_SOC_WIFI_SUPPORTED)
#include <esp_heap_caps.h>
#include <esp_timer.h>
#include <freertos/stream_buffer.h>
#endif

namespace tt::app::wifimonitor {

constexpr auto* TAG = "WifiMonitor";

extern const ::AppManifest manifest;

namespace {

constexpr uint32_t POLL_INTERVAL_MS = 200;
constexpr size_t STREAM_BUFFER_SIZE = 1024 * 1024;
constexpr size_t MAX_FRAME_SIZE = 2346; // 802.11 max MPDU size (sig_len includes FCS)

// Header prepended to each frame in the stream buffer.
struct CaptureRecord {
    uint32_t length;   // frame payload length
    uint32_t ts_sec;   // capture time, seconds
    uint32_t ts_usec;  // capture time, microseconds
    int8_t rssi;
    uint8_t channel;
    uint8_t type;      // WifiPromiscuousPacketType
    uint8_t reserved;
};

struct Context {
    uint32_t appInstanceId = 0;
    // Written by the LVGL task, read by the timer/writer tasks - benign one-tick race.
    bool capturing = false;
    bool active = false;
    std::unique_ptr<Timer> pollTimer;
#if defined(CONFIG_SOC_WIFI_SUPPORTED)
    StreamBufferHandle_t streamBuffer = nullptr;
    uint8_t* streamBufferStorage = nullptr; // PSRAM backing for the stream buffer
    ::Thread* writerThread = nullptr;
    PcapWriter writer;
    std::atomic<uint32_t> packetCount{0};
    std::atomic<uint32_t> droppedCount{0};
    std::atomic<bool> writerStop{false}; // signals the writer to drain and exit
    std::atomic<uint8_t> currentChannel{0}; // live readout of the radio's channel
    uint8_t targetMac[6] = {0};  // optional MAC filter (all zeros = capture everything)
    bool macFilterEnabled = false; // set when targetMac holds a valid address
    uint8_t apBssid[6] = {0};    // AP BSSID, auto-detected from captured frames
    bool apBssidKnown = false;
    Device* wifiDevice = nullptr;
    uint8_t channelIndex = 0;
    uint8_t hopTick = 0;
    uint8_t lockChannel = 0; // 0 = auto/hop, otherwise lock to this 802.11 channel
    bool wifiAutoConnectPaused = false; // set when we paused WiFi auto-connect for capture
    Mutex captureMutex; // serializes start/stop state transitions
#endif
    lv_obj_t* statusLabel = nullptr;
    lv_obj_t* statsLabel = nullptr;
    lv_obj_t* startButtonLabel = nullptr;
};

void onBackPressed(lv_event_t* event) {
    auto* ctx = static_cast<Context*>(lv_event_get_user_data(event));
    app_event_emit_close(ctx->appInstanceId);
}

void updateStatusLabel(Context* ctx) {
    if (ctx->statusLabel == nullptr) return;
#if defined(CONFIG_SOC_WIFI_SUPPORTED)
    const char* text = ctx->active ? "Capturing" : (ctx->capturing ? "Starting..." : "Stopped");
#else
    const char* text = "Wi-Fi unavailable";
#endif
    lv_label_set_text(ctx->statusLabel, text);
}

void updateStartButtonLabel(Context* ctx) {
    if (ctx->startButtonLabel != nullptr) {
        lv_label_set_text(ctx->startButtonLabel, ctx->capturing ? "Stop" : "Start");
    }
}

void updateStatsLabel(Context* ctx) {
    if (ctx->statsLabel == nullptr) return;
#if defined(CONFIG_SOC_WIFI_SUPPORTED)
    auto text = std::format("Packets: {}   Dropped: {}   {} KB   Ch: {}",
        ctx->packetCount.load(), ctx->droppedCount.load(),
        (unsigned)(ctx->writer.getBytesWritten() / 1024),
        (unsigned)ctx->currentChannel.load());
    lv_label_set_text(ctx->statsLabel, text.c_str());
#else
    lv_label_set_text(ctx->statsLabel, "");
#endif
}

#if defined(CONFIG_SOC_WIFI_SUPPORTED)

// Static scratch buffer for the promiscuous callback, which runs on the single
// ESP-IDF Wi-Fi task - so a file-scope buffer is safe and avoids stack/alloc issues.
static uint8_t s_scratch[sizeof(CaptureRecord) + MAX_FRAME_SIZE];

// Channel hopping sequence (2.4 GHz + 5 GHz U-NII bands).
static constexpr uint8_t kChannels[] = {
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
    36, 40, 44, 48, 52, 56, 60, 64,
    100, 104, 108, 112, 116, 120, 124, 128, 132, 136, 140, 144,
    149, 153, 157, 161, 165
};
constexpr size_t kChannelCount = sizeof(kChannels) / sizeof(kChannels[0]);

// Static control block for the stream buffer (the data buffer itself is PSRAM).
static StaticStreamBuffer_t s_streamBufferStruct;

void hopChannel(Context* ctx) {
    uint8_t channel = kChannels[ctx->channelIndex % kChannelCount];
    ctx->channelIndex++;
    ctx->currentChannel.store(channel);
    Device* dev = ctx->wifiDevice;
    if (dev != nullptr) {
        wifi_set_channel(dev, channel);
    }
}

void onChannelSelected(lv_event_t* event) {
    auto* dropdown = static_cast<lv_obj_t*>(lv_event_get_target(event));
    auto* ctx = static_cast<Context*>(lv_event_get_user_data(event));
    uint32_t index = lv_dropdown_get_selected(dropdown);
    ctx->lockChannel = (index == 0) ? 0 : kChannels[index - 1];
    if (ctx->lockChannel != 0) {
        ctx->currentChannel.store(ctx->lockChannel);
    }

    // If already capturing, switch to the newly selected channel immediately.
    if (ctx->active && ctx->lockChannel != 0 && ctx->wifiDevice != nullptr) {
        wifi_set_channel(ctx->wifiDevice, ctx->lockChannel);
    }
}

// Parse "aa:bb:cc:dd:ee:ff" into 6 bytes. False on invalid or empty input.
static bool parseMacAddress(const char* text, uint8_t out[6]) {
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

void onMacChanged(lv_event_t* event) {
    auto* textarea = static_cast<lv_obj_t*>(lv_event_get_target(event));
    auto* ctx = static_cast<Context*>(lv_event_get_user_data(event));
    const char* mac_text = lv_textarea_get_text(textarea);
    uint8_t mac[6];
    if (parseMacAddress(mac_text, mac)) {
        std::memcpy(ctx->targetMac, mac, 6);
        ctx->macFilterEnabled = true;
        LOG_I(TAG, "MAC filter set to %02x:%02x:%02x:%02x:%02x:%02x",
            mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    } else {
        ctx->macFilterEnabled = false; // blank/invalid -> capture everything
        LOG_I(TAG, "MAC filter cleared");
    }
}

void onPacket(void* context, const uint8_t* payload, size_t length, WifiPromiscuousPacketInfo info) {
    auto* ctx = static_cast<Context*>(context);
    // Reflect the channel this frame was actually received on, so the UI readout
    // shows where the radio really is (not just where we asked it to hop).
    ctx->currentChannel.store(info.channel);
    // Track the AP BSSID from each frame (used for deauth injection).
    if (length >= 22) {
        bool to_ds = (payload[1] & 0x01) != 0;
        bool from_ds = (payload[1] & 0x02) != 0;
        const uint8_t* bssid = (to_ds && !from_ds) ? (payload + 4) : (payload + 16);
        std::memcpy(ctx->apBssid, bssid, 6);
        ctx->apBssidKnown = true;
    }
    // Data frames only: skip management/control/misc frames so the capture and
    // stream buffer are dedicated to the traffic the user is hunting.
    if (info.type != WIFI_PROMISCUOUS_PACKET_TYPE_DATA) {
        return;
    }

    // Optional MAC filter: only capture frames that involve the target address
    // (source/destination). Blank = capture everything.
    if (ctx->macFilterEnabled && length >= 22) {
        bool matched = false;
        // The 802.11 header's Addr1/Addr2/Addr3 sit at payload offsets 4/10/16.
        for (size_t off = 4; off + 6 <= length && off <= 16; off += 6) {
            if (std::memcmp(payload + off, ctx->targetMac, 6) == 0) {
                matched = true;
                break;
            }
        }
        if (!matched) {
            return; // filtered out on purpose, not counted as a drop
        }
    }

    if (length == 0 || length > MAX_FRAME_SIZE) {
        ctx->droppedCount.fetch_add(1);
        return;
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
    memcpy(s_scratch + sizeof(CaptureRecord), payload, length);

    size_t total = sizeof(CaptureRecord) + length;
    if (xStreamBufferSpacesAvailable(ctx->streamBuffer) >= total) {
        xStreamBufferSend(ctx->streamBuffer, s_scratch, total, 0);
        ctx->packetCount.fetch_add(1);
    } else {
        ctx->droppedCount.fetch_add(1);
    }
}

int32_t captureWriterMain(void* context) {
    auto* ctx = static_cast<Context*>(context);

    // Timestamped filename so consecutive captures don't overwrite each other.
    std::string path = std::format("{}/wifi-monitor-{}.pcap", getUserDataPath(), (long long)esp_timer_get_time());
    if (!ctx->writer.open(path.c_str())) {
        LOG_E(TAG, "Failed to open capture file %s", path.c_str());
        return 1;
    }
    LOG_I(TAG, "Capturing to %s", path.c_str());

    // Drain while capturing; after a stop request keep draining whatever is left
    // in the stream buffer so no captured frames are lost, then exit and close.
    while (!ctx->writerStop.load() || xStreamBufferBytesAvailable(ctx->streamBuffer) > 0) {
        CaptureRecord record;
        size_t got = xStreamBufferReceive(ctx->streamBuffer, &record, sizeof(record), pdMS_TO_TICKS(100));
        if (got != sizeof(record)) {
            continue; // timeout waiting for a complete header
        }

        uint8_t payload[MAX_FRAME_SIZE];
        size_t plen = record.length > MAX_FRAME_SIZE ? MAX_FRAME_SIZE : record.length;
        size_t pgot = xStreamBufferReceive(ctx->streamBuffer, payload, plen, pdMS_TO_TICKS(100));
        if (pgot != plen) {
            continue; // partial payload - drop it
        }

        ctx->writer.writePacket(record.ts_sec, record.ts_usec, payload, plen, record.rssi, record.channel);
    }

    ctx->writer.close();
    LOG_I(TAG, "Capture stopped");
    return 0;
}

bool isWifiOn() {
    using enum tt::service::wifi::RadioState;
    switch (tt::service::wifi::getRadioState()) {
        case On:
        case ConnectionPending:
        case ConnectionActive:
            return true;
        default:
            return false;
    }
}

void ensureWifiOn() {
    using enum tt::service::wifi::RadioState;
    auto state = tt::service::wifi::getRadioState();
    if (state == Off || state == OffPending) {
        LOG_I(TAG, "Enabling Wi-Fi radio");
        tt::service::wifi::setEnabled(true);
    }
}

void joinWriter(Context* ctx) {
    if (ctx->writerThread != nullptr) {
        thread_join(ctx->writerThread, portMAX_DELAY, 10);
        thread_free(ctx->writerThread);
        ctx->writerThread = nullptr;
    }
}

void startCapture(Context* ctx) {
    if (ctx->streamBuffer == nullptr) {
        LOG_E(TAG, "Stream buffer not allocated");
        ctx->capturing = false;
        return;
    }

    // Wait for any previous writer to fully drain before starting a new capture,
    // so two writers never share the stream buffer. This runs on the timer task,
    // not the LVGL task, so it doesn't freeze the UI.
    joinWriter(ctx);

    ctx->captureMutex.withLock([ctx] {
        // The user may have tapped Stop while we were waiting for the old writer.
        if (!ctx->capturing || ctx->active) {
            return;
        }

        Device* dev = nullptr;
        if (device_get_first_active_by_type(&WIFI_TYPE, &dev) != ERROR_NONE || dev == nullptr) {
            LOG_E(TAG, "No active Wi-Fi device");
            ctx->capturing = false;
            return;
        }
        ctx->wifiDevice = dev;

        wifi_set_promiscuous_callback(dev, onPacket, ctx);
        if (wifi_set_promiscuous(dev, true) != ERROR_NONE) {
            LOG_E(TAG, "Failed to enable promiscuous mode");
            wifi_set_promiscuous_callback(dev, nullptr, nullptr);
            device_put(dev);
            ctx->wifiDevice = nullptr;
            ctx->capturing = false;
            return;
        }

        ctx->writerStop = false;
        ctx->writerThread = thread_alloc_full("wifi_cap", 8192, captureWriterMain, ctx, tskNO_AFFINITY);
        if (ctx->writerThread == nullptr) {
            LOG_E(TAG, "Failed to allocate capture writer");
            wifi_set_promiscuous(dev, false);
            wifi_set_promiscuous_callback(dev, nullptr, nullptr);
            device_put(dev);
            ctx->wifiDevice = nullptr;
            ctx->capturing = false;
            return;
        }

        // Keep the writer below LVGL (HIGHER=6) so the UI stays responsive, but
        // above IDLE so it drains promptly and doesn't let the stream buffer fill
        // (which would increase drops and delay the final file).
        thread_set_priority(ctx->writerThread, THREAD_PRIORITY_LOW);

        if (thread_start(ctx->writerThread) != ERROR_NONE) {
            LOG_E(TAG, "Failed to start capture writer");
            wifi_set_promiscuous(dev, false);
            wifi_set_promiscuous_callback(dev, nullptr, nullptr);
            device_put(dev);
            ctx->wifiDevice = nullptr;
            thread_free(ctx->writerThread);
            ctx->writerThread = nullptr;
            ctx->capturing = false;
            return;
        }

        ctx->active = true;
        // If a channel is locked, park on it instead of hopping.
        if (ctx->lockChannel != 0) {
            wifi_set_channel(dev, ctx->lockChannel);
        }
        LOG_I(TAG, "Capture started");
    });
}

void stopCapture(Context* ctx) {
    ctx->captureMutex.withLock([ctx] {
        ctx->capturing = false;

        // Stop new frames from arriving BEFORE telling the writer to exit, so the
        // writer drains exactly the frames that were captured and no more.
        if (ctx->active) {
            ctx->active = false;
            Device* dev = ctx->wifiDevice;
            if (dev != nullptr) {
                wifi_set_promiscuous(dev, false);
                wifi_set_promiscuous_callback(dev, nullptr, nullptr);
                device_put(dev);
                ctx->wifiDevice = nullptr;
            }
        }

        // Tell the writer to drain the remaining buffer and exit on its own.
        // Do NOT join here: the drain can take a while on slow flash and would
        // freeze the caller (the LVGL task on a button press).
        ctx->writerStop = true;
    });

    // Resume WiFi auto-connect/scan if we paused it for capture.
    if (ctx->wifiAutoConnectPaused) {
        tt::service::wifi::setAutoScanPaused(false);
        ctx->wifiAutoConnectPaused = false;
    }
}

void onDeauthClicked(lv_event_t* event) {
    auto* ctx = static_cast<Context*>(lv_event_get_user_data(event));
    if (!ctx->macFilterEnabled) {
        LOG_W(TAG, "Deauth needs the MAC filter (target) set first");
        return;
    }
    if (!ctx->apBssidKnown) {
        LOG_W(TAG, "Deauth needs a known AP BSSID - capture some frames first");
        return;
    }
    if (ctx->wifiDevice == nullptr) {
        LOG_W(TAG, "Deauth needs the Wi-Fi device");
        return;
    }
    LOG_I(TAG, "Sending deauth to force a fresh handshake");
    // Dispatch so the LVGL task isn't blocked while the pinned Wi-Fi thread sends it.
    getMainDispatcher().dispatch([ctx] {
        // 802.11 deauth: management subtype 0x0C, spoofed to look like it's from the AP.
        uint8_t frame[26] = {};
        frame[0] = 0xC0; // FC: version 0, type mgmt, subtype deauth
        frame[1] = 0x00;
        std::memcpy(frame + 4, ctx->targetMac, 6);  // Addr1 = target (the Ring)
        std::memcpy(frame + 10, ctx->apBssid, 6);   // Addr2 = BSSID (spoofed source)
        std::memcpy(frame + 16, ctx->apBssid, 6);   // Addr3 = BSSID
        frame[24] = 0x07;                           // Reason: deauth due to inactivity
        frame[25] = 0x00;
        if (ctx->wifiDevice != nullptr) {
            error_t result = wifi_send_raw_frame(ctx->wifiDevice, frame, sizeof(frame));
            if (result == ERROR_NONE) {
                LOG_I(TAG, "Deauth frame transmitted");
            } else {
                LOG_E(TAG, "Deauth transmit failed (%d)", (int)result);
            }
        }
    });
}

void onPollTick(Context* ctx) {
    if (ctx->capturing && !ctx->active) {
        if (isWifiOn()) {
            startCapture(ctx);
        } else {
            ensureWifiOn();
        }
    } else if (ctx->active) {
        if (ctx->lockChannel != 0) {
            // Locked to a specific channel (e.g. for EAPOL handshake capture);
            // the channel is set at start/selection time, so just stay parked.
        } else if (tt::service::wifi::getRadioState() == tt::service::wifi::RadioState::On) {
            // Only hop when not associated with an AP (hopping would break the link).
            ctx->hopTick++;
            if (ctx->hopTick >= 2) { // hop every ~400 ms
                ctx->hopTick = 0;
                hopChannel(ctx);
            }
        }
    }

    lvgl_lock();
    updateStatsLabel(ctx);
    updateStatusLabel(ctx);
    lvgl_unlock();
}

#endif // CONFIG_SOC_WIFI_SUPPORTED

void onStartStopClicked(lv_event_t* event) {
    auto* ctx = static_cast<Context*>(lv_event_get_user_data(event));
#if defined(CONFIG_SOC_WIFI_SUPPORTED)
    if (ctx->capturing) {
        stopCapture(ctx);
    } else {
        ctx->capturing = true;
        // Pause the WiFi service's auto-connect/scan so the radio stays where it is:
        // if connected it remains locked to that channel; if disconnected it hops
        // without being yanked back onto the saved AP.
        tt::service::wifi::setAutoScanPaused(true);
        ctx->wifiAutoConnectPaused = true;
        ensureWifiOn();
    }
#endif
    updateStatusLabel(ctx);
    updateStartButtonLabel(ctx);
}

void createWidgets(lv_obj_t* parent, void* userData) {
    auto* ctx = static_cast<Context*>(userData);

    lv_obj_set_flex_flow(parent, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_row(parent, 0, LV_STATE_DEFAULT);

    auto* toolbar = lvgl_toolbar_create(parent, "Wi-Fi Monitor");
    lvgl_toolbar_set_nav_action(toolbar, LV_SYMBOL_CLOSE, onBackPressed, ctx);

    // Scrollable container: when the on-screen keyboard appears it shrinks the
    // content area; making the body scrollable lets the focused MAC field be
    // scrolled above the keyboard instead of staying hidden behind it.
    auto* wrapper = lv_obj_create(parent);
    lv_obj_set_width(wrapper, LV_PCT(100));
    lv_obj_set_flex_grow(wrapper, 1);
    lv_obj_set_flex_flow(wrapper, LV_FLEX_FLOW_COLUMN);

    ctx->statsLabel = lv_label_create(wrapper);
    lv_label_set_text(ctx->statsLabel, "Packets: 0   Dropped: 0   0 KB");

    ctx->statusLabel = lv_label_create(wrapper);
    lv_label_set_text(ctx->statusLabel, "Stopped");

#if defined(CONFIG_SOC_WIFI_SUPPORTED)
    {
        // Channel selector: "Auto" (hop) or lock to a specific channel for
        // targeted capture (e.g. a device's EAPOL handshake on a known channel).
        auto* channelDropdown = lv_dropdown_create(wrapper);
        std::string channelOptions = "Auto";
        for (uint8_t ch : kChannels) {
            channelOptions += "\n" + std::to_string(ch);
        }
        lv_dropdown_set_options(channelDropdown, channelOptions.c_str());
        lv_dropdown_set_selected(channelDropdown, 0);
        lv_obj_set_width(channelDropdown, LV_PCT(100));
        lv_obj_add_event_cb(channelDropdown, onChannelSelected, LV_EVENT_VALUE_CHANGED, ctx);
    }
    {
        // Optional MAC filter: blank = capture everything, otherwise only frames
        // to/from this address (drastically reduces drops on busy channels).
        auto* macTextarea = lv_textarea_create(wrapper);
        lv_textarea_set_placeholder_text(macTextarea, "Filter MAC (blank = all), e.g. 34:3e:a4:7e:90:45");
        lv_textarea_set_one_line(macTextarea, true);
        lv_textarea_set_accepted_chars(macTextarea, "0123456789abcdefABCDEF:");
        lv_obj_set_width(macTextarea, LV_PCT(100));
        lv_obj_add_event_cb(macTextarea, onMacChanged, LV_EVENT_VALUE_CHANGED, ctx);
    }
#endif

    auto* btnRow = lv_obj_create(wrapper);
    lv_obj_set_size(btnRow, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(btnRow, LV_FLEX_FLOW_ROW);
    lv_obj_set_style_pad_all(btnRow, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_pad_column(btnRow, 8, 0);
    lv_obj_set_style_border_opa(btnRow, 0, LV_STATE_DEFAULT);

    auto* button = lv_button_create(btnRow);
    lv_obj_set_flex_grow(button, 1);
    ctx->startButtonLabel = lv_label_create(button);
    lv_label_set_text(ctx->startButtonLabel, "Start");
    lv_obj_center(ctx->startButtonLabel);
    lv_obj_add_event_cb(button, onStartStopClicked, LV_EVENT_SHORT_CLICKED, ctx);

#if defined(CONFIG_SOC_WIFI_SUPPORTED)
    auto* deauthButton = lv_button_create(btnRow);
    lv_obj_set_flex_grow(deauthButton, 1);
    auto* deauthLabel = lv_label_create(deauthButton);
    lv_label_set_text(deauthLabel, "Deauth");
    lv_obj_center(deauthLabel);
    lv_obj_add_event_cb(deauthButton, onDeauthClicked, LV_EVENT_SHORT_CLICKED, ctx);
#endif
}

int32_t appMain(uint32_t appInstanceId, int argc, char* argv[]) {
    Context ctx;
    ctx.appInstanceId = appInstanceId;

#if defined(CONFIG_SOC_WIFI_SUPPORTED)
    ctx.streamBufferStorage = static_cast<uint8_t*>(heap_caps_malloc(STREAM_BUFFER_SIZE, MALLOC_CAP_SPIRAM));
    if (ctx.streamBufferStorage == nullptr) {
        LOG_E(TAG, "Failed to allocate stream buffer in PSRAM");
    } else {
        ctx.streamBuffer = xStreamBufferCreateStatic(STREAM_BUFFER_SIZE, 1, ctx.streamBufferStorage, &s_streamBufferStruct);
    }
#endif

    ctx.pollTimer = std::make_unique<Timer>(Timer::Type::Periodic, millis_to_ticks(POLL_INTERVAL_MS), [&ctx] {
#if defined(CONFIG_SOC_WIFI_SUPPORTED)
        onPollTick(&ctx);
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
        switch (event.type) {
            case APP_EVENT_CLOSE:
#if defined(CONFIG_SOC_WIFI_SUPPORTED)
                stopCapture(&ctx);
#endif
                app_manager_finish(appInstanceId);
                shouldClose = true;
                break;
            default:
                break;
        }
        }
    }

    ctx.pollTimer->stop();
    window_manager_remove(window);
    app_event_unsubscribe(&sub);
    task_event_group_destruct(&event_group);

#if defined(CONFIG_SOC_WIFI_SUPPORTED)
    // Wait for the writer to finish draining before freeing its stream buffer.
    joinWriter(&ctx);
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
    app_manager_start(manifest.id, &instanceId);
    return instanceId;
}

extern const ::AppManifest manifest = {
    .id = "WifiMonitor",
    .name = "Wi-Fi Monitor",
    .category = APP_CATEGORY_USER,
    .location = { APP_LOCATION_MEMORY, reinterpret_cast<void*>(appMain) },
    .flags = 0
};

} // namespace tt::app::wifimonitor
