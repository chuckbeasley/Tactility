#ifdef ESP_PLATFORM
#include <sdkconfig.h>
#endif

#if defined(CONFIG_SOC_WIFI_SUPPORTED) || defined(CONFIG_SLAVE_SOC_WIFI_SUPPORTED)

#include <esp_event.h>
#include <esp_netif.h>
#include <esp_wifi.h>
#include <esp_wifi_default.h>

// ---- WiFi health monitor ----
// The failure this exists for leaves the station associated with a healthy RSSI while no data moves
// in either direction, for minutes at a time, with nothing logged and no panic - association state,
// RSSI and the IP configuration all stay normal, so only an end-to-end probe can see it. It was
// observed four times in one day at 10, 29, 35 and 40 minutes of uptime, and it heals itself, so the
// probe's job is twofold: shorten the blackout by forcing a reconnect, and record the lwIP counters
// at the moment the fault is detected, which is the evidence the investigation is missing.
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <unistd.h>

#include <esp_timer.h>
#include <lwip/sockets.h>
#include <lwip/stats.h>
// Included here rather than with the rest of the Tactility headers below, because the health monitor
// logs from above that point.
#include <tactility/log.h>

constexpr auto* WIFI_HEALTH_TAG = "wifi_health";

static esp_netif_t* s_healthNetif = nullptr;
static int s_healthFailures = 0;
static esp_timer_handle_t s_healthTimer = nullptr;

static constexpr int HEALTH_PROBE_INTERVAL_S = 5;
static constexpr int HEALTH_PROBE_TIMEOUT_S = 3;
static constexpr int HEALTH_FAILURES_BEFORE_RECONNECT = 3;

// ---- Recovery ----
//
// The fault this monitor exists for is invisible from the association state: the station stays
// associated at a healthy RSSI (measured at -42 dBm, with a PC on the same BSSID still reaching the
// gateway), beacons keep arriving - no AP-loss disconnect ever fires, and the driver's own beacon
// timeout is 25 s - and no data moves in either direction: TCP connects to the gateway, to a PC on
// the same LAN, to the DNS server and to the internet all fail with ETIMEDOUT, and DNS resolution
// fails with them. Measured windows of 60-100 seconds of that, and it is not the power-save mode (it
// happens with the radio wide awake), not the BLE scan duty cycle (cutting it changed nothing) and not
// a reboot (the fault windows contain no boot banner). What ends it: re-associating restored traffic
// in some observations, and in others the link recovered on its own within a minute.
//
// It does not always come back on the first re-association either. One measured recovery needed two
// disconnects twelve seconds apart: the first gave a new DHCP lease in 1.1 s and the link stayed dead,
// the second took 12 s and then carried traffic normally for the next two minutes. So a recovery is a
// staged, verified sequence rather than a fire-and-forget disconnect: each stage gets a window in
// which the probe decides whether it worked, a stage that did not is escalated once, and a round that
// fails entirely is followed by a cooldown that grows. Reconnecting every 15 seconds on a link that is
// not coming back costs the DHCP lease, every open socket and any request in flight, and fixes nothing.
static constexpr int HEALTH_STAGE_NONE = 0;
static constexpr int HEALTH_STAGE_REASSOCIATE = 1;
static constexpr int HEALTH_STAGE_FRESH_JOIN = 2;
static constexpr int64_t HEALTH_VERIFY_US = 12 * 1000 * 1000;
static constexpr int HEALTH_COOLDOWN_SECONDS[] = { 15, 30, 60, 120, 300 };

static int s_healthStage = HEALTH_STAGE_NONE;
static int s_healthRounds = 0;
static int64_t s_healthStageStartUs = 0;
static int64_t s_healthStageDeadlineUs = 0;
static int64_t s_healthCooldownUntilUs = 0;

static int cooldownSeconds(int round) {
    constexpr int count = (int)(sizeof(HEALTH_COOLDOWN_SECONDS) / sizeof(HEALTH_COOLDOWN_SECONDS[0]));
    return HEALTH_COOLDOWN_SECONDS[round < 0 ? 0 : (round < count ? round : count - 1)];
}

/** Re-associate, or re-join without the cached BSSID and channel so the driver scans again.
 *
 *  The cached BSS is what a fast reconnect reuses, and a station that re-joins the same mesh node it
 *  just failed on is not obviously a different attempt: clearing it makes the second stage a fresh
 *  scan, which can land on another node of the same SSID.
 */
static void startRecoveryStage(int stage, int64_t now_us) {
    // Disconnect first: the driver refuses a configuration change while a connection is being
    // established (measured: esp_wifi_set_config returned an error when it was called before the
    // disconnect, and the log said so on the first staged recovery this ran).
    esp_wifi_disconnect();

    if (stage == HEALTH_STAGE_FRESH_JOIN) {
        wifi_config_t config = {};
        const esp_err_t get_result = esp_wifi_get_config(WIFI_IF_STA, &config);
        if (get_result == ESP_OK) {
            config.sta.bssid_set = false;
            config.sta.channel = 0;
            const esp_err_t set_result = esp_wifi_set_config(WIFI_IF_STA, &config);
            if (set_result != ESP_OK) {
                LOG_W(WIFI_HEALTH_TAG, "Could not clear the cached BSS/channel before re-joining: %s",
                    esp_err_to_name(set_result));
            }
        } else {
            LOG_W(WIFI_HEALTH_TAG, "Could not read the station config before re-joining: %s",
                esp_err_to_name(get_result));
        }
        LOG_W(WIFI_HEALTH_TAG, "Re-joining from a fresh scan");
    } else {
        LOG_W(WIFI_HEALTH_TAG, "Re-associating to recover the link");
    }

    s_healthStage = stage;
    s_healthStageStartUs = now_us;
    s_healthStageDeadlineUs = now_us + HEALTH_VERIFY_US;
    esp_wifi_connect();
}

/** One TCP connect to the gateway's port 80: did a round trip to it complete?
 *
 *  A refusal counts as an answer. The far end sending a RST is the gateway proving it received the
 *  SYN and replied, which is what "is there a path at all" asks; counting it as unreachable would tear
 *  down a working link on any gateway whose port 80 is closed.
 */
static bool probeGateway(uint32_t gateway_address) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        return true;  // No socket to probe with is not evidence about the link.
    }
    fcntl(sock, F_SETFL, O_NONBLOCK);

    struct sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(80);
    addr.sin_addr.s_addr = gateway_address;

    bool answered = connect(sock, (struct sockaddr*)&addr, sizeof(addr)) == 0;
    if (!answered && errno == EINPROGRESS) {
        fd_set write_set;
        FD_ZERO(&write_set);
        FD_SET(sock, &write_set);
        struct timeval timeout = {};
        timeout.tv_sec = HEALTH_PROBE_TIMEOUT_S;
        if (select(sock + 1, nullptr, &write_set, nullptr, &timeout) > 0) {
            int socket_error = 0;
            socklen_t length = sizeof(socket_error);
            getsockopt(sock, SOL_SOCKET, SO_ERROR, &socket_error, &length);
            answered = socket_error == 0 || socket_error == ECONNREFUSED;
        }
    }
    close(sock);
    return answered;
}

#include <esp_rom_sys.h>
#include <esp_rom_uart.h>
#include <freertos/task.h>

static TaskStatus_t s_healthTaskTable[24];

// Printed only when the monitor fires, never continuously. The serial wedge probe wrote ten lines a
// second and every fault observed during this investigation happened on firmware without it, so this
// instrument stays silent until there is something to say. State: 0 running, 1 ready, 2 blocked,
// 3 suspended.
static void dumpHealthTaskTable() {
    const UBaseType_t count = uxTaskGetSystemState(s_healthTaskTable, 24, nullptr);
    esp_rom_printf("[wifi_health] %u tasks:\n", (unsigned)count);
    for (UBaseType_t i = 0; i < count; ++i) {
        esp_rom_printf("[wifi_health]   %-16s state=%u prio=%u stack_free=%u\n",
            s_healthTaskTable[i].pcTaskName,
            (unsigned)s_healthTaskTable[i].eCurrentState,
            (unsigned)s_healthTaskTable[i].uxCurrentPriority,
            (unsigned)s_healthTaskTable[i].usStackHighWaterMark);
    }
}

static void wifiHealthTick(void*) {
    if (s_healthNetif == nullptr) {
        return;
    }

    // Serial memory report: send '?' on the console and the heap figures come back. Polled from this
    // tick rather than from a task of its own so it costs no stack, and deliberately silent otherwise
    // - continuous console output has been observed to suppress the intermittent fault this board is
    // being investigated for, and the only other way to read memory (/api/sysinfo) is served by a
    // service that may be stopped, wedged, or the very thing being measured.
    uint8_t console_key = 0;
    if (esp_rom_uart_rx_one_char(&console_key) == 0 && console_key == '?') {
        esp_rom_printf("[mem] internal free=%u largest=%u min=%u | psram free=%u largest=%u\n",
            (unsigned)heap_caps_get_free_size(MALLOC_CAP_INTERNAL),
            (unsigned)heap_caps_get_largest_free_block(MALLOC_CAP_INTERNAL),
            (unsigned)heap_caps_get_minimum_free_size(MALLOC_CAP_INTERNAL),
            (unsigned)heap_caps_get_free_size(MALLOC_CAP_SPIRAM),
            (unsigned)heap_caps_get_largest_free_block(MALLOC_CAP_SPIRAM));
    }

    esp_netif_ip_info_t info = {};
    if (esp_netif_get_ip_info(s_healthNetif, &info) != ESP_OK || info.gw.addr == 0) {
        // No address to probe yet: not a failure, just not connected.
        s_healthFailures = 0;
        return;
    }

    const int64_t now_us = esp_timer_get_time();
    const bool linkUp = probeGateway(info.gw.addr);

    if (linkUp) {
        if (s_healthStage != HEALTH_STAGE_NONE) {
            LOG_I(WIFI_HEALTH_TAG, "Link recovered after stage %d (%d ms)",
                s_healthStage, (int)((now_us - s_healthStageStartUs) / 1000));
        }
        s_healthFailures = 0;
        s_healthStage = HEALTH_STAGE_NONE;
        s_healthRounds = 0;
        s_healthCooldownUntilUs = 0;
        return;
    }

    s_healthFailures++;

    // A recovery is in flight: give it its verification window before judging it.
    if (s_healthStage != HEALTH_STAGE_NONE) {
        if (now_us < s_healthStageDeadlineUs) {
            return;
        }
        if (s_healthStage == HEALTH_STAGE_REASSOCIATE) {
            LOG_W(WIFI_HEALTH_TAG, "Link still down %d ms after re-associating - re-joining from a fresh scan",
                (int)((now_us - s_healthStageStartUs) / 1000));
            startRecoveryStage(HEALTH_STAGE_FRESH_JOIN, now_us);
            return;
        }
        LOG_W(WIFI_HEALTH_TAG, "Link still down %d ms after a fresh join - backing off for %d s",
            (int)((now_us - s_healthStageStartUs) / 1000), cooldownSeconds(s_healthRounds) );
        s_healthRounds++;
        s_healthStage = HEALTH_STAGE_NONE;
        s_healthCooldownUntilUs = now_us + (int64_t)cooldownSeconds(s_healthRounds) * 1000 * 1000;
        s_healthFailures = 0;
        return;
    }

    if (now_us < s_healthCooldownUntilUs) {
        return;
    }

    if (s_healthFailures < HEALTH_FAILURES_BEFORE_RECONNECT) {
        return;
    }

    wifi_ap_record_t ap = {};
    const bool associated = esp_wifi_sta_get_ap_info(&ap) == ESP_OK;
#if LWIP_STATS && IP_STATS && TCP_STATS
    LOG_W(WIFI_HEALTH_TAG, "gateway unreachable for %d probes (associated=%d rssi=%d ip_recv=%u tcp_recv=%u) - recovering",
        s_healthFailures, (int)associated, (int)ap.rssi,
        (unsigned)lwip_stats.ip.recv, (unsigned)lwip_stats.tcp.recv);
#else
    LOG_W(WIFI_HEALTH_TAG, "gateway unreachable for %d probes (associated=%d rssi=%d) - recovering",
        s_healthFailures, (int)associated, (int)ap.rssi);
#endif
    dumpHealthTaskTable();
    startRecoveryStage(HEALTH_STAGE_REASSOCIATE, now_us);
}

#include <tactility/concurrent/mutex.h>
#include <tactility/device.h>
#include <tactility/driver.h>
#include <tactility/drivers/esp32_wifi.h>
#include <tactility/drivers/wifi.h>
#include <tactility/error_esp32.h>
#include <tactility/log.h>
#include <tactility/time.h>

#include <TactilityCpp/Allocator.h>

#if defined(CONFIG_ESP_HOSTED_ENABLED)
#include <tactility/drivers/esp32_esp_hosted_ota.h>
#endif

#include "tactility/system_event.h"


#include <algorithm>
#include <cstdio>
#include <cstring>
#include <new>

#define TAG "esp32_wifi"

namespace {

constexpr uint16_t WIFI_SCAN_RECORD_LIMIT = 32;

struct Esp32WifiCtx {
    Device* device = nullptr;

    Mutex mutex{};
    WifiRadioState radioState = WIFI_RADIO_STATE_OFF;
    WifiStationState stationState = WIFI_STATION_STATE_DISCONNECTED;
    bool scanning = false;
    char targetSsid[33] = {};
    esp_netif_ip_info_t ipInfo{};
    wifi_ap_record_t scanResults[WIFI_SCAN_RECORD_LIMIT] = {};
    uint16_t scanResultCount = 0;

    esp_netif_t* netif = nullptr;
    esp_event_handler_instance_t wifiEventHandler = nullptr;
    esp_event_handler_instance_t ipEventHandler = nullptr;

    // Dedup for WIFI_EVENT/IP_EVENT notifications: on the esp_hosted/Wi-Fi Remote transport
    // (e.g. Tab5's P4 host + C6 co-processor), the RPC layer has been observed delivering the
    // exact same event twice in a row (same base, same event_id, same millisecond - not two
    // genuinely separate occurrences). Native WiFi doesn't exhibit this, but the handler is
    // shared, so the guard applies unconditionally; it's a no-op for well-separated real events.
    esp_event_base_t lastEventBase = nullptr;
    int32_t lastEventId = -1;
    TickType_t lastEventTick = 0;

    Mutex subscriptionsMutex{};
    WifiEventSubscription* subscriptions = nullptr;
};

#define GET_CTX(device) (static_cast<Esp32WifiCtx*>(device_get_driver_data(device)))

// Promiscuous-mode RX trampoline. ESP-IDF's esp_wifi_set_promiscuous_rx_cb() takes only a
// function pointer (no user context), so a single file-scope channel carries the callback + context
// that the app registered via api_set_promiscuous_callback(). There is exactly one WiFi device per
// board, so a file-scope channel is unambiguous and keeps the RX path allocation-free.
static WifiPromiscuousCallback g_promiscuousCallback = nullptr;
static void* g_promiscuousContext = nullptr;

// Called from ESP-IDF's Wi-Fi task. Must be short and non-blocking: copy nothing, just forward the
// raw 802.11 frame (payload[0], sig_len in the rx_ctrl, FCS included) to the app's callback. The app
// is responsible for any further filtering/copying.
static void promiscuous_rx_cb(void* buffer, wifi_promiscuous_pkt_type_t type) {
    if (g_promiscuousCallback == nullptr || buffer == nullptr) {
        return;
    }
    auto* pkt = static_cast<wifi_promiscuous_pkt_t*>(buffer);

    WifiPromiscuousPacketInfo info {};
    info.rssi = static_cast<int8_t>(pkt->rx_ctrl.rssi);
    info.channel = pkt->rx_ctrl.channel;
    switch (type) {
        case WIFI_PKT_MGMT: info.type = WIFI_PROMISCUOUS_PACKET_TYPE_MGMT; break;
        case WIFI_PKT_CTRL: info.type = WIFI_PROMISCUOUS_PACKET_TYPE_CTRL; break;
        case WIFI_PKT_DATA: info.type = WIFI_PROMISCUOUS_PACKET_TYPE_DATA; break;
        default:           info.type = WIFI_PROMISCUOUS_PACKET_TYPE_MISC; break;
    }

    // sig_len is the reception MPDU length including the 4-byte 802.11 FCS; the payload buffer holds
    // exactly that many bytes. Forwarding length=sig_len keeps the provider/consumer (e.g. PCAP
    // writer with its radiotap "FCS at end" flag) consistent.
    g_promiscuousCallback(g_promiscuousContext, pkt->payload, pkt->rx_ctrl.sig_len, info);
}

WifiAuthenticationType to_wifi_authentication_type(wifi_auth_mode_t mode) {
    switch (mode) {
        case WIFI_AUTH_OPEN: return WIFI_AUTHENTICATION_TYPE_OPEN;
        case WIFI_AUTH_WEP: return WIFI_AUTHENTICATION_TYPE_WEP;
        case WIFI_AUTH_WPA_PSK: return WIFI_AUTHENTICATION_TYPE_WPA_PSK;
        case WIFI_AUTH_WPA2_PSK: return WIFI_AUTHENTICATION_TYPE_WPA2_PSK;
        case WIFI_AUTH_WPA_WPA2_PSK: return WIFI_AUTHENTICATION_TYPE_WPA_WPA2_PSK;
        case WIFI_AUTH_WPA2_ENTERPRISE: return WIFI_AUTHENTICATION_TYPE_WPA2_ENTERPRISE;
        case WIFI_AUTH_WPA3_PSK: return WIFI_AUTHENTICATION_TYPE_WPA3_PSK;
        case WIFI_AUTH_WPA2_WPA3_PSK: return WIFI_AUTHENTICATION_TYPE_WPA2_WPA3_PSK;
        case WIFI_AUTH_WAPI_PSK: return WIFI_AUTHENTICATION_TYPE_WAPI_PSK;
        case WIFI_AUTH_OWE: return WIFI_AUTHENTICATION_TYPE_OWE;
        case WIFI_AUTH_WPA3_ENT_192: return WIFI_AUTHENTICATION_TYPE_WPA3_ENT_192;
        case WIFI_AUTH_DUMMY_1: return WIFI_AUTHENTICATION_TYPE_WPA3_EXT_PSK;
        case WIFI_AUTH_DUMMY_2: return WIFI_AUTHENTICATION_TYPE_WPA3_EXT_PSK_MIXED_MODE;
        default: return WIFI_AUTHENTICATION_TYPE_OPEN;
    }
}

void fire_event(Esp32WifiCtx* ctx, WifiEvent event) {
    mutex_lock(&ctx->subscriptionsMutex);
    for (WifiEventSubscription* sub = ctx->subscriptions; sub != nullptr; sub = sub->internal.next) {
        mutex_lock(&sub->internal.ring_mutex);
        if (sub->internal.count < WIFI_EVENT_QUEUE_CAPACITY) {
            uint8_t tail = (sub->internal.head + sub->internal.count) % WIFI_EVENT_QUEUE_CAPACITY;
            sub->internal.queue[tail] = event;
            sub->internal.count++;
        }
        mutex_unlock(&sub->internal.ring_mutex);
        task_event_group_signal(sub->internal.event_group, sub->bit);
    }
    mutex_unlock(&ctx->subscriptionsMutex);
}

// ---- Wi-Fi FTM (802.11mc) probe ----
//
// PROBE, temporary: answers one question - does the AP this board is associated with implement
// 802.11mc Fine Timing Measurement, i.e. is there a real distance measurement available here instead
// of an RSSI estimate? The C5's radio supports it (SOC_WIFI_FTM_SUPPORT), the host API hands back
// dist_est in centimetres, and the only unknown is the far end: an AP that does not answer as an FTM
// responder makes the whole feature moot.
//
// Runs once, a couple of seconds after the station gets its IP, and logs the outcome. Every status is
// logged, including the ones that mean "not supported", because that is the answer being looked for -
// not just the successful case.
//
// Behind CONFIG_ESP_WIFI_FTM_ENABLE (the API does not exist without it), so every other device in
// this tree still builds; only the profile being measured turns it on.
#if defined(CONFIG_ESP_WIFI_FTM_ENABLE)
static esp_timer_handle_t s_ftmProbeTimer = nullptr;
static esp_timer_handle_t s_ftmProbeScanTimer = nullptr;
static bool s_ftmProbeDone = false;
static int s_ftmProbeAttempts = 0;
static bool s_ftmProbeApClaimsResponder = false;
static bool s_ftmProbeScanWanted = false;
static int s_ftmProbeScanAttempts = 0;
static constexpr int FTM_PROBE_MAX_ATTEMPTS = 3;
static constexpr int FTM_PROBE_MAX_SCAN_ATTEMPTS = 4;

static void ftmProbeStart(void* /*arg*/) {
    wifi_ap_record_t ap = {};
    esp_err_t err = esp_wifi_sta_get_ap_info(&ap);
    if (err != ESP_OK) {
        LOG_W("WifiFtm", "PROBE: no associated AP (%s)", esp_err_to_name(err));
        return;
    }

    char bssid[18];
    std::snprintf(bssid, sizeof(bssid), "%02x:%02x:%02x:%02x:%02x:%02x",
        ap.bssid[0], ap.bssid[1], ap.bssid[2], ap.bssid[3], ap.bssid[4], ap.bssid[5]);

    wifi_ftm_initiator_cfg_t cfg = {};
    std::memcpy(cfg.resp_mac, ap.bssid, 6);
    cfg.channel = ap.primary;
    cfg.frm_count = 16;
    cfg.burst_period = 2;

    // Whether the AP advertises 802.11mc at all is visible before any frame is sent: the Extended
    // Capabilities element's "FTM Responder" bit, which the driver already parses into this record.
    // Logging it separates "the AP never claimed to support this" from "it claimed to and still did
    // not answer", which are different problems with different fixes.
    if (s_ftmProbeAttempts == 0) {
        s_ftmProbeApClaimsResponder = ap.ftm_responder != 0;
        LOG_I("WifiFtm", "PROBE: AP %s (\"%s\") ch=%u rssi=%d ftm_responder=%d ftm_initiator=%d",
            bssid, reinterpret_cast<const char*>(ap.ssid), (unsigned)ap.primary, (int)ap.rssi,
            (int)ap.ftm_responder, (int)ap.ftm_initiator);
    }
    s_ftmProbeAttempts++;

    LOG_I("WifiFtm", "PROBE: FTM session attempt %d/%d (frm_count=%u burst_period=%u)",
        s_ftmProbeAttempts, FTM_PROBE_MAX_ATTEMPTS, (unsigned)cfg.frm_count, (unsigned)cfg.burst_period);

    err = esp_wifi_ftm_initiate_session(&cfg);
    if (err != ESP_OK) {
        LOG_W("WifiFtm", "PROBE: esp_wifi_ftm_initiate_session failed: %s", esp_err_to_name(err));
    }
}

static void ftmProbeScheduleOnce() {
    if (s_ftmProbeDone) return;
    s_ftmProbeDone = true;
    s_ftmProbeAttempts = 0;
    if (s_ftmProbeTimer == nullptr) {
        esp_timer_create_args_t args = {};
        args.callback        = ftmProbeStart;
        args.dispatch_method = ESP_TIMER_TASK;
        args.name            = "ftm_probe";
        if (esp_timer_create(&args, &s_ftmProbeTimer) != ESP_OK) {
            LOG_E("WifiFtm", "PROBE: timer create failed");
            return;
        }
    }
    // Two seconds: the association has just completed and the driver is still finishing its own
    // post-connect work; an FTM request sent into that is a needless second variable.
    esp_timer_start_once(s_ftmProbeTimer, 2 * 1000 * 1000);
}

// Starts (or restarts) the scan for other 802.11mc responders. Runs on the esp_timer task: the scan
// itself is non-blocking, because a blocking one would need the AP records on this task's 2 KB stack.
static void ftmProbeScanStart(void* /*arg*/) {
    s_ftmProbeScanWanted = true;
    esp_err_t err = esp_wifi_scan_start(nullptr, false);
    if (err != ESP_OK) {
        s_ftmProbeScanWanted = false;
        LOG_W("WifiFtm", "PROBE: could not scan for other FTM responders: %s", esp_err_to_name(err));
    }
}

static void ftmProbeLogReport(const wifi_event_ftm_report_t* report) {
    char peer[18];
    std::snprintf(peer, sizeof(peer), "%02x:%02x:%02x:%02x:%02x:%02x",
        report->peer_mac[0], report->peer_mac[1], report->peer_mac[2],
        report->peer_mac[3], report->peer_mac[4], report->peer_mac[5]);

    const char* status;
    switch (report->status) {
        case FTM_STATUS_SUCCESS:          status = "SUCCESS"; break;
        case FTM_STATUS_UNSUPPORTED:      status = "UNSUPPORTED (peer does not do FTM)"; break;
        case FTM_STATUS_CONF_REJECTED:    status = "CONF_REJECTED"; break;
        case FTM_STATUS_NO_RESPONSE:      status = "NO_RESPONSE"; break;
        case FTM_STATUS_FAIL:             status = "FAIL"; break;
        case FTM_STATUS_NO_VALID_MSMT:    status = "NO_VALID_MSMT"; break;
        case FTM_STATUS_USER_TERM:        status = "USER_TERM"; break;
        default:                          status = "UNKNOWN"; break;
    }

    LOG_I("WifiFtm", "PROBE: report from %s: status=%s rtt_raw=%uns rtt_est=%uns dist_est=%ucm entries=%u",
        peer, status, (unsigned)report->rtt_raw, (unsigned)report->rtt_est,
        (unsigned)report->dist_est, (unsigned)report->ftm_report_num_entries);

    if (report->status == FTM_STATUS_SUCCESS && report->ftm_report_num_entries > 0) {
        // The per-burst entries carry the raw timestamps and each frame's RSSI; the first few are
        // enough to see whether the measurement is consistent or drifting.
        wifi_ftm_report_entry_t entries[8] = {};
        uint8_t count = report->ftm_report_num_entries;
        if (count > 8) count = 8;
        if (esp_wifi_ftm_get_report(entries, count) == ESP_OK) {
            for (uint8_t i = 0; i < count; ++i) {
                LOG_I("WifiFtm", "PROBE:   burst %u: rtt=%ups rssi=%d ppm=%d",
                    (unsigned)i, (unsigned)entries[i].rtt, (int)entries[i].rssi, (int)entries[i].ppm);
            }
        }
        LOG_I("WifiFtm", "PROBE verdict: FTM WORKS against this AP - %.2f m (rtt_est %u ns over %u bursts)",
            (double)report->dist_est / 100.0, (unsigned)report->rtt_est,
            (unsigned)report->ftm_report_num_entries);
    } else {
        // Free the internal report; the API documents NULL as "just release it".
        esp_wifi_ftm_get_report(nullptr, 0);

        if (s_ftmProbeAttempts < FTM_PROBE_MAX_ATTEMPTS && s_ftmProbeTimer != nullptr) {
            LOG_I("WifiFtm", "PROBE: retrying in 2s (attempt %d of %d)",
                s_ftmProbeAttempts + 1, FTM_PROBE_MAX_ATTEMPTS);
            esp_timer_start_once(s_ftmProbeTimer, 2 * 1000 * 1000);
        } else {
            LOG_I("WifiFtm", "PROBE verdict: no usable FTM from this AP after %d attempts "
                "(AP advertises FTM responder: %s) - RSSI ranging is the only option here",
                s_ftmProbeAttempts, s_ftmProbeApClaimsResponder ? "yes" : "no");
            // And before giving up on 802.11mc entirely: is there any other AP in range that does
            // answer as a responder? The result is reported from the driver's own scan-complete path,
            // where the records are already read into the context - a blocking scan here would run on
            // the 2 KB esp_timer stack.
            if (s_ftmProbeScanTimer == nullptr) {
                esp_timer_create_args_t scan_args = {};
                scan_args.callback        = ftmProbeScanStart;
                scan_args.dispatch_method = ESP_TIMER_TASK;
                scan_args.name            = "ftm_scan";
                if (esp_timer_create(&scan_args, &s_ftmProbeScanTimer) != ESP_OK) {
                    LOG_E("WifiFtm", "PROBE: scan timer create failed");
                    return;
                }
            }
            s_ftmProbeScanAttempts = 0;
            esp_timer_start_once(s_ftmProbeScanTimer, 2 * 1000 * 1000);
        }
    }
}
#endif // CONFIG_ESP_WIFI_FTM_ENABLE

// ---- ESP-IDF event handling (runs on the esp_event task) ----

void on_wifi_or_ip_event(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
    auto* ctx = static_cast<Esp32WifiCtx*>(arg);

    // See Esp32WifiCtx::lastEventBase/lastEventId/lastEventTick - collapse an immediate duplicate
    // delivery of the same event (observed on the esp_hosted/Wi-Fi Remote transport) into one.
    constexpr uint32_t DEDUP_WINDOW_MS = 50; // well under any real re-occurrence of the same event
    TickType_t now = get_ticks();
    bool is_duplicate = event_base == ctx->lastEventBase && event_id == ctx->lastEventId &&
        (now - ctx->lastEventTick) <= millis_to_ticks(DEDUP_WINDOW_MS);
    ctx->lastEventBase = event_base;
    ctx->lastEventId = event_id;
    ctx->lastEventTick = now;
    if (is_duplicate) {
        LOG_D(TAG, "Ignoring duplicate WiFi event %d", (int)event_id);
        return;
    }

    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        mutex_lock(&ctx->mutex);
        bool was_pending = ctx->stationState == WIFI_STATION_STATE_CONNECTION_PENDING;
        ctx->stationState = WIFI_STATION_STATE_DISCONNECTED;
        memset(&ctx->ipInfo, 0, sizeof(ctx->ipInfo));
        mutex_unlock(&ctx->mutex);

        WifiEvent state_event = {};
        state_event.type = WIFI_EVENT_TYPE_STATION_STATE_CHANGED;
        state_event.station_state = WIFI_STATION_STATE_DISCONNECTED;
        fire_event(ctx, state_event);

        if (was_pending) {
            WifiEvent result_event = {};
            result_event.type = WIFI_EVENT_TYPE_STATION_CONNECTION_RESULT;
            result_event.connection_error = WIFI_STATION_CONNECTION_ERROR_TARGET_NOT_FOUND;
            fire_event(ctx, result_event);
            NetworkDisconnectedEvent disconnected_event = {
                .device = ctx->device
            };
            system_event_emit(KERNEL_EVENT_NETWORK_DISCONNECTED, &disconnected_event, sizeof(disconnected_event));
        }
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        auto* got_ip = static_cast<ip_event_got_ip_t*>(event_data);

        mutex_lock(&ctx->mutex);
        ctx->ipInfo = got_ip->ip_info;
        ctx->stationState = WIFI_STATION_STATE_CONNECTED;
        mutex_unlock(&ctx->mutex);

        WifiEvent state_event = {};
        state_event.type = WIFI_EVENT_TYPE_STATION_STATE_CHANGED;
        state_event.station_state = WIFI_STATION_STATE_CONNECTED;
        fire_event(ctx, state_event);

        WifiEvent result_event = {};
        result_event.type = WIFI_EVENT_TYPE_STATION_CONNECTION_RESULT;
        result_event.connection_error = WIFI_STATION_CONNECTION_ERROR_NONE;
        fire_event(ctx, result_event);

        NetworkConnectedEvent connected_event = {
            .device = ctx->device,
            .ipv4_addr = ctx->ipInfo.ip.addr,
            .gateway = ctx->ipInfo.gw.addr,
        };
        system_event_emit(KERNEL_EVENT_NETWORK_CONNECTED, &connected_event, sizeof(connected_event));

        // The FTM branch below is inside this #if on purpose: the else-if chain stays intact whether
        // or not the feature is compiled in, because the branch's own body wraps the brace.
        // PROBE: one FTM session per boot, against whatever AP this associated with.
#if defined(CONFIG_ESP_WIFI_FTM_ENABLE)
        ftmProbeScheduleOnce();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_FTM_REPORT) {
        // PROBE: the outcome of the session started above.
        ftmProbeLogReport(static_cast<const wifi_event_ftm_report_t*>(event_data));
#endif
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_SCAN_DONE) {
        mutex_lock(&ctx->mutex);
        ctx->scanning = false;
        uint16_t count = WIFI_SCAN_RECORD_LIMIT;
        esp_err_t err = esp_wifi_scan_get_ap_records(&count, ctx->scanResults);
        ctx->scanResultCount = (err == ESP_OK) ? count : 0;
        mutex_unlock(&ctx->mutex);

        WifiEvent event = {};
        event.type = WIFI_EVENT_TYPE_SCAN_FINISHED;
        fire_event(ctx, event);

#if defined(CONFIG_ESP_WIFI_FTM_ENABLE)
        if (s_ftmProbeScanWanted) {
            unsigned responders = 0;
            for (uint16_t i = 0; i < count; ++i) {
                if (ctx->scanResults[i].ftm_responder == 0) continue;
                responders++;
                LOG_I("WifiFtm", "PROBE: 802.11mc responder in range: \"%s\" rssi=%d ch=%u",
                    reinterpret_cast<const char*>(ctx->scanResults[i].ssid),
                    (int)ctx->scanResults[i].rssi, (unsigned)ctx->scanResults[i].primary);
            }

            s_ftmProbeScanAttempts++;
            if (count == 0 && s_ftmProbeScanAttempts < FTM_PROBE_MAX_SCAN_ATTEMPTS) {
                // A scan that comes back with no records at all means it was cut short rather than
                // that the air is empty - observed here as the Wi-Fi health monitor forcing a
                // reconnect mid-scan, which tears the scan down with it. Retry rather than report a
                // measurement that never happened.
                LOG_I("WifiFtm", "PROBE: scan %d returned no records (interrupted?), retrying in 5s",
                    s_ftmProbeScanAttempts);
                esp_timer_start_once(s_ftmProbeScanTimer, 5 * 1000 * 1000);
            } else {
                s_ftmProbeScanWanted = false;
                LOG_I("WifiFtm", "PROBE verdict: %u of %u APs in range advertise 802.11mc support "
                    "(scan attempt %d)", responders, (unsigned)count, s_ftmProbeScanAttempts);
            }
        }
#endif
    }
}

// ---- Work, always run inline on the caller (see note at the top) ----

error_t bring_up_wifi(Esp32WifiCtx* ctx) {
    ctx->netif = esp_netif_create_default_wifi_sta();
    if (ctx->netif == nullptr) {
        LOG_E(TAG, "Failed to create default STA netif");
        return ERROR_RESOURCE;
    }

    // Warning: this is the memory-intensive operation. It uses over 100kB of
    // RAM with default settings.
    wifi_init_config_t init_config = WIFI_INIT_CONFIG_DEFAULT();
    esp_err_t err = esp_wifi_init(&init_config);
    if (err != ESP_OK) {
        LOG_E(TAG, "esp_wifi_init failed: %s", esp_err_to_name(err));
        esp_netif_destroy(ctx->netif);
        ctx->netif = nullptr;
        return esp_err_to_error(err);
    }

    esp_wifi_set_storage(WIFI_STORAGE_RAM);

    err = esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &on_wifi_or_ip_event, ctx, &ctx->wifiEventHandler);
    if (err != ESP_OK) {
        LOG_E(TAG, "Failed to register WIFI_EVENT handler: %s", esp_err_to_name(err));
        esp_wifi_deinit();
        esp_netif_destroy(ctx->netif);
        ctx->netif = nullptr;
        return esp_err_to_error(err);
    }

    err = esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &on_wifi_or_ip_event, ctx, &ctx->ipEventHandler);
    if (err != ESP_OK) {
        LOG_E(TAG, "Failed to register IP_EVENT handler: %s", esp_err_to_name(err));
        esp_event_handler_instance_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID, ctx->wifiEventHandler);
        ctx->wifiEventHandler = nullptr;
        esp_wifi_deinit();
        esp_netif_destroy(ctx->netif);
        ctx->netif = nullptr;
        return esp_err_to_error(err);
    }

    err = esp_wifi_set_mode(WIFI_MODE_STA);
    if (err != ESP_OK) {
        LOG_E(TAG, "esp_wifi_set_mode failed: %s", esp_err_to_name(err));
        esp_event_handler_instance_unregister(IP_EVENT, IP_EVENT_STA_GOT_IP, ctx->ipEventHandler);
        esp_event_handler_instance_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID, ctx->wifiEventHandler);
        ctx->ipEventHandler = nullptr;
        ctx->wifiEventHandler = nullptr;
        esp_wifi_deinit();
        esp_netif_destroy(ctx->netif);
        ctx->netif = nullptr;
        return esp_err_to_error(err);
    }

    err = esp_wifi_start();
    if (err != ESP_OK) {
        LOG_E(TAG, "esp_wifi_start failed: %s", esp_err_to_name(err));
        esp_event_handler_instance_unregister(IP_EVENT, IP_EVENT_STA_GOT_IP, ctx->ipEventHandler);
        esp_event_handler_instance_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID, ctx->wifiEventHandler);
        ctx->ipEventHandler = nullptr;
        ctx->wifiEventHandler = nullptr;
        esp_wifi_set_mode(WIFI_MODE_NULL);
        esp_wifi_deinit();
        esp_netif_destroy(ctx->netif);
        ctx->netif = nullptr;
        return esp_err_to_error(err);
    }

    // Power save, applied once the radio is up - the driver rejects the call before this point.
    //
    // IDF's default is WIFI_PS_MIN_MODEM, which wakes the radio once per beacon interval. On a link
    // that misses round trips that is not just latency: the health monitor below probes the gateway
    // every 5 s, and on a link this board sees with its antenna situation it concludes the connection
    // is dead and forces a reconnect every 15-20 seconds - tearing down the connection it exists to
    // protect, while the board stays associated with a healthy RSSI and its own web server goes
    // unreachable. CONFIG_TT_WIFI_POWER_SAVE turns IDF's default back on for boards where battery
    // life matters more; see that option's help text for the measurements behind the default.
    {
#if defined(CONFIG_TT_WIFI_POWER_SAVE)
        const wifi_ps_type_t power_save_mode = WIFI_PS_MIN_MODEM;
#else
        const wifi_ps_type_t power_save_mode = WIFI_PS_NONE;
#endif
        err = esp_wifi_set_ps(power_save_mode);
        if (err != ESP_OK) {
            LOG_W(TAG, "esp_wifi_set_ps(%d) failed: %s", (int)power_save_mode, esp_err_to_name(err));
        } else {
            LOG_I(TAG, "Wi-Fi power save: %s",
                power_save_mode == WIFI_PS_NONE ? "off (radio stays awake)" : "on (wakes per beacon)");
        }
    }

    // Start the health monitor (see wifiHealthTick). Created once; the netif is re-pointed on every
    // start so a stop/start cycle cannot leave the probe reading a destroyed netif.
    s_healthNetif = ctx->netif;
    s_healthFailures = 0;
    if (s_healthTimer == nullptr) {
        const esp_timer_create_args_t health_args = {
            .callback = wifiHealthTick,
            .arg = nullptr,
            .dispatch_method = ESP_TIMER_TASK,
            .name = "wifi_health",
        };
        if (esp_timer_create(&health_args, &s_healthTimer) == ESP_OK) {
            esp_timer_start_periodic(s_healthTimer, (uint64_t)HEALTH_PROBE_INTERVAL_S * 1000 * 1000);
        } else {
            LOG_W(WIFI_HEALTH_TAG, "WiFi health monitor timer could not be created");
            s_healthTimer = nullptr;
        }
    }

    mutex_lock(&ctx->mutex);
    ctx->radioState = WIFI_RADIO_STATE_ON;
    mutex_unlock(&ctx->mutex);

    WifiEvent radio_event = {};
    radio_event.type = WIFI_EVENT_TYPE_RADIO_STATE_CHANGED;
    radio_event.radio_state = WIFI_RADIO_STATE_ON;
    fire_event(ctx, radio_event);

    LOG_I(TAG, "WiFi radio on");
    return ERROR_NONE;
}

void bring_down_wifi(Esp32WifiCtx* ctx) {
    mutex_lock(&ctx->mutex);
    bool was_connected = ctx->stationState != WIFI_STATION_STATE_DISCONNECTED;
    bool was_scanning = ctx->scanning;
    ctx->stationState = WIFI_STATION_STATE_DISCONNECTED;
    ctx->scanning = false;
    mutex_unlock(&ctx->mutex);

    if (was_scanning) {
        esp_wifi_scan_stop();
    }
    if (was_connected) {
        esp_wifi_disconnect();
    }

    // Detach netif from the internal WiFi event handlers before stopping,
    // otherwise esp_netif_destroy() can race with esp_wifi_stop()'s own
    // netif teardown (see esp32_ble/WifiEsp.cpp for the same issue).
    if (ctx->netif != nullptr) {
        esp_wifi_clear_default_wifi_driver_and_handlers(ctx->netif);
    }

    esp_wifi_stop();
    esp_wifi_set_mode(WIFI_MODE_NULL);

    if (ctx->wifiEventHandler != nullptr) {
        esp_event_handler_instance_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID, ctx->wifiEventHandler);
        ctx->wifiEventHandler = nullptr;
    }
    if (ctx->ipEventHandler != nullptr) {
        esp_event_handler_instance_unregister(IP_EVENT, IP_EVENT_STA_GOT_IP, ctx->ipEventHandler);
        ctx->ipEventHandler = nullptr;
    }

    esp_wifi_deinit();

    if (ctx->netif != nullptr) {
        esp_netif_destroy(ctx->netif);
        ctx->netif = nullptr;
    }

    mutex_lock(&ctx->mutex);
    ctx->radioState = WIFI_RADIO_STATE_OFF;
    mutex_unlock(&ctx->mutex);

    WifiEvent radio_event = {};
    radio_event.type = WIFI_EVENT_TYPE_RADIO_STATE_CHANGED;
    radio_event.radio_state = WIFI_RADIO_STATE_OFF;
    fire_event(ctx, radio_event);

    LOG_I(TAG, "WiFi radio off");
}

// ---- WifiApi ----

error_t api_get_radio_state(Device* device, WifiRadioState* state) {
    auto* ctx = GET_CTX(device);
    if (ctx == nullptr || state == nullptr) return ERROR_INVALID_ARGUMENT;
    mutex_lock(&ctx->mutex);
    *state = ctx->radioState;
    mutex_unlock(&ctx->mutex);
    return ERROR_NONE;
}

error_t api_get_station_state(Device* device, WifiStationState* state) {
    auto* ctx = GET_CTX(device);
    if (ctx == nullptr || state == nullptr) return ERROR_INVALID_ARGUMENT;
    mutex_lock(&ctx->mutex);
    *state = ctx->stationState;
    mutex_unlock(&ctx->mutex);
    return ERROR_NONE;
}

error_t api_get_access_point_state(Device* device, WifiAccessPointState* state) {
    auto* ctx = GET_CTX(device);
    if (ctx == nullptr || state == nullptr) return ERROR_INVALID_ARGUMENT;
    // Access point mode isn't implemented by this driver.
    *state = WIFI_ACCESS_POINT_STATE_STOPPED;
    return ERROR_NONE;
}

bool api_is_scanning(Device* device) {
    auto* ctx = GET_CTX(device);
    if (ctx == nullptr) return false;
    mutex_lock(&ctx->mutex);
    bool scanning = ctx->scanning;
    mutex_unlock(&ctx->mutex);
    return scanning;
}

error_t api_scan(Device* device) {
    auto* ctx = GET_CTX(device);
    if (ctx == nullptr) return ERROR_INVALID_STATE;

    mutex_lock(&ctx->mutex);
    if (ctx->radioState != WIFI_RADIO_STATE_ON || ctx->scanning) {
        mutex_unlock(&ctx->mutex);
        return ERROR_INVALID_STATE;
    }
    mutex_unlock(&ctx->mutex);

    esp_err_t err = esp_wifi_scan_start(nullptr, false);
    if (err != ESP_OK) {
        return esp_err_to_error(err);
    }

    mutex_lock(&ctx->mutex);
    ctx->scanning = true;
    mutex_unlock(&ctx->mutex);

    WifiEvent event = {};
    event.type = WIFI_EVENT_TYPE_SCAN_STARTED;
    fire_event(ctx, event);
    return ERROR_NONE;
}

error_t api_get_scan_results(Device* device, WifiApRecord* results, size_t* num_results) {
    auto* ctx = GET_CTX(device);
    if (ctx == nullptr || results == nullptr || num_results == nullptr) return ERROR_INVALID_ARGUMENT;

    mutex_lock(&ctx->mutex);
    size_t count = std::min<size_t>(*num_results, ctx->scanResultCount);
    for (size_t i = 0; i < count; i++) {
        const wifi_ap_record_t& src = ctx->scanResults[i];
        WifiApRecord& dst = results[i];
        memset(dst.ssid, 0, sizeof(dst.ssid));
        memcpy(dst.ssid, src.ssid, std::min(sizeof(dst.ssid) - 1, sizeof(src.ssid)));
        dst.rssi = src.rssi;
        dst.channel = src.primary;
        memcpy(dst.bssid, src.bssid, sizeof(dst.bssid));
        dst.authentication_type = to_wifi_authentication_type(src.authmode);
    }
    *num_results = count;
    mutex_unlock(&ctx->mutex);
    return ERROR_NONE;
}

error_t api_station_get_ipv4_address(Device* device, char* ipv4) {
    auto* ctx = GET_CTX(device);
    if (ctx == nullptr || ipv4 == nullptr) return ERROR_INVALID_ARGUMENT;
    mutex_lock(&ctx->mutex);
    esp_ip4addr_ntoa(&ctx->ipInfo.ip, ipv4, 16);
    mutex_unlock(&ctx->mutex);
    return ERROR_NONE;
}

error_t api_station_get_target_ssid(Device* device, char* ssid) {
    auto* ctx = GET_CTX(device);
    if (ctx == nullptr || ssid == nullptr) return ERROR_INVALID_ARGUMENT;
    mutex_lock(&ctx->mutex);
    constexpr size_t ssid_buffer_size = sizeof(ctx->targetSsid);
    strncpy(ssid, ctx->targetSsid, ssid_buffer_size);
    mutex_unlock(&ctx->mutex);
    return ERROR_NONE;
}

error_t api_station_connect(Device* device, const char* ssid, const char* password, int32_t channel) {
    auto* ctx = GET_CTX(device);
    if (ctx == nullptr || ssid == nullptr) return ERROR_INVALID_ARGUMENT;

    mutex_lock(&ctx->mutex);
    bool radio_on = ctx->radioState == WIFI_RADIO_STATE_ON;
    bool was_connected = ctx->stationState != WIFI_STATION_STATE_DISCONNECTED;
    mutex_unlock(&ctx->mutex);

    if (!radio_on) {
        return ERROR_INVALID_STATE;
    }

    if (was_connected) {
        esp_wifi_disconnect();
    }

    wifi_config_t config {};
    config.sta.channel = static_cast<uint8_t>(channel);
    config.sta.scan_method = WIFI_FAST_SCAN;
    config.sta.sort_method = WIFI_CONNECT_AP_BY_SIGNAL;
    config.sta.threshold.rssi = -127;
    config.sta.pmf_cfg.capable = true;
    strncpy(reinterpret_cast<char*>(config.sta.ssid), ssid, sizeof(config.sta.ssid) - 1);
    if (password != nullptr && password[0] != '\0') {
        strncpy(reinterpret_cast<char*>(config.sta.password), password, sizeof(config.sta.password) - 1);
        config.sta.threshold.authmode = WIFI_AUTH_WPA2_PSK;
    }

    esp_err_t err = esp_wifi_set_config(WIFI_IF_STA, &config);
    if (err != ESP_OK) {
        return esp_err_to_error(err);
    }

    mutex_lock(&ctx->mutex);
    strncpy(ctx->targetSsid, ssid, sizeof(ctx->targetSsid) - 1);
    ctx->targetSsid[sizeof(ctx->targetSsid) - 1] = '\0';
    ctx->stationState = WIFI_STATION_STATE_CONNECTION_PENDING;
    mutex_unlock(&ctx->mutex);

    err = esp_wifi_connect();
    if (err != ESP_OK) {
        mutex_lock(&ctx->mutex);
        ctx->stationState = WIFI_STATION_STATE_DISCONNECTED;
        mutex_unlock(&ctx->mutex);
        return esp_err_to_error(err);
    }

    WifiEvent event = {};
    event.type = WIFI_EVENT_TYPE_STATION_STATE_CHANGED;
    event.station_state = WIFI_STATION_STATE_CONNECTION_PENDING;
    fire_event(ctx, event);
    return ERROR_NONE;
}

error_t api_station_disconnect(Device* device) {
    auto* ctx = GET_CTX(device);
    if (ctx == nullptr) return ERROR_INVALID_STATE;

    mutex_lock(&ctx->mutex);
    bool was_connected = ctx->stationState != WIFI_STATION_STATE_DISCONNECTED;
    mutex_unlock(&ctx->mutex);

    if (!was_connected) {
        return ERROR_NONE;
    }

    // The DISCONNECTED state change is published by the WIFI_EVENT handler
    // once ESP-IDF confirms the disconnect, so we don't fire it here.
    esp_err_t err = esp_wifi_disconnect();
    return err == ESP_OK ? ERROR_NONE : esp_err_to_error(err);
}

error_t api_station_get_rssi(Device* device, int32_t* rssi) {
    auto* ctx = GET_CTX(device);
    if (ctx == nullptr || rssi == nullptr) return ERROR_INVALID_ARGUMENT;

    int native_rssi = 0;
    esp_err_t err = esp_wifi_sta_get_rssi(&native_rssi);
    if (err != ESP_OK) {
        return esp_err_to_error(err);
    }
    *rssi = native_rssi;
    return ERROR_NONE;
}

error_t api_set_promiscuous(Device* device, bool enable) {
    auto* ctx = GET_CTX(device);
    if (ctx == nullptr) return ERROR_INVALID_STATE;

    esp_err_t err = esp_wifi_set_promiscuous(enable);
    if (err != ESP_OK) {
        return esp_err_to_error(err);
    }
    if (enable) {
        // Management, data and misc - deliberately NOT control.
        //
        // WIFI_PROMIS_FILTER_MASK_ALL also forwards control frames, and on any real channel that
        // means an ACK for every acknowledged frame: thousands per second, each delivered to the
        // app callback from the driver's own receive path, where each one is copied twice. That work
        // competes with the driver's handling of beacons, and the station then times out on them and
        // drops the link - observed consistently about nine seconds after the sniffer is enabled,
        // with the channel uninvolved and internal memory healthy (43 KB free, no low-memory warning).
        // Nothing the app needs is a control frame: EAPOL/PMKID are data, and deauth/assoc/beacons are
        // management. The previously stated reason for ALL - observing ACK/RTS noise for the
        // association-sleep attack - is not worth losing the association over, and can be revisited
        // with a narrower filter if that feature ever needs it.
        // All three data masks, not just DATA: MPDU and AMPDU are documented as "a kind of
        // WIFI_PKT_DATA", and EAPOL frequently arrives aggregated, so DATA alone silently stops
        // delivering the frames this app exists to capture - which showed up as EAPOL, PMKID and
        // deauth all counting zero while packets were still being received.
        wifi_promiscuous_filter_t filter {
            .filter_mask = WIFI_PROMIS_FILTER_MASK_MGMT | WIFI_PROMIS_FILTER_MASK_DATA |
                WIFI_PROMIS_FILTER_MASK_DATA_MPDU | WIFI_PROMIS_FILTER_MASK_DATA_AMPDU |
                WIFI_PROMIS_FILTER_MASK_MISC
        };
        esp_wifi_set_promiscuous_filter(&filter);
    }
    return ERROR_NONE;
}

error_t api_get_promiscuous(Device* device, bool* enabled) {
    auto* ctx = GET_CTX(device);
    if (ctx == nullptr || enabled == nullptr) return ERROR_INVALID_ARGUMENT;

    esp_err_t err = esp_wifi_get_promiscuous(enabled);
    return err == ESP_OK ? ERROR_NONE : esp_err_to_error(err);
}

error_t api_set_promiscuous_callback(Device* device, WifiPromiscuousCallback callback, void* context) {
    auto* ctx = GET_CTX(device);
    if (ctx == nullptr) return ERROR_INVALID_STATE;

    // Route the caller's callback/context through the file-scope trampoline. Even when `callback`
    // is null (unregister), keep the trampoline registered and let it no-op - avoiding any concern
    // about whether esp_wifi accepts a null rx cb.
    g_promiscuousCallback = callback;
    g_promiscuousContext = context;

    esp_err_t err = esp_wifi_set_promiscuous_rx_cb(promiscuous_rx_cb);
    return err == ESP_OK ? ERROR_NONE : esp_err_to_error(err);
}

error_t api_set_channel(Device* device, uint8_t channel) {
    auto* ctx = GET_CTX(device);
    if (ctx == nullptr) return ERROR_INVALID_STATE;

    esp_err_t err = esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE);
    return err == ESP_OK ? ERROR_NONE : esp_err_to_error(err);
}

error_t api_station_get_bssid(Device* device, uint8_t* bssid) {
    auto* ctx = GET_CTX(device);
    if (ctx == nullptr || bssid == nullptr) return ERROR_INVALID_ARGUMENT;

    // The driver already has this - it logs the BSSID when the station associates - it was simply
    // never exposed. Addressing a deauth at the associated access point needs it, and taking it
    // from the connection is what stops the target drifting between cells heard on the same channel.
    wifi_ap_record_t ap_info = {};
    esp_err_t err = esp_wifi_sta_get_ap_info(&ap_info);
    if (err != ESP_OK) return esp_err_to_error(err);

    memcpy(bssid, ap_info.bssid, 6);
    return ERROR_NONE;
}

error_t api_send_raw_frame(Device* device, const uint8_t* frame, size_t length) {
    auto* ctx = GET_CTX(device);
    if (ctx == nullptr || frame == nullptr) return ERROR_INVALID_ARGUMENT;
    if (length < 24 || length > 1500) return ERROR_INVALID_ARGUMENT;

    // esp_wifi_80211_tx sends raw 802.11 frames (beacon/probe/action + non-QoS data). en_sys_seq=true
    // works whether or not the STA is connected, so beacon/probe injection is unaffected by the
    // radio's association state. (Deauth frames are rejected by the stock WiFi library's sanity
    // check, so they silently don't transmit here - see the WifiToolbox notes.)
    esp_err_t err = esp_wifi_80211_tx(WIFI_IF_STA, frame, static_cast<int>(length), true);
    return err == ESP_OK ? ERROR_NONE : esp_err_to_error(err);
}

error_t api_set_radio_on(Device* device) {
    auto* ctx = GET_CTX(device);
    if (ctx == nullptr) return ERROR_INVALID_STATE;

    mutex_lock(&ctx->mutex);
    bool already_on = ctx->radioState == WIFI_RADIO_STATE_ON;
    mutex_unlock(&ctx->mutex);
    if (already_on) return ERROR_NONE;

    return bring_up_wifi(ctx);
}

error_t api_set_radio_off(Device* device) {
    auto* ctx = GET_CTX(device);
    if (ctx == nullptr) return ERROR_INVALID_STATE;

    mutex_lock(&ctx->mutex);
    bool already_off = ctx->radioState == WIFI_RADIO_STATE_OFF;
    mutex_unlock(&ctx->mutex);
    if (already_off) return ERROR_NONE;

    bring_down_wifi(ctx);
    return ERROR_NONE;
}

error_t api_event_subscribe(Device* device, WifiEventSubscription* sub, TaskEventGroup* event_group) {
    auto* ctx = GET_CTX(device);
    if (ctx == nullptr || sub == nullptr || event_group == nullptr) return ERROR_INVALID_ARGUMENT;

    uint32_t bit;
    error_t claim_result = task_event_group_claim_bit(event_group, &bit);
    if (claim_result != ERROR_NONE) {
        return claim_result;
    }

    mutex_lock(&ctx->subscriptionsMutex);

    // Avoid cyclic subscription list that would loop forever
    for (WifiEventSubscription* existing = ctx->subscriptions; existing != nullptr; existing = existing->internal.next) {
        if (existing == sub) {
            mutex_unlock(&ctx->subscriptionsMutex);
            task_event_group_release_bit(event_group, bit);
            return ERROR_INVALID_STATE;
        }
    }

    sub->internal.event_group = event_group;
    sub->bit = bit;
    sub->internal.next = ctx->subscriptions;
    ctx->subscriptions = sub;
    mutex_unlock(&ctx->subscriptionsMutex);
    return ERROR_NONE;
}

error_t api_event_unsubscribe(Device* device, WifiEventSubscription* sub) {
    auto* ctx = GET_CTX(device);
    if (ctx == nullptr || sub == nullptr) return ERROR_INVALID_ARGUMENT;

    error_t result = ERROR_NOT_FOUND;
    mutex_lock(&ctx->subscriptionsMutex);
    for (WifiEventSubscription** link = &ctx->subscriptions; *link != nullptr; link = &(*link)->internal.next) {
        if (*link == sub) {
            *link = sub->internal.next;
            result = ERROR_NONE;
            break;
        }
    }
    mutex_unlock(&ctx->subscriptionsMutex);

    if (result == ERROR_NONE) {
        task_event_group_release_bit(sub->internal.event_group, sub->bit);
    }
    return result;
}

error_t api_get_firmware_ops(Device* /*device*/, const FirmwareOps** ops, void** ctx) {
    // ops/ctx are caller-supplied output pointers, reachable from external (ELF) apps via
    // wifi_get_firmware_ops() - validate at this API boundary rather than trusting the caller.
    if (ops == nullptr || ctx == nullptr) {
        return ERROR_INVALID_ARGUMENT;
    }
#if defined(CONFIG_ESP_HOSTED_ENABLED)
    // Only meaningful on a hosted board (P4+C6/C5 etc.) - this wifi device is backed by a real
    // co-processor with its own updatable firmware there. On a native (non-hosted) chip, this
    // device's "radio" is the chip's own built-in WiFi, nothing to update via this interface.
    *ops = esp32_esp_hosted_ota_get_ops();
    *ctx = nullptr; // esp32_esp_hosted_ota's FirmwareOps functions are all singleton/global, no per-call ctx needed
    return ERROR_NONE;
#else
    return ERROR_NOT_SUPPORTED;
#endif
}

const WifiApi esp32_wifi_api = {
    .set_radio_on = api_set_radio_on,
    .set_radio_off = api_set_radio_off,
    .get_radio_state = api_get_radio_state,
    .get_station_state = api_get_station_state,
    .get_access_point_state = api_get_access_point_state,
    .is_scanning = api_is_scanning,
    .scan = api_scan,
    .get_scan_results = api_get_scan_results,
    .station_get_ipv4_address = api_station_get_ipv4_address,
    .station_get_target_ssid = api_station_get_target_ssid,
    .station_get_bssid = api_station_get_bssid,
    .station_connect = api_station_connect,
    .station_disconnect = api_station_disconnect,
    .station_get_rssi = api_station_get_rssi,
    .event_subscribe = api_event_subscribe,
    .event_unsubscribe = api_event_unsubscribe,
    .get_firmware_ops = api_get_firmware_ops,
    .set_promiscuous = api_set_promiscuous,
    .get_promiscuous = api_get_promiscuous,
    .set_promiscuous_callback = api_set_promiscuous_callback,
    .set_channel = api_set_channel,
    .send_raw_frame = api_send_raw_frame
};

error_t start_device(Device* device) {
    // Prefers PSRAM/SPIRAM, falling back to internal RAM when unavailable - aborts on true OOM
    // (see OptExternalAllocator's own doc), so no null check here.
    tt::OptExternalAllocator<Esp32WifiCtx> allocator;
    auto* ctx = allocator.allocate(1);
    new (ctx) Esp32WifiCtx();

    ctx->device = device;
    mutex_construct(&ctx->mutex);
    mutex_construct(&ctx->subscriptionsMutex);
    device_set_driver_data(device, ctx);

    return ERROR_NONE;
}

error_t stop_device(Device* device) {
    auto* ctx = GET_CTX(device);
    if (ctx == nullptr) return ERROR_NONE;

    if (ctx->radioState == WIFI_RADIO_STATE_ON) {
        bring_down_wifi(ctx);
    }

    // Release any subscribers that never unsubscribed: device_stop() doesn't wait for apps still
    // using this device, so a later wifi_event_unsubscribe() would find no ctx and skip releasing
    // the bit.
    mutex_lock(&ctx->subscriptionsMutex);
    for (WifiEventSubscription* sub = ctx->subscriptions; sub != nullptr;) {
        WifiEventSubscription* next = sub->internal.next;
        // Force-close, don't destruct: a concurrent wifi_event_poll() may hold/await this same
        // lock (see WifiEventSubscription::internal::closed).
        mutex_lock(&sub->internal.ring_mutex);
        sub->internal.closed = true;
        mutex_unlock(&sub->internal.ring_mutex);
        task_event_group_release_bit(sub->internal.event_group, sub->bit);
        sub = next;
    }
    ctx->subscriptions = nullptr;
    mutex_unlock(&ctx->subscriptionsMutex);

    device_set_driver_data(device, nullptr);
    mutex_destruct(&ctx->subscriptionsMutex);
    mutex_destruct(&ctx->mutex);
    ctx->~Esp32WifiCtx();
    tt::OptExternalAllocator<Esp32WifiCtx>().deallocate(ctx, 1);

    return ERROR_NONE;
}

} // namespace

extern "C" {

extern Module platform_esp32_module;

Driver esp32_wifi_driver = {
    .name = "esp32_wifi",
    .compatible = (const char*[]) { "espressif,esp32-wifi", nullptr },
    .start_device = start_device,
    .stop_device = stop_device,
    .api = (const void*)&esp32_wifi_api,
    .device_type = &WIFI_TYPE,
    .owner = &platform_esp32_module,
    .internal = nullptr
};

} // extern "C"

#endif // CONFIG_SOC_WIFI_SUPPORTED or CONFIG_SLAVE_SOC_WIFI_SUPPORTED
