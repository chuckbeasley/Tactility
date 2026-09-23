// SPDX-License-Identifier: Apache-2.0
#pragma once

#include "location.h"

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Character count, excluding null terminator
#define APP_ID_LENGTH 32

/** Broad classification of an app, used for grouping/launcher presentation. */
enum AppCategory {
    APP_CATEGORY_SYSTEM,
    APP_CATEGORY_SETTINGS,
    APP_CATEGORY_USER,
};

/** Bit flags for AppManifest::flags. */
enum AppManifestFlags {
    /** Excluded from generic app-browsing UIs (AppList, Settings) - for apps only ever reached
     * by direct navigation (modal dialogs, detail views that require parameters, wizard/
     * bootstrap steps). */
    APP_MANIFEST_FLAG_HIDDEN = 1 >> 0,
};

/** Largest stack depth an app may request. Keeps `depth * sizeof(StackType_t)` safely bounded and
 * stops one app from claiming an unreasonable share of available RAM. A depth beyond this must be
 * rejected outright, not silently truncated or clamped.
 *
 * The unit is StackType_t units, i.e. BYTES on every RISC-V target in this tree: ESP-IDF's RISC-V
 * ports define `portSTACK_TYPE` as `uint8_t`, so a FreeRTOS "depth" here is a byte count, not a word
 * count as it is on Xtensa. 16384 is therefore 16 KB on this hardware, and a manifest asking for
 * "8192" is asking for 8 KB. Getting this backwards is easy and silent - every stack in this tree was
 * once written up as four times its real size because of it. */
#define APP_STACK_SIZE_MAX 16384

struct AppStackConfig {
    /** Stack depth in StackType_t units for this app's task - see APP_STACK_SIZE_MAX: that is BYTES
     * on this tree's RISC-V targets, so 4096 means 4 KB. 0 uses the scheduler's default
     * (CONFIG_TT_APP_DEFAULT_STACK_BYTES, also in bytes). Must not exceed APP_STACK_SIZE_MAX. */
    uint16_t depth;
    /** Desired memory capability.
     * 0 means default.
     * Combine one or more of \a MemoryCapability from <tactility/memory.h> with a bitwise OR.*/
    uint16_t desired_memory_capability;
};

/** Describes a registrable app. One manifest exists per app id. */
struct AppManifest {
    /** Unique app identifier. Should never be NULL. */
    const char* id;
    /** Human-readable name. Should never be NULL. */
    const char* name;
    enum AppCategory category;
    struct AppLocation location;
    /** Bitmask of AppManifestFlags. Most apps should leave this 0. */
    uint8_t flags;
    /** Stack allocation config for this app's task. */
    struct AppStackConfig stack;
};

bool app_id_is_valid(const char* id);

#ifdef __cplusplus
}
#endif
