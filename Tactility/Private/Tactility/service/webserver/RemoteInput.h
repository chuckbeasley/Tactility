// SPDX-License-Identifier: Apache-2.0
#pragma once

#include <stdint.h>

namespace tt::service::webserver {

/** Kind of remote pointer event. */
enum class RemoteInputType : uint8_t {
    Move,
    Press,
    Release
};

/**
 * Queues one remote pointer event, in device (panel) coordinates. Called from the web server's
 * task; safe to call from any task (does nothing until the indev has been registered).
 */
void remoteInputPush(RemoteInputType type, int32_t x, int32_t y);

/**
 * Queues one remote key press, using LVGL key codes: printable ASCII maps directly, and special
 * keys use the LV_KEY_* values (8 backspace, 9 tab, 10 enter, 27 escape, 127 delete, 17..20 the
 * arrow keys). The press and its matching release are reported to LVGL as two separate reads.
 *
 * Safe to call from any task (does nothing until the indevs have been registered).
 */
void remoteInputPushKey(uint32_t key);

/**
 * Registers the synthetic pointer and keypad indevs used to inject queued remote events into LVGL,
 * creating the queue mutex on first call. Idempotent, and safe to call on every received WebSocket
 * frame.
 *
 * The indevs are deliberately kept for the process lifetime rather than tied to a session: a client
 * can disappear without telling us (a closed browser tab produces no further frames), so there is
 * no reliable moment to tear them down. Both are inert while they have no events, and RemoteInput's
 * stuck-press watchdog covers the abandoned-press case.
 */
void remoteInputEnsureIndev();

} // namespace tt::service::webserver
