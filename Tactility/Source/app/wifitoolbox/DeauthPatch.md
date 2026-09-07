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

## How the fix is applied

The patch targets the machine-local ESP-IDF install (not the repo), because
`libnet80211.a` is a precompiled binary shipped with IDF. Two layers make this
automatic:

### 1. Build-time automation (no manual step)

The root `CMakeLists.txt` registers a `tactility_deauth_patch` custom target
for `esp32c5` targets and makes `Tactility.elf` depend on it, so it runs **before
the link** on every `idf.py build`. It invokes `patch_deauth_libnet.py`, which is
idempotent (a no-op once the byte is already `0x0c`), so:

- **Fresh IDF install** → the first build applies the byte change once, then links.
- **Subsequent builds** → it prints `already patched` and links (fast).

This means you just run `idf.py build` as usual; deauth-capable and
beacon/probe-capable firmware comes out automatically.

### 2. The patch script (`patch_deauth_libnet.py`)

It backs up the stock lib to `libnet80211.a.orig` (once) and applies the clean
one-byte change directly in `ieee80211_output.o`. It auto-locates the lib and the
toolchain from `IDF_PATH`/`IDF_TARGET`/`IDF_TOOLS_PATH` (or PATH), so it works on
any machine, not just this one:

```
python patch_deauth_libnet.py                 # auto-locate from IDF env
python patch_deauth_libnet.py --lib <path>    # explicit archive
python patch_deauth_libnet.py --target esp32c5
```

Manual run (only needed if you build outside `idf.py`):
```
python patch_deauth_libnet.py
```
Expected result after patching: `libnet80211.a` sanity check at `+0x128` shows
`li a3,192` (was `208`).

## Why this is needed

`esp_wifi_80211_tx` on the ESP32-C5 (WiFi 6, IDF v6.1) is the only raw-802.11
injection entry point. With the stock lib it silently drops deauth; with the
clean one-byte patch it transmits deauth **and** beacon/probe without crashing.
The Wi-Fi Toolbox's Deauth (targeted + broadcast) feature and Beacon Spam /
Probe Flood all run through `wifi_send_raw_frame → esp_wifi_80211_tx`.

> Note: this is a machine-local IDF tweak, but the `CMakeLists.txt` hook already
> re-applies it automatically via `patch_deauth_libnet.py` on every build, so a
> fresh checkout/rebuild on the same IDF install carries deauth. You only need to
> re-run `patch_deauth_libnet.py` manually if you build via a path that bypasses
> this project's CMake (e.g., a different top-level `CMakeLists.txt`) or after an
> IDF reinstall resets the archive.
