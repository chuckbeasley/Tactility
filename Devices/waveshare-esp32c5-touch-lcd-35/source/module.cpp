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
//       1. The AXP2101 PMIC's rail enables, which is the first thing the vendor's app_main does, about
//          1.4 s before it creates the panel. Those rails are now declared in the devicetree, where
//          the PMIC driver brings them up at device start - before the panel, because pmic0 is a
//          child of i2c0 and the display is a child of spi0 which comes later in the tree. Only the
//          rails this board uses are enabled, at the voltages the vendor's own boot log reports.
//       2. A reset pulse on pins 0 and 1 of the CH32V003 I2C IO-expander at 0x24, immediately before
//          the panel is addressed. The vendor does this once, in bsp_display_new's caller, and it is
//          what releases whatever the panel is waiting for. That is what runs here, because the
//          expander has no devicetree driver: it is a custom part with a vendor-specific register map
//          (reg 2 direction, 3 level, 4 inputs, 5 PWM) and no binding in this tree.
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

constexpr int I2C_TIMEOUT_MS = 100;

// The AXP2101 register the devicetree PMIC binding cannot express.
//
// The rails this board uses are declared in the devicetree, and pmic0 brings them up before the
// touch controller and the panel. But the vendor's own PMIC init also sets bit 0 of register 0x91,
// and that bit is not one of the rails the binding models. It matters on a power cycle and nowhere
// else, which is exactly why it took a while to find: with the PMIC still holding state from a
// previous boot, everything works, so the board looks fine until it is unplugged. From a cold start
// the touch controller does not answer I2C at all ("Read vendor ID error") and there is no touch.
//
// Written here rather than left out, because the alternative is a board that only works until the
// first power cycle. See the .dts for the rail declarations themselves.
constexpr uint8_t AXP2101_ADDRESS = 0x34;
constexpr uint8_t AXP2101_REG_LDO_ENABLE_1 = 0x91;

// Settling time between the expander pulse and the device starts that follow.
//
// Not cosmetic: on a cold boot the FT6336 failed to initialise ("Read vendor ID error") because it was
// addressed while still powering up - the PMIC rails only come up moments earlier. On a warm boot it
// worked, because the controller still had state from the previous run, which is exactly the kind of
// fault that only shows up on a real power cycle. The vendor's own touch driver waits 300 ms after
// its reset pulse for the same reason.
constexpr int POST_PULSE_SETTLE_MS = 300;

bool write_register(i2c_master_dev_handle_t device, uint8_t reg, uint8_t value) {
    const uint8_t payload[2] = { reg, value };
    return i2c_master_transmit(device, payload, sizeof(payload), I2C_TIMEOUT_MS) == ESP_OK;
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

    i2c_device_config_t device_config = {};
    device_config.dev_addr_length = I2C_ADDR_BIT_LEN_7;
    device_config.device_address = EXPANDER_ADDRESS;
    device_config.scl_speed_hz = 100000;

    // The PMIC rail bit the devicetree cannot express (see above). Done first, so the rail is up
    // before anything downstream of it is addressed.
    device_config.device_address = AXP2101_ADDRESS;
    i2c_master_dev_handle_t pmic = nullptr;
    if (i2c_master_bus_add_device(bus, &device_config, &pmic) == ESP_OK) {
        uint8_t enable_1 = 0;
        if (i2c_master_transmit_receive(pmic, (const uint8_t[]){ AXP2101_REG_LDO_ENABLE_1 }, 1,
                                        &enable_1, 1, I2C_TIMEOUT_MS) == ESP_OK) {
            write_register(pmic, AXP2101_REG_LDO_ENABLE_1, (uint8_t)(enable_1 | 0x01));
            LOG_I(TAG, "AXP2101 reg 0x91 |= 0x01 (rail the devicetree binding does not model)");
        }
        i2c_master_bus_rm_device(pmic);
    } else {
        LOG_W(TAG, "no AXP2101 at 0x%02x; a cold boot may leave the touch dead", AXP2101_ADDRESS);
    }

    device_config.device_address = EXPANDER_ADDRESS;

    i2c_master_dev_handle_t expander = nullptr;
    if (i2c_master_bus_add_device(bus, &device_config, &expander) == ESP_OK) {
        write_register(expander, EXPANDER_REG_DIRECTION, EXPANDER_PINS_0_1_5);
        write_register(expander, EXPANDER_REG_LEVEL, EXPANDER_PINS_0_1_5);
        vTaskDelay(pdMS_TO_TICKS(50));
        write_register(expander, EXPANDER_REG_LEVEL, EXPANDER_PINS_0_1);
        vTaskDelay(pdMS_TO_TICKS(50));
        write_register(expander, EXPANDER_REG_LEVEL, EXPANDER_PINS_0_1_5);
        i2c_master_bus_rm_device(expander);
        LOG_I(TAG, "CH32V003 expander pulsed (pins 0/1), panel reset released");
    } else {
        LOG_W(TAG, "no expander at 0x%02x; the panel may stay silent", EXPANDER_ADDRESS);
    }

    vTaskDelay(pdMS_TO_TICKS(POST_PULSE_SETTLE_MS));

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
