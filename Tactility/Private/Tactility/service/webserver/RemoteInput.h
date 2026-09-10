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

/**
 * Reports that a mirror session has ended, so the input it injected can be cleaned up.
 *
 * The one thing that needs undoing is the on-screen keyboard: a remote tap on a text field opens it
 * exactly as a local tap would, and a client that then disappears leaves it sitting over the screen.
 *
 * Only acts if this session actually injected input - a client that only ever watched must not have
 * a keyboard closed underneath whoever is holding the device, since that keyboard may be theirs.
 *
 * Safe to call from any task, including a timer callback: widget work is done under the LVGL lock,
 * or handed to the LVGL task if that lock is busy at the time.
 */
void remoteInputNotifyMirrorStopped();

} // namespace tt::service::webserver
