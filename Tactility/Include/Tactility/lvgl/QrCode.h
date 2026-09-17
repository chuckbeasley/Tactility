// SPDX-License-Identifier: Apache-2.0
#pragma once

#include <lvgl.h>

#include <cstdint>
#include <string>

namespace tt::lvgl {

/**
 * Draw @a text as a QR code into a canvas created inside @a parent.
 *
 * The code is scaled in whole-pixel modules, so the canvas is the largest integer multiple of the
 * module size that still fits within @a max_size, and can be smaller than it.
 *
 * Lives here rather than in the app that needed it first because two apps now need it: the crash
 * reporter shows a link to its report, and the radar screen shows a link to the weather service's
 * map. The alignment of the returned canvas is left to the caller.
 *
 * @param[in] parent widget to create the canvas in
 * @param[in] text payload, encoded as bytes, so any UTF-8 is fine
 * @param[in] max_size the largest square, in pixels, that the code may occupy
 * @param[in] dark_on_light draw dark modules on a light background, which is the orientation the
 *            QR standard specifies. Left configurable because the crash reporter has always drawn
 *            the inverse, and changing how that screen looks is not this helper's business
 * @param[out] out_size optional; the side length actually used, or 0 on failure
 * @return the canvas, or nullptr when the payload is too large to encode at all or a single module
 *         would not fit within @a max_size
 */
lv_obj_t* qr_code_create(lv_obj_t* parent, const std::string& text, int32_t max_size, bool dark_on_light = false, int32_t* out_size = nullptr);

}
