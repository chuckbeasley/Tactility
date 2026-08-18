// SPDX-License-Identifier: Apache-2.0
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <tactility/device.h>
#include <tactility/error.h>
#include <tactility/freertos/freertos.h>

#include <tactility/drivers/gpio.h>

struct Xpt2046SoftSpiConfig {
    struct GpioPinSpec pin_mosi;
    struct GpioPinSpec pin_miso;
    struct GpioPinSpec pin_sck;
    struct GpioPinSpec pin_cs;
    uint16_t x_max;
    uint16_t y_max;
    bool swap_xy;
    bool mirror_x;
    bool mirror_y;
    /** Expose a power-supply child device that reads battery voltage/capacity off the chip's v-bat input */
    bool power_supply;
    /** Battery voltage (mV) considered 100% capacity, used to derive POWER_SUPPLY_PROP_CAPACITY */
    uint32_t power_supply_reference_voltage_mv;
};

/**
 * Reads one debounced raw touch sample using Touch_Calibrate-style validation
 * (pressure stabilization + two close raw reads). Coordinates are raw ADC
 * values in controller axis space.
 */
error_t xpt2046_softspi_read_valid_touch(struct Device* device, uint16_t* x, uint16_t* y, uint16_t threshold, TickType_t timeout);

#ifdef __cplusplus
}
#endif
