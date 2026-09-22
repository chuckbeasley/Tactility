// SPDX-License-Identifier: Apache-2.0
#include <drivers/ft6x36.h>

#include <ft6x36_module.h>

#include <tactility/check.h>
#include <tactility/device.h>
#include <tactility/driver.h>
#include <tactility/drivers/esp32_i2c.h>
#include <tactility/drivers/esp32_i2c_master.h>
#include <tactility/drivers/gpio.h>
#include <tactility/drivers/gpio_controller.h>
#include <tactility/drivers/i2c_controller.h>
#include <tactility/drivers/pointer.h>
#include <tactility/log.h>

#include <esp_err.h>
#include <driver/gpio.h>
#include <driver/i2c_master.h>
#include <esp_lcd_io_i2c.h>
#include <esp_lcd_panel_io.h>
#include <esp_lcd_touch.h>
#include <esp_lcd_touch_ft6x36.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <cstdlib>

#define TAG "FT6x36"
#define GET_CONFIG(device) (static_cast<const Ft6x36Config*>((device)->config))

// Power-up settle before the controller is first addressed, for boards with no reset pin (see
// pulse_reset). The vendor's own driver for this part waits the same 300 ms after its reset pulse.
constexpr int POWER_UP_SETTLE_MS = 300;

// The controller's touch block: register 0x00 upwards, one transaction, containing the point count at
// offset 0x02 and up to two sets of coordinates at 0x03..0x08. The same block the component's read
// fetches as its second half.
constexpr uint8_t TOUCH_BLOCK_REGISTER = 0x00;
constexpr size_t TOUCH_BLOCK_SIZE = 16;
constexpr uint8_t TOUCH_BLOCK_COUNT_OFFSET = 0x02;
constexpr uint8_t TOUCH_BLOCK_FIRST_POINT_OFFSET = 0x03;
constexpr size_t TOUCH_POINT_STRIDE = 6;

struct Ft6x36Internal {
    esp_lcd_panel_io_handle_t io_handle;
    esp_lcd_touch_handle_t touch_handle;
    // Non-null when pin_reset is configured. Owned/pulsed by this driver instead of esp_lcd_touch
    GpioDescriptor* reset_descriptor;
};

// Only valid for pin_interrupt: esp_lcd_touch only ever reads this pin's level / attaches an ISR
// to it via ESP-IDF's native gpio_* calls, so - like ili9341-module's cs/dc pins - it must be a
// real ESP32 GPIO. pin_reset has no such requirement and must never go through this helper; see
// pulse_reset() instead.
static inline gpio_num_t pin_or_nc(const struct GpioPinSpec& pin) {
    return pin.gpio_controller == nullptr ? GPIO_NUM_NC : static_cast<gpio_num_t>(pin.pin);
}

// See ili9341-module's pulse_reset() for the full rationale; same idea, same 10ms/10ms timing.
// esp_lcd_touch's rst_gpio_num is always left at GPIO_NUM_NC (see start()), under which it just
// skips its own reset step entirely.
//
// When there is no reset pin at all this still waits, because the wait is not only about the reset
// pulse: it is also the controller's power-up time. On boards where the touch supply comes from a
// power-management device declared just above it in the devicetree, this driver is started within
// milliseconds of that rail coming up, and the FT6336 does not answer I2C until it has settled -
// which shows up as "Read vendor ID error" and a touch panel that simply does not work. Where there
// is a reset pin, the delay below already covers this.
static error_t pulse_reset(GpioDescriptor* descriptor) {
    if (descriptor == nullptr) {
        vTaskDelay(pdMS_TO_TICKS(POWER_UP_SETTLE_MS));
        return ERROR_NONE;
    }
    // Logical high (physical low, because of earlier GPIO_FLAG_ACTIVE_LOW)
    error_t error = gpio_descriptor_set_level(descriptor, true);
    if (error != ERROR_NONE) {
        return error;
    }
    vTaskDelay(pdMS_TO_TICKS(50));
    // Logical low (physical high, because of earlier GPIO_FLAG_ACTIVE_LOW)
    error = gpio_descriptor_set_level(descriptor, false);
    if (error != ERROR_NONE) {
        return error;
    }
    vTaskDelay(pdMS_TO_TICKS(300));
    return ERROR_NONE;
}

// region Driver lifecycle

// FT6x36 always sits at a fixed I2C address, unlike GT911's strapping-dependent address,
// so no bus probing is needed here.
static esp_err_t create_io_handle(Device* parent, esp_lcd_panel_io_handle_t* out_handle) {
    esp_lcd_panel_io_i2c_config_t io_config = ESP_LCD_TOUCH_IO_I2C_FT6x36_CONFIG();

    auto* parent_driver = device_get_driver(parent);
    // IDF 6.1 declares only esp_lcd_new_panel_io_i2c(i2c_master_bus_handle_t, ...): the port-based
    // _v1 and the _v2 spellings this used to call are gone, so the bus handle is what both paths
    // need. That is also why the modern "espressif,esp32-i2c-master" controller (which installs the
    // new driver with i2c_new_master_bus) is the one to bind against.
    if (driver_is_compatible(parent_driver, "espressif,esp32-i2c-master")) {
        auto bus = esp32_i2c_master_get_bus_handle(parent);
        io_config.scl_speed_hz = esp32_i2c_master_get_clock_frequency(parent);
        return esp_lcd_new_panel_io_i2c(bus, &io_config, out_handle);
    }
    if (driver_is_compatible(parent_driver, "espressif,esp32-i2c")) {
        // The legacy controller installs the deprecated I2C driver (i2c_driver_install) rather than a
        // master bus, so there is usually no handle to find. Try anyway, and say plainly what to do
        // about it instead of failing somewhere less obvious later.
        auto port = static_cast<const Esp32I2cConfig*>(parent->config)->port;
        i2c_master_bus_handle_t bus = nullptr;
        const esp_err_t error = i2c_master_get_bus_handle(port, &bus);
        if (error != ESP_OK) {
            LOG_E(TAG, "espressif,esp32-i2c has no I2C master bus handle on IDF 6.1 - "
                       "use espressif,esp32-i2c-master for this controller");
            return error;
        }
        return esp_lcd_new_panel_io_i2c(bus, &io_config, out_handle);
    }

    LOG_E(TAG, "Unsupported I2C driver");
    return ESP_ERR_NOT_SUPPORTED;
}

static error_t start(Device* device) {
    auto* parent = device_get_parent(device);
    check(device_get_type(parent) == &I2C_CONTROLLER_TYPE);

    const auto* config = GET_CONFIG(device);

    auto* internal = static_cast<Ft6x36Internal*>(malloc(sizeof(Ft6x36Internal)));
    if (internal == nullptr) {
        return ERROR_OUT_OF_MEMORY;
    }

    internal->reset_descriptor = nullptr;
    if (config->pin_reset.gpio_controller != nullptr) {
        internal->reset_descriptor = gpio_descriptor_acquire(config->pin_reset.gpio_controller, config->pin_reset.pin, GPIO_FLAG_DIRECTION_OUTPUT | GPIO_FLAG_ACTIVE_LOW, GPIO_OWNER_GPIO);
        if (internal->reset_descriptor == nullptr) {
            LOG_E(TAG, "Failed to acquire reset GPIO descriptor");
            free(internal);
            return ERROR_RESOURCE;
        }
    }

    esp_err_t ret = create_io_handle(parent, &internal->io_handle);
    if (ret != ESP_OK) {
        if (internal->reset_descriptor != nullptr) {
            gpio_descriptor_release(internal->reset_descriptor);
        }
        free(internal);
        return ERROR_RESOURCE;
    }

    if (pulse_reset(internal->reset_descriptor) != ERROR_NONE) {
        LOG_E(TAG, "Failed to pulse reset pin");
        esp_lcd_panel_io_del(internal->io_handle);
        if (internal->reset_descriptor != nullptr) {
            gpio_descriptor_release(internal->reset_descriptor);
        }
        free(internal);
        return ERROR_RESOURCE;
    }

    esp_lcd_touch_config_t touch_config = {
        .x_max = config->x_max,
        .y_max = config->y_max,
        // Always NC: pulse_reset() above already handled the physical pin (see its comment for
        // why); esp_lcd_touch just skips its own no-op reset step when this is NC.
        .rst_gpio_num = GPIO_NUM_NC,
        .int_gpio_num = pin_or_nc(config->pin_interrupt),
        // FT6x36's reset and interrupt lines are both fixed active-low in hardware.
        .levels = {
            .reset = 0u,
            .interrupt = 0u,
        },
        .flags = {
            .swap_xy = config->swap_xy ? 1u : 0u,
            .mirror_x = config->mirror_x ? 1u : 0u,
            .mirror_y = config->mirror_y ? 1u : 0u,
        },
        .process_coordinates = nullptr,
        .interrupt_callback = nullptr,
        .user_data = nullptr,
        .driver_data = nullptr,
    };

    ret = esp_lcd_touch_new_i2c_ft6x36(internal->io_handle, &touch_config, &internal->touch_handle);
    if (ret != ESP_OK) {
        LOG_E(TAG, "Failed to create touch handle: %s", esp_err_to_name(ret));
        esp_lcd_panel_io_del(internal->io_handle);
        if (internal->reset_descriptor != nullptr) {
            gpio_descriptor_release(internal->reset_descriptor);
        }
        free(internal);
        return ERROR_RESOURCE;
    }

    // Put the controller's configuration registers back to their stock values explicitly.
    //
    // This part has no reset pin on this board (see pulse_reset), so nothing hardware-resets it between
    // boots: it keeps whatever registers it was last given, across a reflash, a reboot and even a
    // firmware change. Two experiments on this board wrote non-stock values into it (interrupt mode
    // 0xA4 = 0 and scan rate 0x88 = 60) and the misses they caused survived the reverting commit,
    // because the revert had nothing to undo them with. The component's own init writes 0x00, 0x80 and
    // 0x88 but never 0xA4, so the interrupt mode in particular was left at whatever it had been.
    //
    // The values here are the ones measured on a freshly powered controller: 0x80 = 128 (threshold),
    // 0x88 = 0x0E (14 Hz, the component's own value) and 0xA4 = 1 (trigger mode). Power mode (0xA5) is
    // deliberately left alone: the controller manages that itself, and it was 0x01 (monitor) when idle
    // on a fresh part, which is normal.
    const uint8_t stock_threshold = 128;
    const uint8_t stock_point_rate = 0x0E;
    const uint8_t stock_interrupt_mode = 0x01;
    const esp_err_t threshold_result =
        esp_lcd_panel_io_tx_param(internal->io_handle, 0x80, &stock_threshold, 1);
    const esp_err_t rate_result =
        esp_lcd_panel_io_tx_param(internal->io_handle, 0x88, &stock_point_rate, 1);
    const esp_err_t mode_result =
        esp_lcd_panel_io_tx_param(internal->io_handle, 0xA4, &stock_interrupt_mode, 1);
    if (threshold_result != ESP_OK || rate_result != ESP_OK || mode_result != ESP_OK) {
        LOG_W(TAG, "could not restore the controller's stock registers; leaving it as it is");
    }

    device_set_driver_data(device, internal);
    return ERROR_NONE;
}

static error_t stop(Device* device) {
    auto* internal = static_cast<Ft6x36Internal*>(device_get_driver_data(device));

    // esp_lcd_touch_del() only releases the touch-side resources; the panel IO handle is owned
    // separately and needs its own deletion.
    if (internal->touch_handle != nullptr) {
        if (esp_lcd_touch_del(internal->touch_handle) != ESP_OK) {
            LOG_E(TAG, "Failed to delete touch handle");
            return ERROR_RESOURCE;
        }
        internal->touch_handle = nullptr;
    }

    if (internal->io_handle != nullptr) {
        if (esp_lcd_panel_io_del(internal->io_handle) != ESP_OK) {
            LOG_E(TAG, "Failed to delete panel IO handle");
            return ERROR_RESOURCE;
        }
        internal->io_handle = nullptr;
    }

    if (internal->reset_descriptor != nullptr) {
        gpio_descriptor_release(internal->reset_descriptor);
        internal->reset_descriptor = nullptr;
    }

    free(internal);
    device_set_driver_data(device, nullptr);
    return ERROR_NONE;
}

// endregion

// region PointerApi

static error_t ft6x36_enter_sleep(Device* device) {
    auto* internal = static_cast<Ft6x36Internal*>(device_get_driver_data(device));
    return esp_lcd_touch_enter_sleep(internal->touch_handle) == ESP_OK ? ERROR_NONE : ERROR_RESOURCE;
}

static error_t ft6x36_exit_sleep(Device* device) {
    auto* internal = static_cast<Ft6x36Internal*>(device_get_driver_data(device));
    return esp_lcd_touch_exit_sleep(internal->touch_handle) == ESP_OK ? ERROR_NONE : ERROR_RESOURCE;
}

static error_t ft6x36_read_data(Device* device, TickType_t timeout) {
    (void)timeout; // esp_lcd_touch_read_data() has no timeout parameter
    auto* internal = static_cast<Ft6x36Internal*>(device_get_driver_data(device));

    // One transaction, one coherent snapshot: the count byte and the coordinates come out of the same
    // block read, so they cannot disagree with each other, and no other reader has to touch the
    // controller first.
    //
    // The stock component reads the other way round - the count register first, returning immediately
    // if it reads zero, and only then the block - which on battery power disagreed with itself in 14 of
    // 98 samples (never once on USB, in 63) and silently threw the touch away every time it did. The
    // next attempt let the component read first and fetched the block only when it reported nothing:
    // that fixed battery but cost missed taps on USB, because on this part a read consumes the
    // detection it just reported, so a second reader steals the first one's touch. A single read by a
    // single reader has neither problem.
    uint8_t block[TOUCH_BLOCK_SIZE] = {};
    if (esp_lcd_panel_io_rx_param(internal->io_handle, TOUCH_BLOCK_REGISTER, block, sizeof(block)) != ESP_OK) {
        // A failed read must not leave the previous touch standing as if it were current.
        auto* failed = internal->touch_handle;
        portENTER_CRITICAL(&failed->data.lock);
        failed->data.points = 0;
        portEXIT_CRITICAL(&failed->data.lock);
        return ERROR_RESOURCE;
    }

    // 0x0F is not a count this controller can report, so a block of all-ones (a bus that answered
    // nothing meaningful) lands here as "no touch" rather than as fifteen of them.
    const uint8_t raw_count = block[TOUCH_BLOCK_COUNT_OFFSET] & 0x0F;
    const uint8_t point_count = raw_count <= CONFIG_ESP_LCD_TOUCH_MAX_POINTS ? raw_count : 0;

    auto* touch = internal->touch_handle;
    portENTER_CRITICAL(&touch->data.lock);
    touch->data.points = point_count;
    for (uint8_t i = 0; i < point_count; ++i) {
        const size_t offset = TOUCH_BLOCK_FIRST_POINT_OFFSET + (i * TOUCH_POINT_STRIDE);
        touch->data.coords[i].x = (uint16_t)(((block[offset] & 0x0F) << 8) | block[offset + 1]);
        touch->data.coords[i].y = (uint16_t)(((block[offset + 2] & 0x0F) << 8) | block[offset + 3]);
    }
    portEXIT_CRITICAL(&touch->data.lock);

    return ERROR_NONE;
}

static bool ft6x36_get_touched_points(Device* device, uint16_t* x, uint16_t* y, uint16_t* strength, uint8_t* point_count, uint8_t max_point_count) {
    auto* internal = static_cast<Ft6x36Internal*>(device_get_driver_data(device));
    return esp_lcd_touch_get_coordinates(internal->touch_handle, x, y, strength, point_count, max_point_count);
}

static error_t ft6x36_set_swap_xy(Device* device, bool swap) {
    auto* internal = static_cast<Ft6x36Internal*>(device_get_driver_data(device));
    return esp_lcd_touch_set_swap_xy(internal->touch_handle, swap) == ESP_OK ? ERROR_NONE : ERROR_RESOURCE;
}

static error_t ft6x36_get_swap_xy(Device* device, bool* swap) {
    auto* internal = static_cast<Ft6x36Internal*>(device_get_driver_data(device));
    return esp_lcd_touch_get_swap_xy(internal->touch_handle, swap) == ESP_OK ? ERROR_NONE : ERROR_RESOURCE;
}

static error_t ft6x36_set_mirror_x(Device* device, bool mirror) {
    auto* internal = static_cast<Ft6x36Internal*>(device_get_driver_data(device));
    return esp_lcd_touch_set_mirror_x(internal->touch_handle, mirror) == ESP_OK ? ERROR_NONE : ERROR_RESOURCE;
}

static error_t ft6x36_get_mirror_x(Device* device, bool* mirror) {
    auto* internal = static_cast<Ft6x36Internal*>(device_get_driver_data(device));
    return esp_lcd_touch_get_mirror_x(internal->touch_handle, mirror) == ESP_OK ? ERROR_NONE : ERROR_RESOURCE;
}

static error_t ft6x36_set_mirror_y(Device* device, bool mirror) {
    auto* internal = static_cast<Ft6x36Internal*>(device_get_driver_data(device));
    return esp_lcd_touch_set_mirror_y(internal->touch_handle, mirror) == ESP_OK ? ERROR_NONE : ERROR_RESOURCE;
}

static error_t ft6x36_get_mirror_y(Device* device, bool* mirror) {
    auto* internal = static_cast<Ft6x36Internal*>(device_get_driver_data(device));
    return esp_lcd_touch_get_mirror_y(internal->touch_handle, mirror) == ESP_OK ? ERROR_NONE : ERROR_RESOURCE;
}

// endregion

static const PointerApi ft6x36_pointer_api = {
    .enter_sleep = ft6x36_enter_sleep,
    .exit_sleep = ft6x36_exit_sleep,
    .read_data = ft6x36_read_data,
    .get_touched_points = ft6x36_get_touched_points,
    .set_swap_xy = ft6x36_set_swap_xy,
    .get_swap_xy = ft6x36_get_swap_xy,
    .set_mirror_x = ft6x36_set_mirror_x,
    .get_mirror_x = ft6x36_get_mirror_x,
    .set_mirror_y = ft6x36_set_mirror_y,
    .get_mirror_y = ft6x36_get_mirror_y,
};

Driver ft6x36_driver = {
    .name = "ft6x36",
    .compatible = (const char*[]) { "focaltech,ft6x36", nullptr },
    .start_device = start,
    .stop_device = stop,
    .api = &ft6x36_pointer_api,
    .device_type = &POINTER_TYPE,
    .owner = &ft6x36_module,
    .internal = nullptr
};
