// SPDX-License-Identifier: Apache-2.0
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

#include <tactility/drivers/gpio.h>

struct Ft6x36Config {
    // Devicetree address hint. Unused by the driver: the FT6x36 always sits at a fixed
    // I2C address (0x38, see ESP_LCD_TOUCH_IO_I2C_FT6x36_ADDRESS).
    uint8_t address;
    uint16_t x_max;
    uint16_t y_max;
    bool swap_xy;
    bool mirror_x;
    bool mirror_y;
    struct GpioPinSpec pin_reset;
    struct GpioPinSpec pin_interrupt;
    // Panel geometry correction from the devicetree (see the binding): the raw coordinates that sit at
    // the display's edges. A pair with max <= min is ignored, so the default (all zero) leaves raw
    // coordinates untouched. Field order must match the binding's property order - the devicetree
    // compiler emits positional initializers.
    uint16_t raw_x_min;
    uint16_t raw_x_max;
    uint16_t raw_y_min;
    uint16_t raw_y_max;
};

#ifdef __cplusplus
}
#endif
