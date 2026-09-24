// SPDX-License-Identifier: Apache-2.0

/**
 * @brief Generic string key-value ".properties" file.
 */
#pragma once

#include <stdbool.h>
#include <stddef.h>

#include <tactility/error.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Opaque handle - open with properties_file_open(), release with properties_file_close().
 */
typedef struct PropertiesFile PropertiesFile;

/**
 * Open a properties file at @a path. The file is read into memory now; changes
 * made with properties_file_set() are only written back to disk by properties_file_close() -
 * and only if something was set, so opening a file to read it never writes to it.
 * @param[in] path absolute or relative file path (e.g. "/data/settings.properties") - the
 * parent directory must already exist
 * @return the new instance, or NULL on allocation failure, or NULL if @a path exists but a
 * genuine error prevented opening or reading it, e.g. a permissions error (a missing file is
 * not an error - the instance starts out empty in that case, and stays that way unless something
 * is set)
 */
PropertiesFile* properties_file_open(const char* path);

/**
 * Writes any pending properties_file_set() changes to the backing file (via a temporary file in
 * the same directory, which replaces the real path - and the previous content is moved aside rather
 * than deleted first, so a failure at any point leaves the previous file in place instead of no file
 * at all), then releases the instance either way. An instance on which properties_file_set() was
 * never called writes nothing.
 * @retval ERROR_NONE the backing file was fully updated, or there was nothing to write
 * @retval ERROR_RESOURCE writing failed (full filesystem, I/O error, ...) - the previous
 * on-disk content, if any, is unchanged; the in-memory changes are lost along with the instance
 */
error_t properties_file_close(PropertiesFile* file);

bool properties_file_has(const PropertiesFile* file, const char* key);

/**
 * @retval ERROR_NOT_FOUND @a key is absent - @a out_value is left untouched
 * @retval ERROR_BUFFER_OVERFLOW out_value_size is too small - @a out_value is left untouched
 * @retval ERROR_NONE on success
 */
error_t properties_file_get(const PropertiesFile* file, const char* key, char* out_value, size_t out_value_size);

/** Sets the value in the in-memory cache; only persisted to the backing file by
 * properties_file_close(). */
void properties_file_set(PropertiesFile* file, const char* key, const char* value);

typedef void (*PropertiesFileVisitorFn)(const char* key, const char* value, void* context);

/** Invokes @a visitor for every key currently cached, in unspecified order. */
void properties_file_for_each(const PropertiesFile* file, PropertiesFileVisitorFn visitor, void* context);

#ifdef __cplusplus
}
#endif
