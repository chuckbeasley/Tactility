# Wi-Fi Toolbox on ESP-Brookesia — implementation spec (ESP32-P4 host + ESP32-C6 Wi-Fi co-processor)

Status: draft for review. Scope: reimplementing the three Wi-Fi Toolbox tools (PMKID/EAPOL capture,
frame injection, Wi-Fi + net utilities) as an ESP-Brookesia product on a **two-chip** target — an
ESP32-P4 running the application with no radio of its own, and an **ESP32-C6 co-processor** providing
Wi-Fi over esp-hosted.

The two-chip split is the defining constraint of this document: every radio operation is an RPC away
from the code that wants it, and the two operations this toolbox exists for — monitor-mode capture and
raw frame injection — are the two the RPC layer is least likely to carry. Section 3 decides where each
piece must live; section 4 is the design that follows from it.

Everything marked "measured" was measured on this tree's ESP32-C5 board (single-chip, native Wi-Fi) and
must be re-measured on the P4+C6 pair, because the RPC path changes the numbers. Everything marked
"verify" is an open question, not an assumption.

---

## 1. Feature parity target

The three tools exist today in `WifiToolbox.cpp` (~1850 lines of LVGL + ESP-IDF). The radio half is
portable in principle; the question this spec answers is *where* it runs.

| Tool | Behaviour to keep | Reusable code | Where it must run |
| --- | --- | --- | --- |
| PMKID / EAPOL capture | promiscuous 802.11 capture to PCAP, channel hop or fixed, optional deauth to force a handshake, live counters | `onPacket()`, `isEapolFrame()`, `looksLikePmkid()`, `hopChannel()`, PCAP record handling | capture callback on the **C6**; PCAP writing on the **P4** (the card is on the P4) |
| Frame injection | 10 modes (Beacon spam, Probe flood, Deauth burst, Auth flood, Assoc flood, Assoc sleep, Karma, Disassoc, Client→AP deauth, NAV/Duration) with SSID / BSSID-to-spoof / channel | eight frame builders (`buildBeacon`, `buildProbeRequest`, `buildDeauth`, `buildAuth`, `buildAssocRequest`, `buildProbeResponse`, `buildDisassoc`, `buildClientDeauth`, `buildNav`), `randomiseMac()`, the 10-mode engine | **C6** — `esp_wifi_80211_tx()` is a radio-driver call and cannot be emulated from the host |
| Wi-Fi + net utilities | ARP/host sweep, SSH(22) and Telnet(23) checks, preset port scans (Common/Web/IoT-SCADA/Windows-SMB), target IP or the device itself | `probeHost()`, `startNetScan()`, `kPortPresets` | **P4** — plain lwIP sockets, no radio involvement |

Two gaps in the current implementation to fix during the port rather than copy: there is **no
authorisation notice** anywhere in the app (section 10), and the injection modes are reachable without an
arming step.

## 2. Target platform

- **Host: ESP32-P4** — dual-core RISC-V, MIPI-DSI/CSI, USB, large PSRAM, **no Wi-Fi and no Bluetooth of
  its own**.
- **Co-processor: ESP32-C6** — Wi-Fi 6 + BLE + 802.15.4, running esp-hosted's co-processor firmware.
- **Transport: SDIO 4-bit.** This tree already has three P4 profiles (`generic-esp32p4`,
  `guition-jc1060p470ciwy`, `m5stack-tab5`); take **`m5stack-tab5` as the reference board**, because its
  profile is exactly this pairing and is fully configured: `CONFIG_SLAVE_IDF_TARGET_ESP32C6=y`,
  `CONFIG_ESP_HOSTED_ENABLED=y`, `CONFIG_ESP_HOSTED_SDIO_HOST_INTERFACE=y`, slot 1, CMD 13 / CLK 12 /
  D0 11 / D1 10 / D2 9 / D3 8, slave reset on GPIO 15, and `CONFIG_ESP_HOSTED_USE_MEMPOOL=n` (recorded
  there as a fix for recent esp-hosted changes).
- **Framework**: Brookesia `master` (v0.8) — the only line whose ESP-IDF range covers IDF 6.x. Components
  confirmed on the registry while writing this: `brookesia_service_wifi`, `brookesia_service_custom`,
  `brookesia_hal_boards` (all "supports all targets"); the remaining identifiers and versions come from
  the registry at M0, since the componentization is new and names are still moving.
- **Reference software already in this tree, reuse rather than reinvent**:
  - `esp32_esp_hosted_ota.cpp` — flashes the **slave's** firmware from the host. This is how a patched
    C6 image reaches the co-processor in the field (section 5.4).
  - `EspNowBackendHosted.cpp` — registers **six custom-RPC handlers** and needed
    `CONFIG_ESP_HOSTED_MAX_CUSTOM_MSG_HANDLERS=8` (default cap 3). The toolbox's host↔slave channel is
    the same mechanism, already proven here.
  - the platform Wi-Fi driver's event **deduplication** for esp-hosted (the RPC layer has been observed
    delivering the identical event twice in a row on this transport) — a capture/injection state machine
    must not treat a duplicated `ScanApInfosUpdated` or `GeneralEventHappened` as two events.

## 3. The architectural decision: what crosses the RPC boundary

Host-side `esp_wifi_remote` gives the P4 transparent `esp_wifi_*()` calls by generating declarations
from `esp_wifi.h` and translating them into RPCs, so **the toolbox's radio calls compile on the host
either way**. Whether they *work* depends on the co-processor's RPC coverage, and that is the first
thing to establish:

| Capability | Expected over stock esp-hosted RPC | Plan |
| --- | --- | --- |
| scan / connect / event delivery | yes, that is the component's purpose | use `brookesia_service_wifi` (which sits on `esp_wifi_remote` on this target) |
| `esp_wifi_set_promiscuous` + RX callback | **verify** — data-path frames do cross the transport, but promiscuous delivery is not a station-mode flow | if supported, capture directly from the host; if not, slave-side feature (below) |
| `esp_wifi_80211_tx()` with an arbitrary frame | **verify** — least likely of the three, and the one with a driver patch attached | assume **not** until proven; design for slave-side |

**Design assumption (to be confirmed at M0): the radio-side work runs on the C6.** Concretely:

1. A **slave-side toolbox feature** (`eh_cp_feat_wifi_toolbox` in esp-hosted's co-processor tree, or a
   small component alongside it) that owns promiscuous capture and raw TX. Not captive: it must
   implement the parts of the current `WifiToolbox.cpp` that touch the driver — the frame builders, the
   injection engine, the promiscuous callback and its filters.
2. A **custom-RPC channel** between host and slave, modelled on this tree's ESP-NOW bridge:
   - host→slave: `InjectStart(mode, ssid, bssid, client, channel, duration_ms)`, `InjectStop`,
     `CaptureStart(hop|fixed, channel, elicit_deauth)`, `CaptureStop`, `GetStats`;
   - slave→host: `StatsUpdate`, `CaptureFrame(ts, channel, rssi, payload)` for the **filtered** frames
     worth writing, `StateChanged`, `Error(code)`.
3. **Filter on the slave, always.** Sending every captured frame across SDIO would waste the transport
   and the P4's cycles on frames nobody asked for; the slave decides what is interesting (`EAPOL`,
   `PMKID` candidates, deauth/disassoc, beacons and probes for the monitor view) and only those cross.
   This mirrors the existing rule for the promiscuous callback: it must stay allocation-free and cheap.
4. **The libnet80211 patch moves to the C6.** The one-byte fix in
   `ieee80211_raw_frame_sanity_check` (documented in `DeauthPatch.md`, including the trap that
   `objcopy --update-section` corrupts the object) has to be re-derived for the C6's IDF version and its
   instruction layout; the host-side patch becomes irrelevant. `patch_deauth_libnet.py` is the starting
   point, and its CI check (fail loudly when the symbol layout changes) now guards the co-processor
   firmware instead.

If M0 shows that promiscuous mode *is* carried over the RPC, capture may move host-side for a cheaper
build — but injection stays on the C6 either way, so the custom-RPC channel is needed regardless. That
asymmetry is why the design assumes the slave-side route from the start.

## 4. Service design (host side)

### 4.1 Component

`brookesia_service_wifi_toolbox`, built on the pattern of `brookesia_service_custom`: a
`ServiceBase`-derived service registering functions and events with JSON schemas, in the same style as
`brookesia_service_wifi` (which also brings local calls, events and TCP/JSON RPC), plus a type-safe
helper in the `esp_brookesia::service::helper` style.

It is the *only* thing that talks to the co-processor's toolbox feature. The UI, the console example and
(selectively) an agent go through it.

### 4.2 Functions

Schemas follow the Wi-Fi service's conventions (`name`, `description`, `require_scheduler`,
`parameters[]`, `return_value`).

| Function | Parameters | Returns | Notes |
| --- | --- | --- | --- |
| `ArmToolbox` | `Mode` (`Capture`\|`Inject`), `Acknowledged` (Boolean) | – | required before any capture/injection; RAM-only flag (section 10) |
| `DisarmToolbox` | – | – | stops everything, releases the radio lease, disarms |
| `CaptureStart` | `Channel` (`hop`\|`fixed` + value), `ElicitDeauth` (Boolean), `Target` (`bssid`, `client_mac`), `Path` (optional) | `SessionId` | takes the lease, disables power save (storing the old value), tells the slave to start, opens the PCAP writer on the P4 |
| `CaptureStop` | – | `Stats` | flushes the PCAP, restores power save, releases the lease |
| `CaptureStats` | – | `Stats` | packets, EAPOL, PMKID, deauth, dropped, KB, channel, slave tx ok/fail/err |
| `InjectStart` | `Mode` (one of ten), `SSID`, `BSSID`, `ClientMac`, `Channel`, `DurationMs` | `SessionId` | duration mandatory and bounded; slave-side timer ends it even if the host stops answering |
| `InjectStop` | – | `Stats` | |
| `InjectStats` | – | `Stats` | frames sent, failures, channel, elapsed |
| `NetScanStart` | `Mode` (`Arp`\|`Ssh`\|`Telnet`\|`PortPreset`), `Preset`, `Target` | `ScanId` | host-side sockets; requires `Wifi.GetGeneralState == Connected` |
| `NetScanStop` / `NetScanResults` | – | – / Array | |
| `ListCaptures` / `RemoveCapture` | – / `Name` | Array / – | PCAP files on the P4's SD card |
| `SlaveToolboxInfo` | – | Object | slave firmware version, patch status, supported modes — so the UI can say "this co-processor cannot inject" instead of failing at Start |

Deliberately *not* exposed: a `SetPowerSave` function. Power save is a system policy with a default, and
these sessions borrow and return it; exposing it invites the "left it off" class of bug we already fixed
once on the single-chip build.

### 4.3 Events

`CaptureStateChanged`, `CaptureCounters` (~1 Hz), `InjectStateChanged`, `InjectCounters` (~1 Hz),
`NetScanResultAdded`, `NetScanStateChanged`, `SlaveLinkStateChanged`, `ToolboxError(code, message)`.

Counters are events rather than polling, so the UI binds to them and the service stays the only writer.
`SlaveLinkStateChanged` exists because on this target the transport itself can drop — and a capture that
"silently stops writing" is exactly what a broken SDIO link looks like from the host.

### 4.4 Handler sketch

```cpp
// Host side. Every handler that touches the radio goes through the lease and the slave channel; nothing
// here calls esp_wifi_80211_tx() directly, because the radio is on the other chip.
bool WifiToolboxService::on_init() {
    register_function(
        FunctionSchema{
            .name = "InjectStart",
            .description = "Transmit a frame pattern from the co-processor's radio.",
            .require_scheduler = true,
            .parameters = { /* Mode, SSID, BSSID, ClientMac, Channel, DurationMs */ },
            .return_value = { .type = "String", .description = "Session id" },
        },
        [this](FunctionParameterMap params) -> FunctionResult {
            if (!armed_ || armed_mode_ != Mode::Inject) return error("not armed");
            if (!radio_lease_.acquire(RadioUser::Injection)) return error("radio busy");
            if (!slave_.supports_mode(params.at("Mode"))) return error("unsupported on this co-processor");
            power_save_was_enabled_ = wifi_service_.is_power_save_enabled();
            wifi_service_.set_power_save_enabled(false);
            return slave_.inject_start(params);   // custom RPC; the slave arms its own duration timer
        });
    register_event(EventSchema{ .name = "InjectCounters", /* items */ });
    return true;
}
```

## 5. Radio ownership, now spanning two chips

On the single-chip build the toolbox could simply take the radio. Here there are three owners to
coordinate: Brookesia's Wi-Fi service on the host, the esp-hosted transport, and the co-processor's own
Wi-Fi stack.

Rules:

1. **One lease, named holder.** The lease lives in the toolbox service and is the only way to reach the
   slave's toolbox feature. A second requester (a future spectrum view) asks the service instead of
   opening its own RPC channel.
2. **Taking the lease**: `Wifi.TriggerGeneralAction("Disconnect")` → wait for
   `GeneralEventHappened(Disconnected)` → *then* reconfigure. Do not race the service's state machine,
   and remember the transport can deliver that event twice (dedup as the platform driver already does).
   Capture with channel hopping may keep the station connected; **fixed-channel capture and all
   injection disconnect**, because a fixed channel moves the radio off the AP.
3. **Releasing the lease**: reconnect only if the station was connected when the lease was taken, and
   verify with `GetGeneralState` — through the same staged recovery the Tactility build uses
   (re-associate → verify → re-join from a fresh scan → verify → back off), not a fire-and-forget
   reconnect. On this target an RPC-layer hiccup during the reconnect is a second failure mode to
   tolerate.
4. **The slave is authoritative about its own state.** If the C6 reboots (watchdog, crash, esp-hosted
   OTA) mid-session, the host must notice (`SlaveLinkStateChanged`), stop the session, clear the armed
   flag, and re-establish the Wi-Fi service — a session that assumes the slave still holds its promiscuous
   filter is worse than one that stops.
5. **Maximum hold time** (default 10 minutes, configurable down to one) enforced on *both* sides: the
   host disarms, and the slave stops transmitting on its own timer even if the host has gone away. The
   slave-side timer is the safety net; the host-side one is the user-facing limit.

## 6. User interface

Brookesia's GUI is declarative (JSON UI documents resolved by the LVGL backend), so the screens are
documents plus action bindings rather than the imperative `showXScreen()` functions used today. The
reference board's 5-inch 1280×720 panel also means the current 320×480 layouts are re-laid-out, not
scaled.

- **Main**: three entries (PMKID / EAPOL Capture, Frame Injection, WiFi + Net Utilities) plus a
  co-processor status line from `SlaveToolboxInfo` ("C6 toolbox: v1.2, injection available").
- **Capture**: status, PCAP path, channel selector (hop, or a channel list from the *slave's* regulatory
  configuration), "Elicit deauth to force handshake" with target AP and optional client MAC, Start/Stop,
  counters bound to `CaptureCounters`.
- **Inject**: the ten modes with the conditional fields the current screen already implements (SSID only
  for the modes that build it, client MAC only for the targeted ones), channel, mandatory duration, an
  **armed** banner, Start/Stop.
- **Network**: mode, preset, optional target IP, Start, result list.

Additions the current UI lacks and this target makes necessary:

- an **arming dialog** naming what will be transmitted, on which channel, for how long, and from which
  chip;
- a persistent **"injecting"** indicator in the status bar (`brookesia_system_super`) for as long as a
  session is live — a screen the user navigated away from must not hide an active transmitter;
- a **transport warning** when `SlaveLinkStateChanged` says the co-processor is unreachable while a
  session was running: "the capture stopped because the co-processor link dropped", not a silent stop;
- empty-state honesty: "looking for networks…" while retrying a starved scan, "no networks found" only
  after the retries are spent (measured on the C5: scans issued while associated return zero records
  often enough that the first empty result means nothing).

## 7. Application packaging, storage and retrieval

- The UI app must be **native** — an ELF-runtime package or built into the system image. The scripting
  runtimes cannot reach promiscuous mode or raw TX, and on this target those live *two chips away*, which
  no scripting layer can bridge.
- **PCAPs are written on the P4** (the SD card is there), from frames streamed by the slave. Files go to
  `/sdcard/captures` with size-based rotation (~64 MB) and a free-space floor that stops the session
  before the card fills. `brookesia_service_storage` is namespace key-value storage, not a place for
  PCAPs.
- **Retrieval**: `brookesia_service_usb` file transfer, plus the Files app on device. Verify the PCAP
  link type opens in Wireshark/tcpdump before M3 — a wrong link type makes every capture look corrupt to
  the people who will open them.
- **Co-processor firmware** is part of the product image: pin the C6 firmware version, ship the patched
  build, and update it from the host through the existing esp-hosted OTA driver rather than asking users
  to flash two chips by hand.

## 8. Agent / MCP exposure

`brookesia_mcp_utils` can expose functions as MCP tools. Read-only half only: scan results, capture
status, capture listing, net-utility results, `SlaveToolboxInfo`. **Never** `InjectStart`,
`CaptureStart` with elicitation, or `DisarmToolbox`: an agent must not be able to start a transmitter on
someone's network because a prompt asked for it. Agent-driven testing, if ever wanted, goes through
`ArmToolbox` with a physical confirmation and a separate build flag.

## 9. Non-functional requirements

- **Transport budget.** Every control operation is an RPC round trip over SDIO; measurements from the
  single-chip build (≈5 ms control-plane latency, 90 ms power-save penalty) do not transfer. Establish
  numbers in M0: RPC latency, capture frame throughput without disturbing the station link, and the CPU
  cost on the C6 of filtering at full rate.
- **Slave load.** The C6 has one radio shared with Bluetooth and (on some products) 802.15.4. A capture
  session competes with audio streaming and the HCI link, so the slave filter must be cheap and the
  session must be able to run at reduced capture rates before it drops frames silently.
- **Power.** Power save is a property of the C6's modem and the esp-hosted power-save features; sessions
  that need low latency disable it on the slave (through the Wi-Fi service, which owns that policy) and
  restore what they found. *The ~90 ms figure measured on the C5 must be re-measured here* — with an RPC
  hop in the path the cost is likely higher, and the number belongs in this document before M3.
- **No leftovers, ever.** Nothing radio-active runs at boot: no scans, no responder probes, no timing
  sessions. On the single-chip build a leftover one-shot FTM probe — three unanswered sessions per boot —
  caused a fault that looked exactly like an AP problem (station associated at −42 dBm, beacons arriving,
  no data for 18–95 s, only re-association restoring it) and survived attempts to blame buffers,
  aggregation, power save and the AP itself. On a two-chip product the same mistake would also take down
  Bluetooth and any hosted networking, so the rule is absolute: sessions start only when asked, and stop
  on app exit, lease loss, disarm, duration expiry, link loss, or reboot.
- **Error taxonomy**: `ToolboxError` with stable codes — not armed, radio busy, unsupported by slave
  firmware, slave unreachable, driver rejected frame, SD full, station failed to reconnect. The UI has to
  explain failures; the log cannot be the only record.

## 10. Safety and compliance

1. First use of either active tool shows a notice: capture and injection are for networks you own or are
   explicitly authorised to test; deauth and disassoc disrupt other people's connections.
2. Injection and deauth elicitation require the arming step, which names mode, channel, duration and the
   transmitting chip; default duration 30 s, hard maximum bounded.
3. Nothing resumes after a reboot: the armed flag is RAM-only, and the slave's session state is cleared
   on link establishment.
4. A visible indicator for as long as a session is live (section 6).
5. Feature-flagged build: `CONFIG_TOOLBOX_ALLOW_INJECTION` off in shipped products; capture and net
   utilities stay available. Because the injecting code now lives in co-processor firmware, the flag has
   a **second** build: the shipped C6 image must be built without the toolbox feature, or with injection
   compiled out — a host-side flag alone would be a false sense of safety.

## 11. Verification plan (exit criteria)

| Area | Check | Pass condition |
| --- | --- | --- |
| Host bring-up | Tab5-format P4 board running Brookesia with panel, touch, SD, Wi-Fi through the C6 | M0 gate; serial log + screenshot |
| RPC coverage | try `esp_wifi_set_promiscuous*` and `esp_wifi_80211_tx` from the host through `esp_wifi_remote` | answers the section 3 question with a log, not a guess |
| Slave feature | custom-RPC channel registers, calls and events round-trip | modelled on the ESP-NOW bridge; handler cap accounting verified (`MAX_CUSTOM_MSG_HANDLERS`) |
| Slave OTA | flash the patched C6 image from the host | existing `esp32_esp_hosted_ota` path; version reported by `SlaveToolboxInfo` afterwards |
| Frame builders | golden-byte tests for all ten modes | byte-identical to the current implementation's output for the same inputs |
| Injection | a second device sees the frames | expected subtype/rate per mode, `ESP_OK` from the patched C6, `unsupport frame type: 0c0` without the patch |
| Capture | capture a handshake on a test AP we control | EAPOL and PMKID counted, PCAP opens in Wireshark and decrypts with the known key; frame loss across SDIO measured, not assumed |
| Net utilities | scan a host with known open/closed ports | matches `nmap` on the same host |
| Lease | capture from a connected station, then stop | station reconnects within ~20 s (verified by `GetGeneralState` and an ICMP soak); power-save value restored to what it was |
| Slave death | reset the C6 mid-session (GPIO 15 or watchdog) | host notices within the timeout, stops the session, clears the armed flag, and the Wi-Fi service recovers |
| Duplicate events | force a reconnect under load | the state machine counts one event, not two (this transport is known to duplicate) |
| No leftovers | leave a session running, kill the app, reboot | nothing radio-active after either; no promiscuous filter left on the slave |
| Power | idle current and session current, with and without a live capture | numbers recorded here before M3 |

Traps to regression-test explicitly, because each one cost us time already: an empty scan is not an empty
air; a session must restore power save to the value it found; a leftover radio probe can silence the link
for a minute and look exactly like an AP fault; a fixed-channel capture disassociates the station, so
"capture stopped" and "network back" are two separate things to verify; and on this target a duplicated
hosted event is normal, not two events.

## 12. Risks and open questions

1. **RPC coverage for promiscuous and raw TX** — the gating question. If the slave-side route is required
   (expected), the work includes maintaining a co-processor feature and its firmware image.
2. **Two firmwares, one product** — host and C6 versions must be pinned together, and `esp_wifi_remote`
   depends on a matching `esp_wifi` version, so a host IDF bump can force a co-processor rebuild.
3. **libnet80211 patch maintenance on the C6** — a one-byte edit to a prebuilt library, per IDF version,
   with the offset and instruction differing from the C5's. Needs a reproducible patch step and a CI
   check that fails loudly when the layout changes.
4. **SDIO bandwidth and latency** — capture streams and Wi-Fi data share the transport; M0 must show
   whether an unfiltered capture is even possible, which is part of why filtering on the slave is the
   design and not an optimisation.
5. **Bluetooth coexistence on the C6** — a product using BLE (an HID keyboard, say) shares the radio with
   capture/injection; requires measurement, not assumption.
6. **Brookesia on P4+C6** — component identifiers, versions and HAL board support must be confirmed at M0;
   the P4 devkits Brookesia ships with may not match this panel/touch/PMIC combination.
7. **GUI expressiveness** — the per-mode conditional fields are the interesting case for JSON UI; decide
   between a custom widget and a simpler UI before M2, not during it.
8. **Regulatory** — the channel list for injection must come from the *slave's* country configuration via
   the Wi-Fi service, never a hardcoded table on the host.

## 13. Milestones

- **M0 — two-chip bring-up and the RPC question (go/no-go)**: P4 board running Brookesia with the C6
  providing Wi-Fi; `esp_wifi_remote` probed for promiscuous and raw TX; custom-RPC channel prototype;
  slave OTA path exercised. Exit: a written answer to section 3's table, with logs.
- **M1 — services**: slave toolbox feature (builders, injection engine, capture filter) + host service
  registering section 4's functions/events, exercised from the console example. Exit: capture and port
  scan from the console; injection from the console with arming; counters visible over RPC.
- **M2 — UI**: four JSON UI documents, arming dialog, status-bar indicator, transport-warning states.
  Exit: the screen-by-screen walkthrough of section 6 on the reference board.
- **M3 — product packaging and hardening**: pinned co-processor firmware in the image, feature flags on
  both sides, USB retrieval, read-only MCP tools, the section 11 table passing, and the traps list copied
  into the app's own regression notes.

## 14. References

- ESP-Brookesia: <https://github.com/espressif/esp-brookesia>; components
  [`brookesia_service_wifi`](https://components.espressif.com/components/espressif/brookesia_service_wifi),
  [`brookesia_service_custom`](https://components.espressif.com/components/espressif/brookesia_service_custom),
  [`brookesia_hal_boards`](https://components.espressif.com/components/espressif/brookesia_hal_boards);
  guide pages for the [Wi-Fi service](https://docs.espressif.com/projects/esp-brookesia/en/latest/service/wifi.html)
  and the [custom service](https://docs.espressif.com/projects/esp-brookesia/en/latest/service/framework/custom.html)
- esp-hosted (MCU host, RPC, custom frames): <https://github.com/espressif/esp-hosted-mcu>;
  [`esp_wifi_remote`](https://components.espressif.com/components/espressif/esp_wifi_remote) — transparent
  `esp_wifi_*` over RPC, declarations generated from `esp_wifi.h`
- This tree: `WifiToolbox.cpp` (what is being ported), `DeauthPatch.md` + `patch_deauth_libnet.py` (the
  driver patch injection depends on, now a co-processor concern), `esp32_esp_hosted_ota.cpp` (slave
  firmware update), `EspNowBackendHosted.cpp` (the custom-RPC pattern to copy), `Devices/m5stack-tab5/`
  (the P4+C6 SDIO configuration)
