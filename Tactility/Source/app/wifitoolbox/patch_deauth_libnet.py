#!/usr/bin/env python3
"""
Patch the ESP-IDF v6.1 ESP32-C5 WiFi library (libnet80211.a) so esp_wifi_80211_tx
can transmit deauth frames without crashing.

The stock precompiled lib's ieee80211_raw_frame_sanity_check() rejects deauth
(0xC0) frames ("unsupport frame type: 0c0"). We replicate the ESP32-C5 Deauther's
single-byte fix directly in ieee80211_output.o with a CLEAN binary edit (NOT
objcopy --update-section, which corrupts the object and breaks all raw-frame
injection).

Change: in ieee80211_output.o the 0x12B-th byte of the
.text.ieee80211_raw_frame_sanity_check section (file offset 0x360F) is
0x0d (li a3, 0xD0, "action") -> 0x0c (li a3, 0xC0, "deauth").

Usage (default path is the ESP-IDF v6.1 esp32c5 lib):
    python patch_deauth_libnet.py [path/to/libnet80211.a]

Optional: set RISC_TOOLS to the dir containing riscv32-esp-elf-ar/objdump
(default: C:/Espressif/tools/tools/riscv32-esp-elf/esp-15.2.0_20251204/riscv32-esp-elf/bin).
"""
import os
import shutil
import subprocess
import sys
import tempfile

DEFAULT_LIB = "C:/esp/v6.1/esp-idf/components/esp_wifi/lib/esp32c5/libnet80211.a"
DEFAULT_TOOLS = "C:/Espressif/tools/tools/riscv32-esp-elf/esp-15.2.0_20251204/riscv32-esp-elf/bin"

BYTE_OFF_IN_SECTION = 0x12B
SECTION = ".text.ieee80211_raw_frame_sanity_check"
OLD = 0x0D
NEW = 0x0C


def run(cmd, cwd=None):
    subprocess.run(cmd, cwd=cwd, check=True)


def section_file_offset(obj, section, tools):
    out = subprocess.run(
        [f"{tools}/riscv32-esp-elf-objdump", "-h", obj], capture_output=True, text=True
    ).stdout
    for line in out.splitlines():
        parts = line.split()
        if len(parts) >= 6 and parts[1] == SECTION:
            return int(parts[5], 16)
    raise RuntimeError(f"section {SECTION} not found in {obj}")


def main():
    lib = sys.argv[1] if len(sys.argv) > 1 else DEFAULT_LIB
    tools = os.environ.get("RISC_TOOLS", DEFAULT_TOOLS)
    if not os.path.exists(lib):
        raise SystemExit(f"libnet80211.a not found: {lib}")

    work = tempfile.mkdtemp()
    obj = os.path.join(work, "ieee80211_output.o")
    run([f"{tools}/riscv32-esp-elf-ar", "x", lib, "ieee80211_output.o"], cwd=work)

    off = section_file_offset(obj, SECTION, tools) + BYTE_OFF_IN_SECTION
    with open(obj, "r+b") as f:
        f.seek(off)
        cur = f.read(1)[0]
        if cur == NEW:
            print(f"already patched : {lib}")
            return
        if cur != OLD:
            raise SystemExit(f"unexpected byte {cur:#x} at file offset {off:#x}")
        f.seek(off)
        f.write(bytes([NEW]))
    print(f"patched byte at {off:#x}: 0x{OLD:02x} -> 0x{NEW:02x}")

    backup = lib + ".orig"
    if not os.path.exists(backup):
        shutil.copy2(lib, backup)
        print(f"backed up stock lib -> {backup}")

    run([f"{tools}/riscv32-esp-elf-ar", "r", lib, "ieee80211_output.o"], cwd=work)
    print(f"installed patched object in {lib}")
    print("Rebuild the firmware; deauth (and beacon/probe) injection now works.")


if __name__ == "__main__":
    main()
