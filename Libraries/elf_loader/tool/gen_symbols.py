import re, subprocess, sys, os

READELF = r"C:\Espressif\tools\riscv32-esp-elf\esp-15.2.0_20251204\riscv32-esp-elf\bin\riscv32-esp-elf-readelf.exe"
ELF     = r"C:\Tactility\build\Tactility.elf"
OUT     = r"C:\Tactility\Libraries\elf_loader\src\esp_all_symbol.c"
SYSFSYM = r"C:\Tactility\Libraries\elf_loader\src\esp_elf_symbol.c"

EXCLUDE = {"elf_find_sym", "g_customer_elfsyms", "elf_set_symbol_resolver"}

with open(SYSFSYM, encoding="utf-8") as f:
    EXCLUDE |= set(re.findall(r"ESP_ELFSYM_EXPORT\((\w+)\)", f.read()))

def main():
    with open(ELF, "rb") as f:
        pass
    out = subprocess.run([READELF, "-s", "-W", ELF], stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    if out.returncode != 0:
        print("readelf failed:", out.stderr)
        sys.exit(1)

    symbols = []
    ident = re.compile(r'^[A-Za-z_][A-Za-z0-9_]*$')
    for raw in out.stdout.splitlines():
        line = raw.strip()
        parts = line.split()
        if len(parts) < 8 or ":" not in parts[0]:
            continue
        idx = parts[0].rstrip(":")
        try:
            int(idx)
        except ValueError:
            continue
        addr   = parts[1]
        size   = parts[2]
        stype  = parts[3]
        bind   = parts[4]
        vis    = parts[5]
        ndx    = parts[6]
        name   = "".join(parts[7:])
        if stype not in ("FUNC", "OBJECT"):
            continue
        if bind not in ("GLOBAL", "WEAK"):
            continue
        if vis != "DEFAULT":
            continue
        if name in EXCLUDE or not name or not ident.match(name):
            continue
        if name.startswith("_ZN"):
            continue
        if ndx == "ABS":
            pass
        elif ndx.isdigit():
            if int(ndx) == 0:
                continue
        else:
            continue
        symbols.append(name)

    seen = set()
    uniq = []
    for s in symbols:
        if s not in seen:
            seen.add(s)
            uniq.append(s)

    print("unique symbols:", len(uniq))

    lines = []
    lines.append("/*")
    lines.append(" * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD")
    lines.append(" *")
    lines.append(" * SPDX-License-Identifier: Apache-2.0")
    lines.append(" */")
    lines.append("")
    lines.append("#include <stddef.h>")
    lines.append("")
    lines.append('#include "private/elf_symbol.h"')
    lines.append("")
    lines.append("/* Extern declarations from ELF symbol table */")
    lines.append("")
    lines.append("#pragma GCC diagnostic push")
    lines.append('#pragma GCC diagnostic ignored "-Wbuiltin-declaration-mismatch"')
    for s in uniq:
        lines.append("extern int %s;" % s)
    lines.append("#pragma GCC diagnostic pop")
    lines.append("")
    lines.append("/* Available ELF symbols table: g_customer_elfsyms */")
    lines.append("")
    lines.append("const struct esp_elfsym g_customer_elfsyms[] = {")
    for s in uniq:
        lines.append("    ESP_ELFSYM_EXPORT(%s)," % s)
    lines.append("    ESP_ELFSYM_END")
    lines.append("};")
    lines.append("")

    os.makedirs(os.path.dirname(OUT), exist_ok=True)
    with open(OUT, "w", encoding="utf-8") as f:
        f.write("\n".join(lines))
    print("wrote", OUT)

if __name__ == "__main__":
    main()
