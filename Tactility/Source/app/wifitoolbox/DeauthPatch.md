# ESP32-C5 deauth injection — the real fix

## Findings (all verified on-device)

1. **Root cause of "deauth does nothing":** the precompiled IDF WiFi library
   `libnet80211.a` (`ieee80211_raw_frame_sanity_check`) rejects deauth/dissoc
   frames — serial log shows `E wifi:unsupport frame type: 0c0`.

2. **The ESP32-C5 Deauther project fixes this** with a patched `libnet80211.a`.
   We identified it as a **single byte** change in `ieee80211_output.o`
   (`ieee80211_raw_frame_sanity_check`): the whitelist check at offset `0x12B`
   changes `li a3, 0xD0` (action) → `li a3, 0xC0` (deauth), allowing deauth.

3. **CRITICAL — how you apply that byte matters.** Applying it via
   `objcopy --dump-section / --update-section` **corrupts the object** and makes
   `esp_wifi_80211_tx` crash (Load-access fault) for **every** raw frame
   (beacon *and* deauth). This is what made it look like "deauth-specific." The
   correct method is a **clean direct binary edit of one byte**, which leaves the
   object 100% intact (relocations preserved) and makes **beacon and deauth both
   inject successfully** (`esp_err=0`, no crash).

## Verified results (clean patch, IDF v6.1, ESP32-C5)

| Test | Result |
|---|---|
| Beacon `0x80` (stock lib) | `esp_err=0` (works) |
| Beacon `0x80` (objcopy patch) | **crash** (broken patch) |
| Deauth `0xC0` (objcopy patch) | **crash** (broken patch) |
| Deauth `0xC0` (clean patch, `en_sys_seq=false`) | `esp_err=0` (works) |
| Deauth `0xC0` (clean patch, `en_sys_seq=true`) | `esp_err=0` (works) |
| Beacon `0x80` (clean patch) | `esp_err=0` (works) |

## How to apply the fix (one-time, per IDF install)

The patch is applied to the machine-local ESP-IDF install (not the repo). Use the
provided script, which backs up the stock lib and applies the clean one-byte
change directly in `ieee80211_output.o`:

```
python patch_deauth_libnet.py [path-to-esp32c5/libnet80211.a]
```

Default path: `C:/esp/v6.1/esp-idf/components/esp_wifi/lib/esp32c5/libnet80211.a`.
Backup saved as `libnet80211.a.orig`. Rebuild the firmware after patching; expect
`libnet80211.a` to be ~1.86 MB with the sanity check at `+0x128` showing
`li a3,192` (was `208`).

## Why this is needed

`esp_wifi_80211_tx` on the ESP32-C5 (WiFi 6, IDF v6.1) is the only raw-802.11
injection entry point. With the stock lib it silently drops deauth; with the
clean one-byte patch it transmits deauth **and** beacon/probe without crashing.
The Wi-Fi Toolbox's Deauth (targeted + broadcast) feature and Beacon Spam /
Probe Flood all run through `wifi_send_raw_frame → esp_wifi_80211_tx`.

> Note: this is a machine-local IDF tweak — a fresh checkout won't carry it. To
> make deauth work on a fresh build, re-run the patch before `idf.py build`.
