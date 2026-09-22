// SPDX-License-Identifier: Apache-2.0
#pragma once
//
// The AXP2101 initialisation the vendor's firmware performs and the devicetree binding cannot express.
//
// The binding models the ALDO/BLDO/DLDO/CPUSLDO channels - their voltages and enables - and nothing
// else. The vendor's init also programmes the whole DCDC group, the VBUS/power-path registers and a
// dozen configuration registers, and leaving those out is not cosmetic: it left this board with a
// touch controller whose supply follows VBUS, so touch worked while the USB cable was plugged in and
// died the moment it was unplugged (on battery, with the display still running), then came back by
// itself when the cable went back in.
//
// Recovered by disassembling the factory image's init (see wrk/axp2101_init_report.md), which was
// calibrated against a real cold boot of the vendor firmware: it reproduces all nine rail enables
// (register 0x90 = 0xFF, 0x91 bit 0) and all fifteen rail voltages that the vendor's own log prints.
//
// Entries are read-modify-write: {register, mask, value} writes (read(reg) & mask) | value. The
// vendor's setters are RMW, so replaying them literally preserves bits it deliberately keeps.
//
// It lives in a header because two callers need it: the boot-time bring-up, and the VBUS watch, which
// re-applies the whole table when the cable is pulled (see vbus_watch.cpp).
#include <cstddef>
#include <cstdint>

#include <driver/i2c_master.h>

struct BoardAxpWrite {
    uint8_t reg;
    uint8_t mask;
    uint8_t value;
};

inline constexpr BoardAxpWrite BOARD_AXP_INIT[] = {
    // begin(), immediately after the chip-ID check
    { 0x30, 0xFD, 0x00 }, // clear bit 1
    { 0x15, 0xF0, 0x06 },
    { 0x16, 0xF8, 0x04 },
    { 0x24, 0xF8, 0x00 }, // power-off voltage 2600 mV

    // The DCDC group: the rails the binding has no properties for at all.
    { 0x82, 0xFF, 0x12 }, // DC1 = 3300 mV
    { 0x83, 0x80, 0x32 }, // DC2 = 1000 mV
    { 0x84, 0x80, 0x69 }, // DC3 = 3300 mV
    { 0x85, 0x80, 0x32 }, // DC4 = 1000 mV
    { 0x86, 0xE0, 0x13 }, // DC5 = 3300 mV
    { 0x92, 0xE0, 0x1C }, // ALDO1 = 3300 mV
    { 0x93, 0xE0, 0x1C }, // ALDO2 = 3300 mV
    { 0x94, 0xE0, 0x1C }, // ALDO3 = 3300 mV
    { 0x95, 0xE0, 0x1C }, // ALDO4, transient - rewritten to 1800 mV below
    { 0x96, 0xE0, 0x0A }, // BLDO1 = 1500 mV
    { 0x97, 0xE0, 0x17 }, // BLDO2 = 2800 mV
    { 0x98, 0xE0, 0x0A }, // CPUSLDO = 1000 mV
    { 0x99, 0xE0, 0x1C }, // DLDO1 = 3300 mV
    { 0x9A, 0xE0, 0x1C }, // DLDO2 = 3300 mV

    // DCDC enables
    { 0x80, 0xFD, 0x02 },
    { 0x80, 0xFB, 0x04 },
    { 0x80, 0xF7, 0x08 },
    { 0x80, 0xEF, 0x10 },

    // Rail enables, one bit at a time, in the vendor's order
    { 0x90, 0xFE, 0x01 },
    { 0x90, 0xFD, 0x02 },
    { 0x90, 0xFB, 0x04 },
    { 0x90, 0xF7, 0x08 },
    { 0x90, 0xEF, 0x10 },
    { 0x90, 0xDF, 0x20 },
    { 0x90, 0xBF, 0x40 },
    { 0x90, 0x7F, 0x80 },
    { 0x91, 0xFE, 0x01 },

    // Seconds pass and rails are re-applied; ALDO4 settles at 1800 mV, not 3300.
    { 0x82, 0xFF, 0x12 },
    { 0x80, 0xFE, 0x01 },
    { 0x92, 0xE0, 0x1C },
    { 0x90, 0xFE, 0x01 },
    { 0x93, 0xE0, 0x1C },
    { 0x90, 0xFD, 0x02 },
    { 0x95, 0xE0, 0x0D }, // ALDO4 final = 1800 mV
    { 0x90, 0xF7, 0x08 },
    { 0x97, 0xE0, 0x17 },
    { 0x90, 0xDF, 0x20 },

    // Power-key timings, the ADC/fuel-gauge block and the interrupt enables.
    { 0x27, 0xF3, 0x00 },
    { 0x27, 0xFC, 0x00 },
    { 0x30, 0xFD, 0x00 },
    { 0x68, 0xFE, 0x01 },
    { 0x30, 0xFB, 0x04 },
    { 0x30, 0xFE, 0x01 },
    { 0x30, 0xF7, 0x08 },
    { 0x69, 0xF9, 0x01 },
    { 0x40, 0x00, 0x00 },
    { 0x41, 0x00, 0x00 },
    { 0x42, 0x00, 0x00 },
    { 0x48, 0xFF, 0xFF },
    { 0x49, 0xFF, 0xFF },
    { 0x4A, 0xFF, 0xFF },
    { 0x41, 0x00, 0xFC },
    { 0x42, 0x00, 0x18 },
    { 0x61, 0xFC, 0x02 },
    { 0x62, 0xE0, 0x08 },
    { 0x64, 0xFC, 0x02 },
    { 0x19, 0xCF, 0x00 },
    { 0x19, 0xF8, 0x02 },
    { 0x42, 0x00, 0x80 },
    { 0x6A, 0xF8, 0x07 },
};

inline constexpr size_t BOARD_AXP_INIT_COUNT = sizeof(BOARD_AXP_INIT) / sizeof(BOARD_AXP_INIT[0]);

/// Replays the table against an already-added I2C device handle, read-modify-write per entry, the way
/// the vendor's own setters do. Returns how many entries were applied; entries whose read fails are
/// skipped, which is what makes this safe to call again on a bus that is only half answering.
int board_axp_apply_init(i2c_master_dev_handle_t pmic_device);
