// SPDX-License-Identifier: Apache-2.0
#include "board_pmic_init.h"

namespace {

constexpr int I2C_TIMEOUT_MS = 100;

} // namespace

int board_axp_apply_init(i2c_master_dev_handle_t pmic_device) {
    if (pmic_device == nullptr) {
        return 0;
    }

    int applied = 0;
    for (size_t i = 0; i < BOARD_AXP_INIT_COUNT; ++i) {
        const BoardAxpWrite& entry = BOARD_AXP_INIT[i];

        uint8_t current = 0;
        if (i2c_master_transmit_receive(pmic_device, &entry.reg, 1, &current, 1, I2C_TIMEOUT_MS) != ESP_OK) {
            continue;
        }

        const uint8_t payload[2] = { entry.reg, (uint8_t)((current & entry.mask) | entry.value) };
        if (i2c_master_transmit(pmic_device, payload, sizeof(payload), I2C_TIMEOUT_MS) == ESP_OK) {
            ++applied;
        }
    }

    return applied;
}
