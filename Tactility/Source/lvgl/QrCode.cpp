// SPDX-License-Identifier: Apache-2.0
#include <Tactility/lvgl/QrCode.h>

// The version-to-capacity table lives with the crash reporter's QR helper rather than here: it was
// there first, and it is exported to loadable apps through the ELF symbol list, so it is an API
// surface that this helper reuses instead of duplicating.
#include <Tactility/app/crashdiagnostics/QrHelpers.h>

#include <lvgl.h>
#include <qrcode.h>

#include <tactility/log.h>

#include <memory>

namespace tt::lvgl {

namespace {

constexpr auto* TAG = "QrCode";

}

lv_obj_t* qr_code_create(lv_obj_t* parent, const std::string& text, int32_t max_size, bool dark_on_light, int32_t* out_size) {
    if (out_size != nullptr) {
        *out_size = 0;
    }

    int version = 0;
    if (!getQrVersionForBinaryDataLength(text.size(), version)) {
        LOG_E(TAG, "Payload of %u bytes is too large for a QR code", static_cast<unsigned>(text.size()));
        return nullptr;
    }

    // Sized by the version rather than by the payload, because that is what the encoder needs.
    auto qrcodeData = std::make_unique<uint8_t[]>(qrcode_getBufferSize(version));
    if (qrcodeData == nullptr) {
        LOG_E(TAG, "Failed to allocate the QR buffer");
        return nullptr;
    }

    QRCode qrcode {};
    if (qrcode_initText(&qrcode, qrcodeData.get(), version, ECC_LOW, text.c_str()) != 0) {
        LOG_E(TAG, "Failed to encode the QR payload");
        return nullptr;
    }

    // The quiet zone: four modules of background colour on every side, which the QR standard
    // requires and which a scanner uses to find the code's edges. Without it the code's dark modules
    // met the dark UI directly and nothing could read it - the canvas has to be bigger than the code
    // itself, not exactly the code.
    constexpr int32_t QUIET_ZONE_MODULES = 4;
    const int32_t total_modules = static_cast<int32_t>(qrcode.size) + (2 * QUIET_ZONE_MODULES);

    // Whole modules only: a fractional module size would produce a code that scanners misread.
    const int32_t pixel_size = max_size / total_modules;
    if (pixel_size < 1) {
        LOG_E(TAG, "QR code of %u modules does not fit in %d pixels", static_cast<unsigned>(total_modules), static_cast<int>(max_size));
        return nullptr;
    }

    const int32_t offset = QUIET_ZONE_MODULES * pixel_size;
    const int32_t canvas_size = pixel_size * total_modules;
    auto* canvas = lv_canvas_create(parent);
    lv_obj_set_size(canvas, canvas_size, canvas_size);
    lv_obj_set_content_width(canvas, canvas_size);
    lv_obj_set_content_height(canvas, canvas_size);

    auto* draw_buf = lv_draw_buf_create(canvas_size, canvas_size, LV_COLOR_FORMAT_RGB565, LV_STRIDE_AUTO);
    if (draw_buf == nullptr) {
        LOG_E(TAG, "Failed to allocate the QR draw buffer");
        lv_obj_delete(canvas);
        return nullptr;
    }
    lv_canvas_set_draw_buf(canvas, draw_buf);
    // Filling with the background colour is what creates the quiet zone; the modules are drawn on
    // top of it, inset by the offset.
    lv_canvas_fill_bg(canvas, dark_on_light ? lv_color_white() : lv_color_black(), LV_OPA_COVER);

    for (uint8_t y = 0; y < qrcode.size; y++) {
        for (uint8_t x = 0; x < qrcode.size; x++) {
            const bool module_set = qrcode_getModule(&qrcode, x, y);
            const lv_color_t color = dark_on_light
                ? (module_set ? lv_color_black() : lv_color_white())
                : (module_set ? lv_color_white() : lv_color_black());
            const int32_t pos_x = offset + (x * pixel_size);
            const int32_t pos_y = offset + (y * pixel_size);
            for (int32_t px = 0; px < pixel_size; px++) {
                for (int32_t py = 0; py < pixel_size; py++) {
                    lv_canvas_set_px(canvas, pos_x + px, pos_y + py, color, LV_OPA_COVER);
                }
            }
        }
    }

    if (out_size != nullptr) {
        *out_size = canvas_size;
    }
    return canvas;
}

}
