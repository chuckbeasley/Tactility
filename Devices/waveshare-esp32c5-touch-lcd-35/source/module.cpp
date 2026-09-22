// SPDX-License-Identifier: Apache-2.0
//
// Board bring-up for the Waveshare ESP32-C5-Touch-LCD-3.5.
//
// This exists because the panel does not come up on its own, and the reason was found by measurement
// rather than from a datasheet (Waveshare publishes neither a schematic nor an example for this SKU:
// its documentation says, of both, "Related resources are still being prepared").
//
// What was established on the board:
//   * The SPI bus itself is fine. The TF card answers CMD0 with R1=0x01 at CS=9 on SCLK=6/MOSI=7/
//     MISO=2, which is the vendor's own SD chip select, so those three pins and the bus are proven.
//   * The panel shares that bus: SCLK=6, MOSI=7, CS=8, DC=5, and no reset GPIO - the driver's reset is
//     the software SWRESET over SPI.
//   * Despite that, the panel ignored every command sent to it - no fill, no blank, no ID read, on any
//     CS/DC combination - until two things were done first, in this order, exactly as the vendor's
//     BSP does them:
//
//       1. The AXP2101 PMIC's rail enables. This is the first thing the vendor's app_main does, about
//          1.4 s before it creates the panel: it sets all eight bits of register 0x90 - which covers
//          BLDO1/BLDO2, the backlight LDOs, plus the ALDO and DLDO rails - and bit 0 of register 0x91.
//       2. A reset pulse on pins 0 and 1 of the CH32V003 I2C IO-expander at 0x24, immediately before
//          the panel is addressed. The vendor does this once, in bsp_display_new's caller, and it is
//          what releases whatever the panel is waiting for.
//
// With both applied, a full-screen fill of a solid colour lands correctly. Without them, nothing
// reaches the panel at all - which is exactly the kind of fault that a pin sweep cannot find, because
// every pin involved is already correct.
//
// The I2C bus is created here and torn down again on purpose: the devicetree's i2c0 node starts its
// own bus on the same port moments later, and two masters cannot share I2C_NUM_0.
#include <tactility/error.h>
#include <tactility/log.h>
#include <tactility/module.h>

#include <driver/i2c_master.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

namespace {

constexpr auto* TAG = "waveshare-c5-35";

// Measured on the board, and the reason the device profile's i2c node is on these pins: the board bus
// is NOT the 4/5 pair its camera example logs (that bus is the camera's SCCB and fails to probe).
constexpr int PIN_I2C_SDA = 27;
constexpr int PIN_I2C_SCL = 26;

constexpr uint8_t EXPANDER_ADDRESS = 0x24;
constexpr uint8_t EXPANDER_REG_DIRECTION = 2;
constexpr uint8_t EXPANDER_REG_LEVEL = 3;
// Pins 0, 1 and 5 as outputs, and 0, 1 and 5 driven high: the state the vendor's BSP leaves behind.
constexpr uint8_t EXPANDER_PINS_0_1_5 = 0x23;
// Pin 5 stays high (it is the backlight enable); only 0 and 1 are pulsed.
constexpr uint8_t EXPANDER_PINS_0_1 = 0x03;

constexpr uint8_t AXP2101_ADDRESS = 0x34;
constexpr uint8_t AXP2101_REG_LDO_ENABLE_0 = 0x90;
constexpr uint8_t AXP2101_REG_LDO_ENABLE_1 = 0x91;

constexpr int I2C_TIMEOUT_MS = 100;

bool write_register(i2c_master_dev_handle_t device, uint8_t reg, uint8_t value) {
    const uint8_t payload[2] = { reg, value };
    return i2c_master_transmit(device, payload, sizeof(payload), I2C_TIMEOUT_MS) == ESP_OK;
}

error_t add_device(i2c_master_bus_handle_t bus, uint8_t address, i2c_master_dev_handle_t& out) {
    i2c_device_config_t config = {};
    config.dev_addr_length = I2C_ADDR_BIT_LEN_7;
    config.device_address = address;
    config.scl_speed_hz = 100000;
    return i2c_master_bus_add_device(bus, &config, &out) == ESP_OK ? ERROR_NONE : ERROR_RESOURCE;
}

error_t board_bring_up() {
    i2c_master_bus_config_t bus_config = {};
    bus_config.i2c_port = I2C_NUM_0;
    bus_config.sda_io_num = static_cast<gpio_num_t>(PIN_I2C_SDA);
    bus_config.scl_io_num = static_cast<gpio_num_t>(PIN_I2C_SCL);
    bus_config.clk_source = I2C_CLK_SRC_DEFAULT;
    bus_config.glitch_ignore_cnt = 7;
    bus_config.flags.enable_internal_pullup = true;

    i2c_master_bus_handle_t bus = nullptr;
    if (i2c_new_master_bus(&bus_config, &bus) != ESP_OK) {
        LOG_E(TAG, "i2c bus init failed on sda=%d scl=%d", PIN_I2C_SDA, PIN_I2C_SCL);
        return ERROR_RESOURCE;
    }

    // 1. The PMIC rails. Without these the panel never answers.
    i2c_master_dev_handle_t pmic = nullptr;
    if (add_device(bus, AXP2101_ADDRESS, pmic) == ERROR_NONE) {
        uint8_t enable_1 = 0;
        i2c_master_transmit_receive(pmic, (const uint8_t[]){ AXP2101_REG_LDO_ENABLE_1 }, 1, &enable_1, 1,
                                   I2C_TIMEOUT_MS);
        write_register(pmic, AXP2101_REG_LDO_ENABLE_0, 0xFF);
        write_register(pmic, AXP2101_REG_LDO_ENABLE_1, (uint8_t)(enable_1 | 0x01));
        i2c_master_bus_rm_device(pmic);
        LOG_I(TAG, "AXP2101 rails enabled (reg 0x90 = 0xFF, reg 0x91 |= 0x01)");
    } else {
        LOG_W(TAG, "no AXP2101 at 0x%02x; the panel may stay silent", AXP2101_ADDRESS);
    }

    // 2. The expander reset pulse, immediately before the display driver starts.
    i2c_master_dev_handle_t expander = nullptr;
    if (add_device(bus, EXPANDER_ADDRESS, expander) == ERROR_NONE) {
        write_register(expander, EXPANDER_REG_DIRECTION, EXPANDER_PINS_0_1_5);
        write_register(expander, EXPANDER_REG_LEVEL, EXPANDER_PINS_0_1_5);
        vTaskDelay(pdMS_TO_TICKS(50));
        write_register(expander, EXPANDER_REG_LEVEL, EXPANDER_PINS_0_1);
        vTaskDelay(pdMS_TO_TICKS(50));
        write_register(expander, EXPANDER_REG_LEVEL, EXPANDER_PINS_0_1_5);
        vTaskDelay(pdMS_TO_TICKS(50));
        i2c_master_bus_rm_device(expander);
        LOG_I(TAG, "CH32V003 expander pulsed (pins 0/1), panel reset released");
    } else {
        LOG_W(TAG, "no expander at 0x%02x; the panel may stay silent", EXPANDER_ADDRESS);
    }

    // Release the port: the devicetree's i2c0 device starts its own bus on I2C_NUM_0 next.
    i2c_del_master_bus(bus);
    return ERROR_NONE;
}

} // namespace

extern "C" {

Module waveshare_esp32c5_touch_lcd_35_module = {
    .name = "waveshare-esp32c5-touch-lcd-35",
    .start = board_bring_up
};

}
