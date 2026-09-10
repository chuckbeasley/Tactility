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
 * Registers the synthetic LV_INDEV_TYPE_POINTER indev used to inject queued remote events into
 * LVGL, creating the queue mutex on first call. Idempotent, and safe to call on every WebSocket
 * handshake.
 *
 * The indev is deliberately kept for the process lifetime rather than tied to a session: a client
 * can disappear without telling us (a closed browser tab produces no further frames), so there is
 * no reliable moment to tear it down. A permanently registered pointer indev is inert while it has
 * no events, and RemoteInput's stuck-press watchdog covers the abandoned-press case.
 */
void remoteInputEnsureIndev();

} // namespace tt::service::webserver
