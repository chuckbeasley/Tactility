// SPDX-License-Identifier: Apache-2.0
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

struct Esp32GpioConfig {
    uint8_t gpioCount;
};

/**
 * Returns true if the given pin belongs to the MSPI bus that drives the on-package/on-module
 * flash and PSRAM.
 *
 * Reconfiguring such a pin as a general purpose GPIO detaches it from the MSPI IO MUX. When the
 * PSRAM chip select is detached, PSRAM stops responding and every read returns 0xFFFFFFFF. That
 * corrupts the PSRAM heap control block (which lives at the start of the PSRAM region), so the
 * next PSRAM allocation calls into TLSF with a 0xFFFFFFFF handle and panics with a load access
 * fault far away from the actual mistake.
 *
 * @param[in] pin Native ESP32 pin number
 */
bool esp32_gpio_is_mspi_pin(int pin);

#ifdef __cplusplus
}
#endif
