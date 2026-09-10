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
#include <lvgl/devices/keyboard.h>
#include <lvgl/lvgl.h>

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <freertos/task.h>

#include <tactility/log.h>

#include <atomic>

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

// Key presses use their own queue: a keypad indev reports one key per read, and the release of the
// previous key has to be reported before the next press (see keyReadCallback).
constexpr size_t KEY_QUEUE_CAPACITY = 16;

uint32_t key_queue[KEY_QUEUE_CAPACITY];
size_t key_head = 0;
size_t key_count = 0;
// Non-zero while a press has been reported but its matching release has not yet been.
uint32_t pending_release_key = 0;

lv_indev_t* key_indev = nullptr;

// On-device indicator: created with the indevs (under the LVGL lock) on the top layer, so it shows
// over whatever screen or app is active. Shown/hidden from the read callback, which runs on the
// LVGL task and can therefore touch widgets without taking the lock itself.
constexpr uint32_t BADGE_TIMEOUT_MS = 3000;

lv_obj_t* badge = nullptr;
bool badge_visible = false;
uint32_t last_remote_activity_ms = 0;

// Set whenever a remote event is actually queued, and consumed when a mirror session ends. It gates
// closing the on-screen keyboard: a client that only ever watched must not have a keyboard closed
// underneath the person holding the device, whose keyboard it may well be.
std::atomic<bool> remote_input_used {false};

// Set when a session ended but the LVGL lock could not be taken at that moment. The pointer indev's
// read callback runs on the LVGL task, so it can finish the job without taking the lock at all.
std::atomic<bool> keyboard_close_pending {false};

/**
 * Hides the on-screen keyboard. Must be called with the LVGL lock held, or from the LVGL task.
 *
 * A remote client taps a text field, LVGL focuses it, and the keyboard opens as it would for a local
 * tap - there is no separate "remote keyboard" state. So if the client then disappears, the keyboard
 * is left sitting over the screen, which is what this undoes. lvgl_software_keyboard_hide() is
 * idempotent and also drops the keyboard's pointer to the textarea, so calling it when the keyboard
 * is already hidden is harmless.
 */
void closeSoftwareKeyboard() {
    LvglSoftwareKeyboard* keyboard = lvgl_software_keyboard_get_last();
    if (keyboard == nullptr || keyboard->object == nullptr) {
        return; // This build has no on-screen keyboard, or it has already been torn down.
    }
    lvgl_software_keyboard_hide(keyboard);
    LOG_I(TAG, "Mirror session ended; on-screen keyboard closed");
}

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
            last_remote_activity_ms = last_event_ms;
        }

        remaining = queue_count;
        xSemaphoreGive(mutex);
    }

    // Keep the indicator up while remote input has been arriving, so whoever is holding the device
    // can see that someone else is driving it.
    if (badge != nullptr) {
        const bool active = (nowMs() - last_remote_activity_ms) < BADGE_TIMEOUT_MS;
        if (active != badge_visible) {
            if (active) {
                lv_obj_remove_flag(badge, LV_OBJ_FLAG_HIDDEN);
            } else {
                lv_obj_add_flag(badge, LV_OBJ_FLAG_HIDDEN);
            }
            badge_visible = active;
        }
    }

    // A session that ended while the LVGL lock was busy is finished here instead: this callback runs
    // on the LVGL task, so widgets can be touched with no lock at all. No mutex is held at this
    // point either.
    if (keyboard_close_pending.exchange(false)) {
        closeSoftwareKeyboard();
    }

    data->point.x = current_x;
    data->point.y = current_y;
    data->state = pressed ? LV_INDEV_STATE_PRESSED : LV_INDEV_STATE_RELEASED;
    // Ask LVGL to call us again immediately while events remain, instead of one event per refresh
    // period (which would stretch a short drag across seconds).
    data->continue_reading = remaining > 0;
}

void keyReadCallback(lv_indev_t* /*indev*/, lv_indev_data_t* data) {
    uint32_t key = 0;
    bool pressed = false;
    size_t queued = 0;
    bool more = false;

    if (mutex != nullptr && xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE) {
        if (pending_release_key != 0) {
            // Second half of the previous key: the release must be reported before anything new,
            // otherwise LVGL never sees a complete press/release pair and the key does nothing.
            key = pending_release_key;
            pending_release_key = 0;
            last_remote_activity_ms = nowMs();
        } else if (key_count > 0) {
            key = key_queue[key_head];
            key_head = (key_head + 1) % KEY_QUEUE_CAPACITY;
            key_count--;
            pending_release_key = key;
            pressed = true;
            last_remote_activity_ms = nowMs();
        }
        queued = key_count;
        more = (pending_release_key != 0) || (queued > 0);
        xSemaphoreGive(mutex);
    }

    data->key = key;
    data->state = pressed ? LV_INDEV_STATE_PRESSED : LV_INDEV_STATE_RELEASED;
    // Drain rather than one key per refresh period: a press and its release are two reads.
    data->continue_reading = more;
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
    remote_input_used.store(true, std::memory_order_relaxed);

    xSemaphoreGive(mutex);
}

void remoteInputPushKey(uint32_t key) {
    if (mutex == nullptr) {
        return; // Not registered yet: no WebSocket frame has arrived.
    }
    if (xSemaphoreTake(mutex, portMAX_DELAY) != pdTRUE) {
        return;
    }

    if (key_count == KEY_QUEUE_CAPACITY) {
        // Typing faster than LVGL can drain: drop the oldest keystroke.
        key_head = (key_head + 1) % KEY_QUEUE_CAPACITY;
        key_count--;
    }

    const size_t tail = (key_head + key_count) % KEY_QUEUE_CAPACITY;
    key_queue[tail] = key;
    key_count++;
    remote_input_used.store(true, std::memory_order_relaxed);

    xSemaphoreGive(mutex);
}

void remoteInputEnsureIndev() {
    if (indev != nullptr && key_indev != nullptr) {
        return; // Both already registered.
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
        LOG_W(TAG, "LVGL busy; the remote indevs will be registered on the next frame");
        return;
    }

    lv_display_t* display = lv_display_get_default();
    if (display != nullptr && bound_x == 0) {
        bound_x = static_cast<int16_t>(lv_display_get_horizontal_resolution(display) - 1);
        bound_y = static_cast<int16_t>(lv_display_get_vertical_resolution(display) - 1);
    }

    if (indev == nullptr) {
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

    if (key_indev == nullptr) {
        key_indev = lv_indev_create();
        if (key_indev != nullptr) {
            lv_indev_set_type(key_indev, LV_INDEV_TYPE_KEYPAD);
            lv_indev_set_read_cb(key_indev, keyReadCallback);
            lv_indev_set_display(key_indev, display);
            // Join the shared keyboard navigation group, so remote keys drive the focused widget
            // (and the on-screen keyboard) exactly like a hardware keyboard would.
            lvgl_hardware_keyboard_add_custom(key_indev);
            LOG_I(TAG, "Remote keypad indev registered");
        } else {
            LOG_E(TAG, "Failed to create remote keypad indev");
        }
    }

    if (badge == nullptr) {
        // Top layer, so it stays visible over every screen and app. Hidden until input arrives.
        badge = lv_label_create(lv_layer_top());
        if (badge != nullptr) {
            lv_label_set_text(badge, LV_SYMBOL_EYE_OPEN " REMOTE INPUT");
            lv_obj_set_style_bg_color(badge, lv_color_hex(0xB00020), LV_PART_MAIN);
            lv_obj_set_style_bg_opa(badge, LV_OPA_90, LV_PART_MAIN);
            lv_obj_set_style_text_color(badge, lv_color_white(), LV_PART_MAIN);
            lv_obj_set_style_pad_all(badge, 4, LV_PART_MAIN);
            lv_obj_set_style_radius(badge, 4, LV_PART_MAIN);
            lv_obj_align(badge, LV_ALIGN_TOP_MID, 0, 24);
            lv_obj_add_flag(badge, LV_OBJ_FLAG_HIDDEN);
            badge_visible = false;
            LOG_I(TAG, "On-device remote-input indicator created");
        } else {
            LOG_E(TAG, "Failed to create on-device remote-input indicator");
        }
    }

    lvgl_unlock();
}

void remoteInputNotifyMirrorStopped() {
    // Only act if this session actually injected input. A client that merely watched never caused
    // the keyboard to open, so closing it would be closing someone else's.
    if (!remote_input_used.exchange(false)) {
        return;
    }

    if (!lvgl_try_lock(pdMS_TO_TICKS(500))) {
        // The LVGL task is mid-render. Leave it to the read callback rather than blocking here: this
        // runs on the timer task, and the session-end notification is one-shot.
        LOG_W(TAG, "LVGL busy; on-screen keyboard will be closed on the next indev read");
        keyboard_close_pending.store(true);
        return;
    }

    closeSoftwareKeyboard();
    lvgl_unlock();
}

} // namespace tt::service::webserver
