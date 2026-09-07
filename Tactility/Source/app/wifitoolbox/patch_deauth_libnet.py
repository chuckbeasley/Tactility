#!/usr/bin/env python3
"""
Patch the ESP-IDF ESP32-C5 WiFi library (libnet80211.a) so esp_wifi_80211_tx
can transmit deauth frames without crashing.

The stock precompiled lib's ieee80211_raw_frame_sanity_check() rejects deauth
(0xC0) frames ("unsupport frame type: 0c0"). We replicate the ESP32-C5
Deauther's single-byte fix directly in ieee80211_output.o with a CLEAN binary
edit (NOT objcopy --update-section, which corrupts the object and breaks all
raw-frame injection).

Change: in ieee80211_output.o the 0x12B-th byte of the
.text.ieee80211_raw_frame_sanity_check section is 0x0d (li a3, 0xD0, "action")
-> 0x0c (li a3, 0xC0, "deauth").

Idempotent: safe to run on every build; it prints "already patched" no-op and
exits 0 if the byte is already 0x0c. Backs up the stock lib once as
<lib>.orig.

Usage:
    python patch_deauth_libnet.py [--lib PATH] [--target esp32c5]

If --lib is omitted it derives the path from $IDF_PATH and $IDF_TARGET
(default target esp32c5). If the toolchain tools aren't found, it defaults to
the standard ESP-IDF install location and can be overridden with RISC_TOOLS.
"""
import os
import shutil
import subprocess
import sys
import tempfile

SECTION = ".text.ieee80211_raw_frame_sanity_check"
BYTE_OFF_IN_SECTION = 0x12B
OLD = 0x0D
NEW = 0x0C

DEFAULT_TOOLS = "C:/Espressif/tools/tools/riscv32-esp-elf/esp-15.2.0_20251204/riscv32-esp-elf/bin"
DEFAULT_LIB = "C:/esp/v6.1/esp-idf/components/esp_wifi/lib/esp32c5/libnet80211.a"


def find_tools():
    p = shutil.which("riscv32-esp-elf-ar")
    if p:
        return os.path.dirname(p)
    tools_root = os.environ.get("IDF_TOOLS_PATH", "C:/Espressif/tools")
    base = os.path.join(tools_root, "tools", "riscv32-esp-elf")
    if os.path.isdir(base):
        for d in os.listdir(base):
            candidate = os.path.join(base, d, "riscv32-esp-elf", "bin")
            if os.path.isfile(os.path.join(candidate, "riscv32-esp-elf-ar")):
                return candidate
    return os.environ.get("RISC_TOOLS", DEFAULT_TOOLS)


def resolve_lib(target):
    idf_path = os.environ.get("IDF_PATH")
    if idf_path:
        candidate = os.path.join(idf_path, "components", "esp_wifi", "lib", target, "libnet80211.a")
        if os.path.exists(candidate):
            return candidate
    return DEFAULT_LIB


def run(cmd, cwd=None):
    subprocess.run(cmd, cwd=cwd, check=True)


def section_file_offset(obj, section, tools):
    out = subprocess.run(
        [f"{tools}/riscv32-esp-elf-objdump", "-h", obj], capture_output=True, text=True
    ).stdout
    for line in out.splitlines():
        parts = line.split()
        if len(parts) >= 6 and parts[1] == section:
            return int(parts[5], 16)
    raise RuntimeError(f"section {section} not found in {obj}")


def main():
    lib = DEFAULT_LIB
    target = os.environ.get("IDF_TARGET", "esp32c5")
    args = sys.argv[1:]
    i = 0
    while i < len(args):
        if args[i] == "--lib":
            lib = args[i + 1]
            i += 2
        elif args[i] == "--target":
            target = args[i + 1]
            i += 2
        elif args[i] == "--help":
            print(__doc__)
            return 0
        else:
            lib = args[i]
            i += 1

    if lib == DEFAULT_LIB:
        lib = resolve_lib(target)

    tools = find_tools()
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
            return 0
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
    return 0


if __name__ == "__main__":
    sys.exit(main())
