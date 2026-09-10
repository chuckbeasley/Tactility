// SPDX-License-Identifier: Apache-2.0
//
// Injects remote pointer events (from the /ws/remote WebSocket) into LVGL through a synthetic
// LV_INDEV_TYPE_POINTER indev - the same shape the USB and Bluetooth HID mice already use.
//
// The web server's task queues events; LVGL's indev read callback - which runs on the LVGL task -
// drains them. Only the indev registration touches LVGL from the web server's task, and it does so
// under the LVGL lock, so the injection path never needs to hold that lock itself.

#include <Tactility/service/webserver/RemoteInput.h>

#include <Tactility/lvgl/Lvgl.h>
#include <lvgl/lvgl.h>

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <freertos/task.h>

#include <tactility/log.h>

namespace tt::service::webserver {

namespace {

constexpr auto* TAG = "RemoteInput";

// Deep enough to absorb a burst of drag samples arriving between two indev reads. The read
// callback also sets continue_reading, so LVGL drains the queue faster than the refresh period.
constexpr size_t QUEUE_CAPACITY = 32;

// A client whose tab is closed sends no "release" and then no further frames at all, so a pending
// press would stick forever. Drop it once nothing has arrived for this long.
constexpr uint32_t STUCK_PRESS_TIMEOUT_MS = 600;

struct Event {
    RemoteInputType type;
    int16_t x;
    int16_t y;
};

SemaphoreHandle_t mutex = nullptr;
Event queue[QUEUE_CAPACITY];
size_t queue_head = 0;
size_t queue_count = 0;

// Pointer state as last reported to LVGL.
bool pressed = false;
int16_t current_x = 0;
int16_t current_y = 0;
uint32_t last_event_ms = 0;

// Panel bounds (max valid coordinate), cached when the indev is created.
int16_t bound_x = 0;
int16_t bound_y = 0;

lv_indev_t* indev = nullptr;

uint32_t nowMs() {
    return static_cast<uint32_t>(xTaskGetTickCount()) * portTICK_PERIOD_MS;
}

void readCallback(lv_indev_t* /*indev*/, lv_indev_data_t* data) {
    size_t remaining = 0;

    if (mutex != nullptr && xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE) {
        // Stuck-press watchdog (see STUCK_PRESS_TIMEOUT_MS): treat silence as the client going away.
        if (pressed && (nowMs() - last_event_ms) > STUCK_PRESS_TIMEOUT_MS) {
            LOG_W(TAG, "No input for %u ms while pressed; forcing release", (unsigned)STUCK_PRESS_TIMEOUT_MS);
            pressed = false;
        }

        if (queue_count > 0) {
            Event event = queue[queue_head];
            queue_head = (queue_head + 1) % QUEUE_CAPACITY;
            queue_count--;

            // Coalesce a run of moves into the newest one so a drag can't lag behind the queue -
            // but never merge across a press/release, because LVGL recognises a tap by seeing the
            // press and the release in separate reads.
            if (event.type == RemoteInputType::Move) {
                while (queue_count > 0 && queue[queue_head].type == RemoteInputType::Move) {
                    event = queue[queue_head];
                    queue_head = (queue_head + 1) % QUEUE_CAPACITY;
                    queue_count--;
                }
            }

            if (event.x < 0) event.x = 0;
            else if (bound_x > 0 && event.x > bound_x) event.x = bound_x;
            if (event.y < 0) event.y = 0;
            else if (bound_y > 0 && event.y > bound_y) event.y = bound_y;

            current_x = event.x;
            current_y = event.y;
            if (event.type == RemoteInputType::Press) pressed = true;
            else if (event.type == RemoteInputType::Release) pressed = false;
            last_event_ms = nowMs();
        }

        remaining = queue_count;
        xSemaphoreGive(mutex);
    }

    data->point.x = current_x;
    data->point.y = current_y;
    data->state = pressed ? LV_INDEV_STATE_PRESSED : LV_INDEV_STATE_RELEASED;
    // Ask LVGL to call us again immediately while events remain, instead of one event per refresh
    // period (which would stretch a short drag across seconds).
    data->continue_reading = remaining > 0;
}

} // namespace

void remoteInputPush(RemoteInputType type, int32_t x, int32_t y) {
    if (mutex == nullptr) {
        return; // Not registered yet: no WebSocket handshake has happened.
    }
    if (xSemaphoreTake(mutex, portMAX_DELAY) != pdTRUE) {
        return;
    }

    if (queue_count == QUEUE_CAPACITY) {
        // Consumer is behind - drop the oldest sample, only the newest position matters. Press and
        // release are never dropped unless the queue is completely full, which only happens while
        // the LVGL task is blocked for a long time.
        queue_head = (queue_head + 1) % QUEUE_CAPACITY;
        queue_count--;
    }

    const size_t tail = (queue_head + queue_count) % QUEUE_CAPACITY;
    queue[tail] = Event { type, static_cast<int16_t>(x), static_cast<int16_t>(y) };
    queue_count++;

    xSemaphoreGive(mutex);
}

void remoteInputEnsureIndev() {
    if (indev != nullptr) {
        return;
    }

    if (mutex == nullptr) {
        mutex = xSemaphoreCreateMutex();
        if (mutex == nullptr) {
            LOG_E(TAG, "Failed to create queue mutex");
            return;
        }
    }

    // Every indev in this codebase is created under the LVGL lock.
    if (!lvgl_try_lock(pdMS_TO_TICKS(500))) {
        LOG_W(TAG, "LVGL busy; the remote pointer indev will be registered on the next handshake");
        return;
    }

    if (indev == nullptr) {
        lv_display_t* display = lv_display_get_default();
        if (display != nullptr) {
            bound_x = static_cast<int16_t>(lv_display_get_horizontal_resolution(display) - 1);
            bound_y = static_cast<int16_t>(lv_display_get_vertical_resolution(display) - 1);
        }

        indev = lv_indev_create();
        if (indev != nullptr) {
            lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
            lv_indev_set_read_cb(indev, readCallback);
            lv_indev_set_display(indev, display);
            LOG_I(TAG, "Remote pointer indev registered (%dx%d)", bound_x + 1, bound_y + 1);
        } else {
            LOG_E(TAG, "Failed to create remote pointer indev");
        }
    }

    lvgl_unlock();
}

} // namespace tt::service::webserver
