// SPDX-License-Identifier: Apache-2.0
#pragma once

#include <app/event.h>

/**
 * Internal event emitter used by app-module's scheduler to deliver APP_EVENT_RESULT to a parent
 * instance and APP_EVENT_CLOSE during app_scheduler_stop(). Not part of the public app API -
 * apps close themselves via app_event_emit_close().
 */
#ifdef __cplusplus
extern "C" {
#endif

error_t app_event_emit(AppInstanceId app_instance_id, const AppEvent* event);

#ifdef __cplusplus
}
#endif
