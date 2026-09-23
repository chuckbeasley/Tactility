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
#include <esp_timer.h>
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

// How often the driver's own task samples the controller, measured as a ~11 ms round (10 ms delay plus
// the bus read) and stretching to ~25 ms while the UI is redrawing. The input path asks for data far
// less often than that (measured: ~35 ms apart when idle, over 100 ms while redrawing), so polling this
// fast is what keeps a short tap or flick from falling entirely between two of its reads. One 17-byte
// read costs about 2 ms at 100 kHz.
constexpr int POLL_INTERVAL_MS = 10;

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

    // The controller is polled by this driver's own task rather than only when the input path asks,
    // because those two rates are far apart and the gap loses taps: measured on this board, the input
    // path calls read_data every ~35 ms when the UI is idle and over 100 ms while it redraws, so a
    // ~100 ms tap can be over before the first of those calls. The task samples the controller at
    // POLL_INTERVAL_MS and latches what it sees, so a tap that happened between two input-path reads
    // is still delivered.
    TaskHandle_t poll_task;
    portMUX_TYPE poll_lock;
    bool poll_task_stop;
    bool finger_down;
    bool latch_pending;
    uint16_t latched_x;
    uint16_t latched_y;

    // Where the current touch started, so its first report can be the position the finger landed on
    // even when the controller has already moved on by the time the input path asks.
    uint16_t gesture_start_x;
    uint16_t gesture_start_y;

    // Read path state (only touched by ft6x36_read_data, which the input path calls from one task).
    // published_* is what the input path was last told; a release is not reported while the controller
    // knows the finger moved somewhere else, because the movement has to reach the input path first -
    // see ft6x36_read_data().
    bool press_delivered;
    uint16_t published_x;
    uint16_t published_y;
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

// Reads the controller's touch block and latches what it says. Runs from the driver's polling task,
// several times per input-path read, so that a tap cannot fall between two samples.
static void ft6x36_poll_once(Ft6x36Internal* internal) {
    uint8_t block[TOUCH_BLOCK_SIZE] = {};
    if (esp_lcd_panel_io_rx_param(internal->io_handle, TOUCH_BLOCK_REGISTER, block, sizeof(block)) != ESP_OK) {
        return; // a failed read leaves the previous state alone rather than inventing a release
    }

    // 0x0F is not a count this controller can report, so an all-ones block (a bus that answered
    // nothing meaningful) lands here as "no touch" rather than as fifteen of them.
    const uint8_t raw_count = block[TOUCH_BLOCK_COUNT_OFFSET] & 0x0F;
    const uint8_t point_count = raw_count <= CONFIG_ESP_LCD_TOUCH_MAX_POINTS ? raw_count : 0;

    const uint16_t x = (uint16_t)(((block[TOUCH_BLOCK_FIRST_POINT_OFFSET] & 0x0F) << 8) |
                                  block[TOUCH_BLOCK_FIRST_POINT_OFFSET + 1]);
    const uint16_t y = (uint16_t)(((block[TOUCH_BLOCK_FIRST_POINT_OFFSET + 2] & 0x0F) << 8) |
                                  block[TOUCH_BLOCK_FIRST_POINT_OFFSET + 3]);

    portENTER_CRITICAL(&internal->poll_lock);
    if (point_count > 0) {
        if (!internal->finger_down) {
            internal->gesture_start_x = x;
            internal->gesture_start_y = y;
        }
        internal->finger_down = true;
        internal->latch_pending = true;
        internal->latched_x = x;
        internal->latched_y = y;
    } else {
        internal->finger_down = false;
    }
    portEXIT_CRITICAL(&internal->poll_lock);
}

static void ft6x36_poll_task(void* argument) {
    auto* internal = static_cast<Ft6x36Internal*>(argument);

    while (!internal->poll_task_stop) {
        ft6x36_poll_once(internal);
        vTaskDelay(pdMS_TO_TICKS(POLL_INTERVAL_MS));
    }

    vTaskDelete(nullptr);
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
    // The threshold is set to 32, after the component's own init has already written its 128 - so this
    // is the value the part ends up with.
    //
    // Measured on this board: 128 -> zero detections in 16 s of firm tapping on battery, 48 -> seven,
    // 32 -> more again, with nothing happening without a finger, and USB normal at all three.
    //
    // How fast the part reports is separate and was measured too: at the stock 0x88 = 0x0E a moving
    // finger produced a new position every ~11-25 ms when polled that often, so this driver does not
    // rely on any particular reading of that register's units. (An earlier note in this file read 0x0E
    // as 14 Hz / 71 ms; the same measurement contradicts it, which is why it is not repeated here.)
    //
    // 128 is not our invention and not a value we can blame on the stock driver: it is the part's own
    // power-on default. That was measured rather than assumed - toggling the LDOs showed ALDO2 is the
    // rail that feeds this controller, cutting it power-cycled the part for the first time in this
    // board's session here, and the next boot printed "FT6x36: Thresh: 128" from a read taken before
    // anything wrote to it.
    //
    // The consequence is worth stating plainly, because it is the reason this value is 32 and not 128:
    // the vendor's firmware writes nothing to this controller, so it runs at 128 on USB and on battery
    // alike - which is exactly the configuration measured to produce no touch at all on battery. Copying
    // the vendor byte for byte would reproduce the fault, not fix it. This deviation is load-bearing.
    const uint8_t touch_threshold = 32;
    const uint8_t stock_point_rate = 0x0E;
    const uint8_t stock_interrupt_mode = 0x01;
    const esp_err_t threshold_result =
        esp_lcd_panel_io_tx_param(internal->io_handle, 0x80, &touch_threshold, 1);
    const esp_err_t rate_result =
        esp_lcd_panel_io_tx_param(internal->io_handle, 0x88, &stock_point_rate, 1);
    const esp_err_t mode_result =
        esp_lcd_panel_io_tx_param(internal->io_handle, 0xA4, &stock_interrupt_mode, 1);
    if (threshold_result != ESP_OK || rate_result != ESP_OK || mode_result != ESP_OK) {
        LOG_W(TAG, "could not restore the controller's stock registers; leaving it as it is");
    } else {
        // Read back so the boot log states what the controller is actually configured with, rather than
        // what it was asked for. Diagnostic, and cheap: three register reads once per boot.
        uint8_t check_threshold = 0;
        uint8_t check_rate = 0;
        uint8_t check_mode = 0;
        esp_lcd_panel_io_rx_param(internal->io_handle, 0x80, &check_threshold, 1);
        esp_lcd_panel_io_rx_param(internal->io_handle, 0x88, &check_rate, 1);
        esp_lcd_panel_io_rx_param(internal->io_handle, 0xA4, &check_mode, 1);
        LOG_I(TAG, "registers: threshold=%u rate=%u interrupt_mode=%u", check_threshold, check_rate, check_mode);
    }

    // Print the panel geometry in force, so the boot log states the mapping rather than leaving it to be
    // inferred from where taps land. Zero ranges mean no rescaling (see the binding).
    LOG_I(TAG, "panel mapping: raw x %u..%u -> 0..%u, raw y %u..%u -> 0..%u", (unsigned)config->raw_x_min,
          (unsigned)config->raw_x_max, (unsigned)config->x_max, (unsigned)config->raw_y_min,
          (unsigned)config->raw_y_max, (unsigned)config->y_max);

    internal->poll_task = nullptr;
    internal->poll_task_stop = false;
    internal->finger_down = false;
    internal->latch_pending = false;
    internal->latched_x = 0;
    internal->latched_y = 0;
    internal->gesture_start_x = 0;
    internal->gesture_start_y = 0;
    internal->press_delivered = false;
    internal->published_x = 0;
    internal->published_y = 0;
    internal->poll_lock = (portMUX_TYPE)portMUX_INITIALIZER_UNLOCKED;

    device_set_driver_data(device, internal);

    // Below the input path and the apps: this observes the controller and must never be the reason a
    // touch is late. Small stack: one log line and one bus transaction per interval.
    if (xTaskCreate(ft6x36_poll_task, "ft6x36-poll", 3072, internal, 3, &internal->poll_task) != pdPASS) {
        LOG_W(TAG, "could not start the polling task; taps fall back to the input path's own rate");
        internal->poll_task = nullptr;
    }

    return ERROR_NONE;
}

static error_t stop(Device* device) {
    auto* internal = static_cast<Ft6x36Internal*>(device_get_driver_data(device));

    // Stop the polling task first: it is the only other user of the panel IO handle deleted below.
    if (internal->poll_task != nullptr) {
        internal->poll_task_stop = true;
        vTaskDelay(pdMS_TO_TICKS(POLL_INTERVAL_MS * 3));
        internal->poll_task = nullptr;
    }

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

// Rescales one raw axis onto the display, using the reachable rectangle the board declares. A pair with
// raw_max <= raw_min means the board did not declare one, and the raw value is passed through.
static uint16_t ft6x36_scale_axis(uint16_t value, uint16_t raw_min, uint16_t raw_max, uint16_t display_max) {
    if (raw_max <= raw_min || display_max == 0) {
        return value;
    }
    const int32_t span = (int32_t)raw_max - (int32_t)raw_min;
    int32_t mapped = ((int32_t)value - (int32_t)raw_min) * (int32_t)display_max / span;
    if (mapped < 0) {
        mapped = 0;
    } else if (mapped > (int32_t)display_max) {
        mapped = (int32_t)display_max;
    }
    return (uint16_t)mapped;
}

static error_t ft6x36_read_data(Device* device, TickType_t timeout) {
    (void)timeout; // esp_lcd_touch_read_data() has no timeout parameter
    auto* internal = static_cast<Ft6x36Internal*>(device_get_driver_data(device));

    // The controller itself is read by ft6x36_poll_task(), several times per call here, because this
    // call happens once per LVGL timer tick (~35 ms measured on this board when idle, over 100 ms while
    // it redraws) while the polling task runs every ~11 ms: anything that happened between two of these
    // calls used to be lost outright. What is left here is to publish the polled state, with one
    // addition - a touch the polling task saw but this path has not been told about yet is reported as a
    // press now, and the next call reports the release, so it arrives as a press/release pair instead of
    // never arriving at all.
    bool finger_down = false;
    bool unread_tap = false;
    uint16_t x = 0;
    uint16_t y = 0;
    uint16_t gesture_start_x = 0;
    uint16_t gesture_start_y = 0;

    portENTER_CRITICAL(&internal->poll_lock);
    finger_down = internal->finger_down;
    x = internal->latched_x;
    y = internal->latched_y;
    gesture_start_x = internal->gesture_start_x;
    gesture_start_y = internal->gesture_start_y;
    if (finger_down) {
        internal->latch_pending = false; // this press is being delivered now
    } else if (internal->latch_pending) {
        internal->latch_pending = false;
        unread_tap = true;
    }
    portEXIT_CRITICAL(&internal->poll_lock);

    // The input path reads this far slower than the controller scans (measured: ~35 ms apart when the
    // UI is idle, over 100 ms while it redraws, against a ~10 ms poll here), so a gesture can be over
    // before its movement has ever been reported. Reporting the release at that point loses the
    // movement entirely: the input path hands LVGL a press and a release at the same coordinates, and
    // LVGL - which decides "click or scroll" from how far its own samples moved - opens whatever is
    // under the finger instead of scrolling. A quick flick is a whole gesture of that kind.
    //
    // So a gesture is reported as: the position the finger landed on, then wherever it moved to, then
    // the release - and the release waits one read if the controller knows the finger moved since the
    // last thing reported. A tap is unaffected: nothing moved, so nothing is deferred, and the press
    // and release arrive as before.
    bool pressed = false;
    uint16_t out_x = 0;
    uint16_t out_y = 0;

    if (finger_down || unread_tap) {
        if (!internal->press_delivered) {
            out_x = gesture_start_x;
            out_y = gesture_start_y;
            internal->press_delivered = true;
        } else {
            out_x = x;
            out_y = y;
        }
        pressed = true;
    } else if (internal->press_delivered && (x != internal->published_x || y != internal->published_y)) {
        out_x = x;
        out_y = y;
        pressed = true;
    } else {
        internal->press_delivered = false;
    }

    if (pressed) {
        internal->published_x = out_x;
        internal->published_y = out_y;
    }

    auto* touch = internal->touch_handle;
    portENTER_CRITICAL(&touch->data.lock);
    touch->data.points = pressed ? 1 : 0;
    if (pressed) {
        const auto* config = GET_CONFIG(device);
        touch->data.coords[0].x = ft6x36_scale_axis(out_x, config->raw_x_min, config->raw_x_max, config->x_max);
        touch->data.coords[0].y = ft6x36_scale_axis(out_y, config->raw_y_min, config->raw_y_max, config->y_max);
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
