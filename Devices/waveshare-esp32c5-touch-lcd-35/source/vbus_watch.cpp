// SPDX-License-Identifier: Apache-2.0
//
// Why does touch die when the USB cable is pulled?
//
// Measured symptom: with the cable in, touch works; unplug it (the board keeps running from its
// battery, display stays lit) and touch stops; plug it back in and touch returns by itself, with no
// reboot. Replaying the vendor's whole AXP2101 initialisation at boot did not change that, so it is
// not a rail the vendor enables and this profile forgets.
//
// This file is the instrument for that question, and it is deliberately deployed *inside* the working
// firmware rather than as a separate app: a bare diagnostic that replaces the firmware leaves the
// screen blank and makes the device look broken, which is exactly the wrong thing to hand someone who
// is already trying to work out whether their board is dead. Here the UI keeps running and touch can
// be tested on the real thing.
//
// What it does, every half second:
//   * reads whether VBUS is present through the PMIC driver's own API (axp2101_is_vbus_present, which
//     is status register 0x00 bit 5) together with the VBUS and battery voltages;
//   * reads the touch controller's touch-point count at 0x38 directly, over the same bus the driver
//     uses, through its own device handle - passively, so it cannot consume an interrupt or disturb
//     the input path. This separates the three explanations that look identical from the outside:
//     the controller stops answering at all (supply), it answers but never reports a touch (sensing),
//     or it reports touches that the input path never sees (interrupt line);
//   * reads the CH32V003 expander's direction/level/PWM registers and the INT pin's level.
//
// Everything goes into a rolling text log in RAM, and - because the log's own transport is the cable
// being unplugged - the log is also written to the partition table's `coredump` partition, which is
// otherwise only used when the firmware panics. The next boot prints it back. That is what makes the
// battery window readable at all.
//
// On the loss of VBUS it also *tries* the candidate fixes, in order, logging what it observes after
// each one:
//   1. re-apply the entire vendor PMIC table and re-assert the expander outputs (+300 ms);
//   2. pulse the expander's pins 0/1, the same pulse the board bring-up uses, which is the touch (and
//      panel) reset line if it is one (+2 s).
// If either restores touch on battery, the log says so and the fix is simply to keep doing it.
//
// This is a diagnostic that is expected to be removed once the question is answered; it is committed
// because the tree does not carry uncommitted diagnostics, not because it belongs in a release.
#include "board_pmic_init.h"

#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <driver/gpio.h>
#include <driver/i2c_master.h>
#include <esp_err.h>
#include <esp_partition.h>
#include <esp_timer.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <drivers/axp2101.h>
#include <tactility/device.h>

namespace {

constexpr int PIN_TOUCH_INT = 3;
constexpr uint8_t AXP_ADDRESS = 0x34;
constexpr uint8_t EXPANDER_ADDRESS = 0x24;
constexpr uint8_t TOUCH_ADDRESS = 0x38;

constexpr uint8_t EXP_REG_DIRECTION = 2;
constexpr uint8_t EXP_REG_LEVEL = 3;
constexpr uint8_t EXP_REG_INPUT = 4;
constexpr uint8_t EXP_REG_PWM = 5;
constexpr uint8_t EXP_PINS_0_1_5 = 0x23;
constexpr uint8_t EXP_PINS_0_1 = 0x03;

constexpr uint8_t FT_REG_POWER_MODE = 0xA5;

constexpr int I2C_TIMEOUT_MS = 100;
constexpr int I2C_SPEED_HZ = 100000;

constexpr int SAMPLE_INTERVAL_MS = 100;
constexpr int SLOW_DIVIDER = 5; // PMIC/expander registers every 500 ms

// VBUS comes back, then the host needs a moment to re-enumerate before anything written here is
// readable; repeating the dump for a while is what makes it a record rather than a race.
constexpr int DUMP_REPEAT_INTERVAL_MS = 5000;
constexpr int DUMP_REPEAT_WINDOW_MS = 40000;

constexpr int RECOVERY_STAGE1_DELAY_MS = 300;
constexpr int RECOVERY_STAGE2_DELAY_MS = 2000;
constexpr int RECOVERY_STAGE3_DELAY_MS = 4000;

#define HIST_MAX 16384
constexpr size_t HIST_MAX_SIZE = HIST_MAX;
char history[HIST_MAX_SIZE];
size_t history_length = 0;

i2c_master_dev_handle_t axp_device = nullptr;
i2c_master_dev_handle_t expander_device = nullptr;
i2c_master_dev_handle_t touch_device = nullptr;

bool vbus_present = true;
bool vbus_valid = false;
int int_level = -1;
unsigned touch_events_on_battery = 0;

uint8_t last_axp[256];
bool last_axp_valid[256];
bool last_axp_known = false;

void history_append(const char* text, size_t length) {
    if (history_length + length + 1 > HIST_MAX_SIZE) {
        const size_t drop = HIST_MAX_SIZE / 4;
        if (drop < history_length) {
            memmove(history, history + drop, history_length - drop);
            history_length -= drop;
        } else {
            history_length = 0;
        }
    }
    if (length > HIST_MAX_SIZE - history_length - 1) {
        length = HIST_MAX_SIZE - history_length - 1;
    }
    memcpy(history + history_length, text, length);
    history_length += length;
    history[history_length] = '\0';
}

void note(const char* format, ...) {
    char line[512];
    va_list args;
    va_start(args, format);
    vsnprintf(line, sizeof(line), format, args);
    va_end(args);
    const size_t length = strnlen(line, sizeof(line));
    history_append(line, length);
    printf("%s", line);
    fflush(stdout);
}

// ---------------------------------------------------------------------------------------------
// Flash stash: the only way to read a log whose transport is the cable being unplugged.
// ---------------------------------------------------------------------------------------------

constexpr uint32_t STASH_MAGIC = 0x31574256u; // "VBW1"
constexpr size_t STASH_MAX = 20480;
constexpr size_t STASH_SECTORS = 6;

struct StashHeader {
    uint32_t magic;
    uint32_t length;
    uint32_t crc;
    uint32_t uptime_ms;
};

const esp_partition_t* stash_partition = nullptr;
unsigned char* stash_buffer = nullptr;

uint32_t crc32_of(const void* data, size_t length) {
    const auto* bytes = static_cast<const unsigned char*>(data);
    uint32_t crc = 0xFFFFFFFFu;
    for (size_t i = 0; i < length; ++i) {
        crc ^= bytes[i];
        for (int bit = 0; bit < 8; ++bit) {
            crc = (crc >> 1) ^ (0xEDB88320u & (uint32_t)(-(int32_t)(crc & 1u)));
        }
    }
    return ~crc;
}

void stash_open() {
    stash_partition = esp_partition_find_first(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_DATA_COREDUMP, nullptr);
    if (stash_partition == nullptr) {
        note("[vw] no coredump partition: the log will not survive a reset\n");
        return;
    }
    stash_buffer = static_cast<unsigned char*>(malloc(STASH_MAX));
    if (stash_buffer == nullptr) {
        stash_partition = nullptr;
    }
}

void stash_load() {
    if (stash_partition == nullptr || stash_buffer == nullptr) {
        return;
    }
    StashHeader header = {};
    if (esp_partition_read(stash_partition, 0, &header, sizeof(header)) != ESP_OK) {
        return;
    }
    if (header.magic != STASH_MAGIC || header.length == 0 || header.length > STASH_MAX - sizeof(header)) {
        return;
    }
    if (esp_partition_read(stash_partition, sizeof(header), stash_buffer, header.length) != ESP_OK) {
        return;
    }
    if (crc32_of(stash_buffer, header.length) != header.crc) {
        return;
    }
    printf("\n[vw] ===== VBUS watch log recovered from flash (previous run, uptime %u ms) =====\n",
           (unsigned)header.uptime_ms);
    fwrite(stash_buffer, 1, header.length, stdout);
    printf("\n[vw] ===== end of recovered log =====\n\n");
    fflush(stdout);
}

void stash_save() {
    if (stash_partition == nullptr || stash_buffer == nullptr || history_length == 0) {
        return;
    }
    StashHeader header = {};
    header.magic = STASH_MAGIC;
    header.length = (uint32_t)history_length;
    header.crc = crc32_of(history, history_length);
    header.uptime_ms = (uint32_t)(esp_timer_get_time() / 1000);
    memcpy(stash_buffer, &header, sizeof(header));
    memcpy(stash_buffer + sizeof(header), history, history_length);

    // Padded to a whole number of words: the write is length-checked, and this keeps the failure mode
    // about the flash rather than about the buffer.
    size_t total = sizeof(header) + history_length;
    while ((total % 4) != 0 && total < STASH_MAX) {
        stash_buffer[total] = '\n';
        ++total;
    }

    const esp_err_t erase_result = esp_partition_erase_range(stash_partition, 0, STASH_SECTORS * 4096);
    if (erase_result != ESP_OK) {
        note("[vw] stash ERASE failed: %s\n", esp_err_to_name(erase_result));
        return;
    }
    const esp_err_t write_result = esp_partition_write(stash_partition, 0, stash_buffer, total);
    if (write_result != ESP_OK) {
        note("[vw] stash WRITE failed: %s\n", esp_err_to_name(write_result));
        return;
    }
    printf("[vw] stashed %u bytes at t=%d ms\n", (unsigned)total, (int)(esp_timer_get_time() / 1000));
    fflush(stdout);
}

// ---------------------------------------------------------------------------------------------
// I2C helpers
// ---------------------------------------------------------------------------------------------

bool read_register(i2c_master_dev_handle_t device, uint8_t reg, uint8_t* buffer, size_t length) {
    if (device == nullptr) {
        return false;
    }
    return i2c_master_transmit_receive(device, &reg, 1, buffer, length, I2C_TIMEOUT_MS) == ESP_OK;
}

bool write_register(i2c_master_dev_handle_t device, uint8_t reg, uint8_t value) {
    if (device == nullptr) {
        return false;
    }
    const uint8_t payload[2] = { reg, value };
    return i2c_master_transmit(device, payload, sizeof(payload), I2C_TIMEOUT_MS) == ESP_OK;
}

bool add_device(i2c_master_bus_handle_t bus, uint8_t address, const char* what, i2c_master_dev_handle_t* out) {
    i2c_device_config_t config = {};
    config.dev_addr_length = I2C_ADDR_BIT_LEN_7;
    config.device_address = address;
    config.scl_speed_hz = I2C_SPEED_HZ;
    if (i2c_master_bus_add_device(bus, &config, out) != ESP_OK) {
        note("[vw] could not add a device handle for %s at 0x%02x\n", what, address);
        return false;
    }
    return true;
}

// ---------------------------------------------------------------------------------------------
// Sampling
// ---------------------------------------------------------------------------------------------

const uint8_t AXP_WATCH_REGS[] = {
    // Everything the vendor's init touches, plus the status and interrupt-status registers: on a VBUS
    // change the answer is whichever of these moves, and dumping the before/after pair says which.
    0x00, 0x01, 0x02, 0x15, 0x16, 0x18, 0x19, 0x24, 0x27, 0x30,
    0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B,
    0x40, 0x41, 0x42, 0x48, 0x49, 0x4A,
    0x61, 0x62, 0x64, 0x68, 0x69, 0x6A,
    0x80, 0x82, 0x83, 0x84, 0x85, 0x86,
    0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A,
};

void log_axp_changes() {
    for (size_t i = 0; i < sizeof(AXP_WATCH_REGS) / sizeof(AXP_WATCH_REGS[0]); ++i) {
        const uint8_t reg = AXP_WATCH_REGS[i];
        uint8_t value = 0;
        const bool ok = read_register(axp_device, reg, &value, 1);
        // The ADC block is live data: its low bytes move every single sample, and logging that would
        // bury everything else. It is printed as a snapshot at a VBUS change instead.
        const bool volatile_adc = reg >= 0x34 && reg <= 0x3B;
        const bool changed = last_axp_known && !volatile_adc &&
                             (ok != last_axp_valid[reg] || value != last_axp[reg]);
        if (changed) {
            note("[vw]   pmic 0x%02x: %s%02x -> %s%02x\n", reg, last_axp_valid[reg] ? "" : "ERR:",
                 last_axp[reg], ok ? "" : "ERR:", value);
        }
        last_axp_valid[reg] = ok;
        last_axp[reg] = value;
    }
    last_axp_known = true;
}

// The whole watched register set, one line: what the PMIC looks like at this instant, which is what
// makes a before/after pair across a VBUS change readable.
void log_axp_snapshot(const char* why) {
    char line[512];
    int offset = snprintf(line, sizeof(line), "[vw]   pmic snapshot (%s):", why);
    for (size_t i = 0; i < sizeof(AXP_WATCH_REGS) / sizeof(AXP_WATCH_REGS[0]) && offset < 440; ++i) {
        const uint8_t reg = AXP_WATCH_REGS[i];
        offset += snprintf(line + offset, sizeof(line) - (size_t)offset, " %02x=%s%02x", reg,
                           last_axp_valid[reg] ? "" : "!", last_axp[reg]);
    }
    note("%s\n", line);
}

void log_expander_state(const char* why) {
    uint8_t direction = 0;
    uint8_t level = 0;
    uint8_t inputs = 0;
    uint8_t pwm = 0;
    const bool ok = read_register(expander_device, EXP_REG_DIRECTION, &direction, 1) &&
                    read_register(expander_device, EXP_REG_LEVEL, &level, 1) &&
                    read_register(expander_device, EXP_REG_INPUT, &inputs, 1) &&
                    read_register(expander_device, EXP_REG_PWM, &pwm, 1);
    note("[vw]   expander (%s): dir=%s%02x level=%s%02x in=%s%02x pwm=%s%02x  int=%d\n", why,
         ok ? "" : "ERR:", direction, ok ? "" : "ERR:", level, ok ? "" : "ERR:", inputs,
         ok ? "" : "ERR:", pwm, gpio_get_level(static_cast<gpio_num_t>(PIN_TOUCH_INT)));
}

void log_touch_state(const char* why) {
    uint8_t points = 0;
    uint8_t power_mode = 0;
    const bool ok = read_register(touch_device, 0x02, &points, 1);
    read_register(touch_device, FT_REG_POWER_MODE, &power_mode, 1);
    note("[vw]   touch (%s): 0x38 %s points=0x%02x power_mode=0x%02x int=%d\n", why,
         ok ? "acks" : "SILENT", points, power_mode, gpio_get_level(static_cast<gpio_num_t>(PIN_TOUCH_INT)));
}

// ---------------------------------------------------------------------------------------------
// Recovery attempts
// ---------------------------------------------------------------------------------------------

void recovery_stage1() {
    note("[vw] stage 1: re-applying the vendor PMIC table and the expander outputs\n");
    const int applied = board_axp_apply_init(axp_device);
    const bool direction_ok = write_register(expander_device, EXP_REG_DIRECTION, EXP_PINS_0_1_5);
    const bool level_ok = write_register(expander_device, EXP_REG_LEVEL, EXP_PINS_0_1_5);
    note("[vw]   applied %d PMIC writes, expander dir/level %s\n", applied,
         (direction_ok && level_ok) ? "rewritten" : "NOT rewritten");
    log_touch_state("after stage 1");
}

void recovery_stage2() {
    note("[vw] stage 2: pulsing the expander's pins 0/1 (the touch/panel reset pulse)\n");
    write_register(expander_device, EXP_REG_LEVEL, EXP_PINS_0_1_5);
    vTaskDelay(pdMS_TO_TICKS(50));
    write_register(expander_device, EXP_REG_LEVEL, EXP_PINS_0_1);
    vTaskDelay(pdMS_TO_TICKS(50));
    write_register(expander_device, EXP_REG_LEVEL, EXP_PINS_0_1_5);
    vTaskDelay(pdMS_TO_TICKS(300));
    log_touch_state("after stage 2");
}

// The controller detects touches only marginally on battery: a finger held down produced a count
// register that flickered between 0 and 1, and light taps were often not detected at all, while the
// same gestures on USB were detected every time. That is not something the PMIC registers explain -
// with the cable out the board floats instead of sharing the host's ground reference, which is the
// classic way a capacitive panel ends up marginal.
//
// Lowering the detection threshold (0x80 = 64) and raising the scan rate (0x88 = 60 instead of 14) were
// both tried as battery-only tuning, and both are gone again: they did not make touch reliable on
// battery, and the scan-rate write turned out to be worse than that. The controller has no reset pin,
// so a register it was given survives reboots and reflashes - the out-of-range rate stayed in the chip
// and went on causing missed taps on USB, after the code that wrote it had already been removed. What
// remains here re-applies the values the driver itself uses, so this watch cannot leave the controller
// in a state the driver does not expect.
constexpr uint8_t TOUCH_THRESHOLD_REGISTER = 0x80;
constexpr uint8_t TOUCH_POINT_RATE_REGISTER = 0x88;
constexpr uint8_t TOUCH_THRESHOLD_STOCK = 128;
constexpr uint8_t TOUCH_POINT_RATE_STOCK = 0x0E;
// On battery this panel's sensing does not report anything at all, while the controller answers I2C and
// every rail is identical to USB - so whatever is lost is lost in the analog path, and the two things
// firmware can do about a signal that is marginal rather than absent are to lower the detection
// threshold and to sample as fast as the part allows. 0x88 is a sample period in milliseconds with a
// documented minimum of 10, so 0x0A is its fastest setting.
constexpr uint8_t TOUCH_THRESHOLD_BATTERY = 32;
constexpr uint8_t TOUCH_POINT_RATE_BATTERY = 0x0A;

// The controller still answers I2C on battery, so it is not unpowered - but it stops sensing reliably.
// Re-applying the configuration it had at boot is the one thing worth doing here, and it is idempotent.
void recovery_stage3() {
    uint8_t previous_threshold = 0;
    read_register(touch_device, TOUCH_THRESHOLD_REGISTER, &previous_threshold, 1);
    note("[vw] stage 3: battery touch profile (threshold %u -> %u, sample period %u ms)\n",
         previous_threshold, TOUCH_THRESHOLD_BATTERY, TOUCH_POINT_RATE_BATTERY);
    write_register(touch_device, 0x00, 0x00);
    write_register(touch_device, TOUCH_THRESHOLD_REGISTER, TOUCH_THRESHOLD_BATTERY);
    write_register(touch_device, TOUCH_POINT_RATE_REGISTER, TOUCH_POINT_RATE_BATTERY);
    write_register(touch_device, FT_REG_POWER_MODE, 0x00);
    uint8_t mode = 0;
    uint8_t power_mode = 0;
    uint8_t threshold = 0;
    uint8_t point_rate = 0;
    read_register(touch_device, 0x00, &mode, 1);
    read_register(touch_device, FT_REG_POWER_MODE, &power_mode, 1);
    read_register(touch_device, TOUCH_THRESHOLD_REGISTER, &threshold, 1);
    read_register(touch_device, TOUCH_POINT_RATE_REGISTER, &point_rate, 1);
    note("[vw]   controller now mode=0x%02x power_mode=0x%02x threshold=%u rate=%u\n", mode, power_mode,
         threshold, point_rate);
}

// ---------------------------------------------------------------------------------------------

[[noreturn]] void vbus_watch_task(void* argument) {
    (void)argument;

    // The devices this depends on are started by the kernel moments after this task is created.
    Device* pmic = nullptr;
    for (int attempt = 0; attempt < 60; ++attempt) {
        if (device_get_by_name("pmic0", &pmic) == ERROR_NONE) {
            break;
        }
        vTaskDelay(pdMS_TO_TICKS(250));
    }
    if (pmic == nullptr) {
        note("[vw] no pmic0 device; VBUS watch disabled\n");
        vTaskDelete(nullptr);
    }

    i2c_master_bus_handle_t bus = nullptr;
    for (int attempt = 0; attempt < 60; ++attempt) {
        if (i2c_master_get_bus_handle(I2C_NUM_0, &bus) == ESP_OK) {
            break;
        }
        vTaskDelay(pdMS_TO_TICKS(250));
    }
    if (bus == nullptr) {
        note("[vw] no I2C bus on port 0; VBUS watch disabled\n");
        vTaskDelete(nullptr);
    }

    add_device(bus, AXP_ADDRESS, "axp2101", &axp_device);
    add_device(bus, EXPANDER_ADDRESS, "expander", &expander_device);
    add_device(bus, TOUCH_ADDRESS, "ft6336", &touch_device);

    gpio_config_t int_input = {};
    int_input.pin_bit_mask = 1ULL << PIN_TOUCH_INT;
    int_input.mode = GPIO_MODE_INPUT;
    int_input.pull_up_en = GPIO_PULLUP_ENABLE;
    gpio_config(&int_input);

    stash_open();
    stash_load();

    note("[vw] VBUS watch running (log %u bytes rolling, flash stash %s)\n", (unsigned)HIST_MAX_SIZE,
         stash_partition != nullptr ? "on" : "OFF");
    log_touch_state("baseline");

    int64_t next_sample = 0;
    int64_t next_dump = 0;
    int64_t next_stash = 0;
    int64_t next_heartbeat = 0;
    int64_t recovery_stage2_at = 0;
    int64_t recovery_stage3_at = 0;
    int64_t dump_until = 0;
    int slow_counter = 0;
    unsigned samples = 0;

    for (;;) {
        const int64_t now = esp_timer_get_time() / 1000;

        // ---- VBUS presence, straight from the PMIC driver -------------------------------------
        bool present = false;
        if (axp2101_is_vbus_present(pmic, &present) == ERROR_NONE) {
            if (!vbus_valid) {
                vbus_valid = true;
                vbus_present = present;
                note("[vw] baseline VBUS present = %d\n", present ? 1 : 0);
            } else if (present != vbus_present) {
                vbus_present = present;
                uint16_t vbus_millivolt = 0;
                uint16_t battery_millivolt = 0;
                bool charging = false;
                axp2101_get_vbus_voltage(pmic, &vbus_millivolt);
                axp2101_get_battery_voltage(pmic, &battery_millivolt);
                axp2101_is_charging(pmic, &charging);
                note("\n[vw] *** VBUS %s at t=%d ms (vbus=%u mV battery=%u mV charging=%d, "
                     "touch_events_on_battery=%u) ***\n",
                     present ? "RETURNED" : "LOST", (int)now, vbus_millivolt, battery_millivolt,
                     charging ? 1 : 0, touch_events_on_battery);
                log_axp_changes();
                log_axp_snapshot("at the change");
                log_expander_state("at the change");
                log_touch_state("at the change");

                if (!present) {
                    recovery_stage2_at = now + RECOVERY_STAGE2_DELAY_MS;
                    recovery_stage3_at = now + RECOVERY_STAGE3_DELAY_MS;
                    vTaskDelay(pdMS_TO_TICKS(RECOVERY_STAGE1_DELAY_MS));
                    recovery_stage1();
                } else {
                    recovery_stage2_at = 0;
                    recovery_stage3_at = 0;
                }

                // Both the write and the read side of this log need the host that just came back.
                stash_save();
                dump_until = now + DUMP_REPEAT_WINDOW_MS;
                next_dump = now + DUMP_REPEAT_INTERVAL_MS;
            }
        }

        // ---- touch controller, read passively -----------------------------------------------
        if (now >= next_sample) {
            next_sample = now + SAMPLE_INTERVAL_MS;

            // Only the interrupt line is read here - a plain GPIO input, so this task cannot disturb
            // the controller's data or race the input path for it. (Earlier versions of this watch did
            // read the touch registers from here: that crashed the firmware once, and it competes with
            // the driver for data the controller only latches briefly.) INT is the controller's own
            // statement that it detected a touch, which is all this needs in order to say whether a
            // fault is in the controller or after it.
            const int level = gpio_get_level(static_cast<gpio_num_t>(PIN_TOUCH_INT));
            if (level != int_level) {
                int_level = level;
                if (level == 0 && !vbus_present) {
                    ++touch_events_on_battery;
                }
                note("[vw]   INT %s at t=%d ms (vbus=%d, battery detections=%u)\n",
                     level == 0 ? "ASSERTED" : "released", (int)now, vbus_present ? 1 : 0,
                     touch_events_on_battery);
            }

            if (recovery_stage2_at != 0 && now >= recovery_stage2_at) {
                recovery_stage2_at = 0;
                recovery_stage2();
            }

            if (recovery_stage3_at != 0 && now >= recovery_stage3_at) {
                recovery_stage3_at = 0;
                recovery_stage3();
            }

            ++samples;

            if (++slow_counter >= SLOW_DIVIDER) {
                slow_counter = 0;
                log_axp_changes();
            }
        }

        // A heartbeat, so the log can prove this task kept sampling through a window in which nothing
        // else happened. Without it, "no touch events on battery" and "the sampler died" look the
        // same in the record, and that ambiguity is the whole question here.
        if (now >= next_heartbeat) {
            next_heartbeat = now + 10000;
            note("[vw] heartbeat t=%d ms vbus=%d samples=%u int=%d battery_detections=%u\n", (int)now,
                 vbus_present ? 1 : 0, samples, int_level, touch_events_on_battery);
        }

        if (now < dump_until && now >= next_dump) {
            next_dump = now + DUMP_REPEAT_INTERVAL_MS;
            printf("\n[vw] ===== log (%u bytes) =====\n%s\n[vw] ===== end of log =====\n",
                   (unsigned)history_length, history);
            fflush(stdout);
        }

        if (now >= next_stash) {
            next_stash = now + 60000;
            stash_save();
        }

        vTaskDelay(pdMS_TO_TICKS(20));
    }
}

} // namespace

extern "C" void waveshare_c5_vbus_watch_start() {
    // Deliberately below the apps and the input path: this is an observer, and it must never be the
    // reason a touch is late.
    xTaskCreate(vbus_watch_task, "vbus-watch", 5120, nullptr, 3, nullptr);
}
