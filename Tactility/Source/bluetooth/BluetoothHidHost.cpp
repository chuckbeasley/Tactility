#ifdef ESP_PLATFORM
#include <sdkconfig.h>
#endif

#if defined(CONFIG_BT_NIMBLE_ENABLED)

#include <Tactility/bluetooth/Bluetooth.h>
#include <Tactility/bluetooth/BluetoothPairedDevice.h>
#include <Tactility/bluetooth/BluetoothPrivate.h>

#include <Tactility/Assets.h>
#include <Tactility/Tactility.h>

#include <tactility/device.h>
#include <tactility/driver.h>
#include <tactility/drivers/keyboard.h>
#include <tactility/log.h>

#include <host/ble_gap.h>
#include <host/ble_gatt.h>
#include <host/ble_hs.h>
#include <host/ble_uuid.h>
#include <esp_timer.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

#include <lvgl/devices/keyboard.h>
#include <lvgl/lvgl.h>

#include <algorithm>
#include <array>
#include <atomic>
#include <cstring>
#include <memory>
#include <vector>

namespace tt::bluetooth {

constexpr auto* TAG = "BtHidHost";

// Defined further down with the other public-facing helpers; used by the GATT discovery
// callbacks above it.
static void fireHidHostFailure(const std::array<uint8_t, 6>& addr);

// Initiates the actual ble_gap_connect() for the current ctx; schedules a retry on EALREADY.
static void hidHostConnectInitiate();

// ---- Report type ----

enum class HidReportType : uint8_t { Unknown = 0, Keyboard, Mouse, Consumer };

struct HidHostInputRpt {
    uint16_t valHandle;
    uint16_t cccdHandle;
    uint16_t rptRefHandle;
    uint8_t  reportId;
    HidReportType type;
};

struct HidHostCtx {
    uint16_t connHandle        = BLE_HS_CONN_HANDLE_NONE;
    uint16_t hidSvcStart       = 0;
    uint16_t hidSvcEnd         = 0;
    std::vector<HidHostInputRpt> inputRpts;
    std::vector<uint16_t> allChrDefHandles;
    int subscribeIdx           = 0;
    int dscDiscIdx             = 0;
    int rptRefReadIdx          = 0;
    uint16_t rptMapHandle      = 0;
    std::vector<uint8_t> rptMap;
    bool securityInitiated     = false;
    bool typeResolutionDone    = false;
    bool readyBlockFired       = false;
    bool capsLock              = false;
    bool numLock               = true;
    lv_indev_t* mouseIndev     = nullptr;
    lv_obj_t*   mouseCursor    = nullptr;
    std::array<uint8_t, 6> peerAddr = {};
};

// ---- Globals ----

static std::unique_ptr<HidHostCtx> hid_host_ctx;
static QueueHandle_t hid_host_key_queue = nullptr;
static uint8_t hid_host_prev_keys[6] = {};
static esp_timer_handle_t hid_enc_retry_timer = nullptr;

// Retry state for ble_gap_connect() returning BLE_HS_EALREADY. The controller can only run one
// connection at a time, so a tap or auto-connect that lands while name-resolution (or another
// connect) is in progress gets EALREADY. Rather than failing silently (the "nothing happens" bug),
// retry briefly until the controller is free.
static esp_timer_handle_t hid_host_connect_retry_timer = nullptr;
static bool hid_host_connect_retry_scheduled = false;

static std::atomic<int32_t> hid_host_mouse_x{0};
static std::atomic<int32_t> hid_host_mouse_y{0};
static std::atomic<bool>    hid_host_mouse_btn{false};
static std::atomic<bool>    hid_host_mouse_active{false};

#define HID_HOST_KEY_QUEUE_SIZE 64
struct HidHostKeyEvt {
    uint32_t key;
    bool pressed;
    bool ctrl;
    bool alt;
    uint8_t hid_keycode;
    uint8_t hid_modifier;
};

// Standard USB HID keyboard usage codes (page 0x07) used by the key map below.
constexpr uint8_t HID_KC_ENTER        = 0x28;
constexpr uint8_t HID_KC_ESC          = 0x29;
constexpr uint8_t HID_KC_BACKSPACE    = 0x2A;
constexpr uint8_t HID_KC_TAB          = 0x2B;
constexpr uint8_t HID_KC_SPACE        = 0x2C;
constexpr uint8_t HID_KC_CAPS_LOCK    = 0x39;
constexpr uint8_t HID_KC_PAGE_UP      = 0x4B;
constexpr uint8_t HID_KC_DELETE       = 0x4C;
constexpr uint8_t HID_KC_PAGE_DOWN    = 0x4E;
constexpr uint8_t HID_KC_FN_RIGHT     = 0x4F; // Right Arrow
constexpr uint8_t HID_KC_FN_LEFT      = 0x50; // Left Arrow
constexpr uint8_t HID_KC_FN_DOWN      = 0x51; // Down Arrow
constexpr uint8_t HID_KC_FN_UP        = 0x52; // Up Arrow
constexpr uint8_t HID_KC_NUM_LOCK     = 0x53;
constexpr uint8_t HID_KC_KP_DIV       = 0x54;
constexpr uint8_t HID_KC_KP_MUL       = 0x55;
constexpr uint8_t HID_KC_KP_SUB       = 0x56;
constexpr uint8_t HID_KC_KP_ADD       = 0x57;
constexpr uint8_t HID_KC_KP_ENTER     = 0x58;
constexpr uint8_t HID_KC_KP_1         = 0x59; // ... 0x61 = 9
constexpr uint8_t HID_KC_KP_0         = 0x62;
constexpr uint8_t HID_KC_KP_DOT       = 0x63;

// Modifier bitmask bits (report byte 0).
constexpr uint8_t HID_MOD_LEFT_SHIFT  = 0x02;
constexpr uint8_t HID_MOD_RIGHT_SHIFT = 0x20;
constexpr uint8_t HID_MOD_LEFT_CTRL   = 0x01;
constexpr uint8_t HID_MOD_RIGHT_CTRL  = 0x10;
constexpr uint8_t HID_MOD_LEFT_ALT    = 0x04;
constexpr uint8_t HID_MOD_RIGHT_ALT   = 0x40;

// keycode2ascii[row][0]=unshifted, [1]=shifted. Index = HID usage 0x04..0x38. Mirrors the
// complete table in Platforms/platform-esp32/source/drivers/usb/esp32_usbhost_hid.cpp so a BLE
// HID keyboard types exactly like a USB HID keyboard.
static const uint8_t hid_keycode2ascii[57][2] = {
    {0, 0}, {0, 0}, {0, 0}, {0, 0},
    {'a', 'A'}, {'b', 'B'}, {'c', 'C'}, {'d', 'D'}, {'e', 'E'},
    {'f', 'F'}, {'g', 'G'}, {'h', 'H'}, {'i', 'I'}, {'j', 'J'},
    {'k', 'K'}, {'l', 'L'}, {'m', 'M'}, {'n', 'N'}, {'o', 'O'},
    {'p', 'P'}, {'q', 'Q'}, {'r', 'R'}, {'s', 'S'}, {'t', 'T'},
    {'u', 'U'}, {'v', 'V'}, {'w', 'W'}, {'x', 'X'}, {'y', 'Y'},
    {'z', 'Z'},
    {'1', '!'}, {'2', '@'}, {'3', '#'}, {'4', '$'}, {'5', '%'},
    {'6', '^'}, {'7', '&'}, {'8', '*'}, {'9', '('}, {'0', ')'},
    {'\r', '\r'}, {0, 0}, {'\b', 0}, {'\t', '\t'}, {' ', ' '},
    {'-', '_'}, {'=', '+'}, {'[', '{'}, {']', '}'},
    {'\\', '|'}, {'\\', '|'}, {';', ':'}, {'\'', '"'},
    {'`', '~'}, {',', '<'}, {'.', '>'}, {'/', '?'},
};

// Transient KEYBOARD_TYPE device that is constructed while a BLE HID keyboard is connected, so
// lvgl_hardware_keyboard_is_available() returns true and the on-screen keyboard is suppressed.
// Its KeyboardApi::read_key drains hid_host_key_queue, the same queue the HID reports feed.
static Device g_bt_kb_device = {};
static bool   g_bt_kb_active = false;
static bool   g_bt_kb_driver_state_ready = false;

// ---- Forward declarations ----

static void hidHostSubscribeNext(HidHostCtx& ctx);
static void hidHostStartRptRefRead(HidHostCtx& ctx);
static void hidHostReadReportMap(HidHostCtx& ctx);
static uint16_t getDescEndHandle(const HidHostCtx& ctx, uint16_t valHandle);

// ---- Keycode mapping ----

// Maps a HID keyboard usage + modifier to a Unicode codepoint (never an LVGL LV_KEY_* sentinel),
// matching the KeyboardKeyData::key contract and the USB HID driver's behaviour. Returns 0 for
// keys that produce no character/action for LVGL (F1-F12, lock toggles, etc.).
static uint32_t hidHostMapKeycode(uint8_t mod, uint8_t kc, bool capsLock, bool numLock) {
    bool shift = ((mod & HID_MOD_LEFT_SHIFT) || (mod & HID_MOD_RIGHT_SHIFT)) != 0;

    switch (kc) {
        case HID_KC_ENTER:     return CODEPOINT_ENTER;
        case HID_KC_ESC:       return CODEPOINT_ESCAPE;
        case HID_KC_DELETE:    return CODEPOINT_DELETE;
        case HID_KC_BACKSPACE: return CODEPOINT_BACKSPACE;
        case HID_KC_TAB:       return '\t';
        case HID_KC_FN_UP:     return CODEPOINT_ARROW_UP;
        case HID_KC_FN_DOWN:   return CODEPOINT_ARROW_DOWN;
        case HID_KC_FN_LEFT:   return CODEPOINT_ARROW_LEFT;
        case HID_KC_FN_RIGHT:  return CODEPOINT_ARROW_RIGHT;
        case HID_KC_KP_ENTER:  return CODEPOINT_ENTER;
        case HID_KC_KP_ADD:    return '+';
        case HID_KC_KP_SUB:    return '-';
        case HID_KC_KP_MUL:    return '*';
        case HID_KC_KP_DIV:    return '/';
        default: break;
    }

    // Numpad. When Num Lock is off the navigation keys are instead mapped to caret/Home/End
    // (matching the USB HID driver's num_lock handling).
    if (kc >= HID_KC_KP_1 && kc <= HID_KC_KP_0) {
        if (!numLock) return 0;
        if (kc == HID_KC_KP_0) return '0';
        return static_cast<uint32_t>('1' + (kc - HID_KC_KP_1));
    }
    if (kc == HID_KC_KP_DOT) return numLock ? (uint32_t)'.' : (uint32_t)CODEPOINT_DELETE;

    // Letters, numbers, punctuation and the C0/whitespace keys via the table. The table is
    // indexed directly by usage code (0x04..0x38), and row 0x28..0x2C (Enter/Unused/Backspace/
    // Tab/Space) carries the ASCII equivalents that LVGL's codepoint_to_lv_key() leaves as-is.
    if (kc >= 0x04 && kc < (sizeof(hid_keycode2ascii) / sizeof(hid_keycode2ascii[0]))) {
        bool is_letter = (kc >= 0x04 && kc <= 0x1D);
        bool effective_shift = is_letter ? (shift ^ capsLock) : shift;
        uint8_t ch = hid_keycode2ascii[kc][effective_shift ? 1 : 0];
        if (ch != 0) return static_cast<uint32_t>(ch);
    }
    return 0;
}

static void hidHostHandleKeyboardReport(HidHostCtx& ctx, const uint8_t* data, uint16_t len) {
    if (len < 3 || !hid_host_key_queue) return;
    uint8_t mod = data[0];
    const uint8_t* curr = &data[2];
    int nkeys = std::min((int)(len - 2), 6);

    // Toggle lock state on a NEW press of the lock key, before mapping the other keys in the same
    // report (USB driver behaviour). Lock keys produce no character of their own and are skipped by
    // the press/release loops below.
    for (int i = 0; i < nkeys; i++) {
        uint8_t kc = curr[i];
        if (kc != HID_KC_CAPS_LOCK && kc != HID_KC_NUM_LOCK) continue;
        bool had = false;
        for (int j = 0; j < 6; j++) { if (hid_host_prev_keys[j] == kc) { had = true; break; } }
        if (!had) {
            if (kc == HID_KC_CAPS_LOCK) ctx.capsLock = !ctx.capsLock;
            else                        ctx.numLock  = !ctx.numLock;
        }
    }

    // Releases.
    for (int i = 0; i < 6; i++) {
        uint8_t kc = hid_host_prev_keys[i];
        if (kc == 0) continue;
        if (kc == HID_KC_CAPS_LOCK || kc == HID_KC_NUM_LOCK) continue;
        bool still = false;
        for (int j = 0; j < nkeys; j++) { if (curr[j] == kc) { still = true; break; } }
        if (!still) {
            uint32_t key = hidHostMapKeycode(0, kc, ctx.capsLock, ctx.numLock);
            if (key) { HidHostKeyEvt e{key, false, false, false, kc, 0}; xQueueSend(hid_host_key_queue, &e, 0); }
        }
    }
    // Presses.
    for (int i = 0; i < nkeys; i++) {
        uint8_t kc = curr[i];
        if (kc == 0) continue;
        if (kc == HID_KC_CAPS_LOCK || kc == HID_KC_NUM_LOCK) continue;
        bool had = false;
        for (int j = 0; j < 6; j++) { if (hid_host_prev_keys[j] == kc) { had = true; break; } }
        if (!had) {
            uint32_t key = hidHostMapKeycode(mod, kc, ctx.capsLock, ctx.numLock);
            if (key) {
                bool ctrl = (mod & (HID_MOD_LEFT_CTRL | HID_MOD_RIGHT_CTRL)) != 0;
                bool alt  = (mod & (HID_MOD_LEFT_ALT  | HID_MOD_RIGHT_ALT))  != 0;
                HidHostKeyEvt e{key, true, ctrl, alt, kc, mod};
                xQueueSend(hid_host_key_queue, &e, 0);
            }
        }
    }
    std::memcpy(hid_host_prev_keys, curr, nkeys);
    if (nkeys < 6) std::memset(hid_host_prev_keys + nkeys, 0, 6 - nkeys);
}

static void hidHostMouseReadCb(lv_indev_t* /*indev*/, lv_indev_data_t* data) {
    int32_t cx = hid_host_mouse_x.load();
    int32_t cy = hid_host_mouse_y.load();

    lv_display_t* disp = lv_display_get_default();
    if (disp) {
        int32_t ow = lv_display_get_original_horizontal_resolution(disp);
        int32_t oh = lv_display_get_original_vertical_resolution(disp);
        switch (lv_display_get_rotation(disp)) {
            case LV_DISPLAY_ROTATION_0:
                data->point.x = (lv_coord_t)cx;
                data->point.y = (lv_coord_t)cy;
                break;
            case LV_DISPLAY_ROTATION_90:
                data->point.x = (lv_coord_t)cy;
                data->point.y = (lv_coord_t)(oh - cx - 1);
                break;
            case LV_DISPLAY_ROTATION_180:
                data->point.x = (lv_coord_t)(ow - cx - 1);
                data->point.y = (lv_coord_t)(oh - cy - 1);
                break;
            case LV_DISPLAY_ROTATION_270:
                data->point.x = (lv_coord_t)(ow - cy - 1);
                data->point.y = (lv_coord_t)cx;
                break;
        }
    } else {
        data->point.x = (lv_coord_t)cx;
        data->point.y = (lv_coord_t)cy;
    }
    data->state = hid_host_mouse_btn.load() ? LV_INDEV_STATE_PRESSED : LV_INDEV_STATE_RELEASED;

    if (!hid_host_mouse_active.load()) {
        hid_host_mouse_active.store(true);
        if (hid_host_ctx && hid_host_ctx->mouseCursor) {
            lv_obj_remove_flag(hid_host_ctx->mouseCursor, LV_OBJ_FLAG_HIDDEN);
        }
    }
}

static void hidHostHandleMouseReport(const uint8_t* data, uint16_t len) {
    if (len < 3) return;
    bool btn  = (data[0] & 0x01) != 0;
    int8_t dx = (int8_t)data[1];
    int8_t dy = (int8_t)data[2];

    lv_display_t* disp = lv_display_get_default();
    int32_t w = disp ? lv_display_get_horizontal_resolution(disp) : 320;
    int32_t h = disp ? lv_display_get_vertical_resolution(disp)   : 240;

    int32_t nx = hid_host_mouse_x.load() + dx;
    int32_t ny = hid_host_mouse_y.load() + dy;
    if (nx < 0) nx = 0;
    if (nx >= w) nx = w - 1;
    if (ny < 0) ny = 0;
    if (ny >= h) ny = h - 1;

    hid_host_mouse_x.store(nx);
    hid_host_mouse_y.store(ny);
    hid_host_mouse_btn.store(btn);

    if (hid_host_ctx && hid_host_ctx->mouseIndev == nullptr) {
        getMainDispatcher().dispatch([] {
            if (!hid_host_ctx || hid_host_ctx->mouseIndev != nullptr) return;
            if (!lvgl_try_lock(1000)) { LOG_W(TAG, "LVGL lock failed for mouse indev"); return; }
            auto* ms = lv_indev_create();
            lv_indev_set_type(ms, LV_INDEV_TYPE_POINTER);
            lv_indev_set_read_cb(ms, hidHostMouseReadCb);
            auto* cur = lv_image_create(lv_layer_sys());
            lv_obj_remove_flag(cur, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_add_flag(cur, LV_OBJ_FLAG_HIDDEN);
            lv_image_set_src(cur, TT_ASSETS_UI_CURSOR);
            lv_indev_set_cursor(ms, cur);
            hid_host_ctx->mouseIndev  = ms;
            hid_host_ctx->mouseCursor = cur;
            lvgl_unlock();
            LOG_I(TAG, "Mouse indev registered");
        });
    }
}

// ---- Timer callback for post-encryption CCCD retry ----

static void hidEncRetryTimerCb(void* /*arg*/) {
    if (hid_host_ctx) {
        if (!hid_host_ctx->typeResolutionDone) {
            LOG_W(TAG, "Post-encryption delay — type resolution timed out, proceeding");
            hid_host_ctx->typeResolutionDone = true;
            hid_host_ctx->subscribeIdx = 0;
        } else {
            LOG_I(TAG, "Post-encryption delay complete — starting CCCD subscriptions");
        }
        hidHostSubscribeNext(*hid_host_ctx);
    }
}

// ---- Report Map parsing ----

static void applyReportMapTypes(HidHostCtx& ctx) {
    const uint8_t* data = ctx.rptMap.data();
    size_t len = ctx.rptMap.size();

    uint16_t usagePage = 0, usage = 0;
    uint8_t reportId = 0;
    int depth = 0;
    HidReportType collType = HidReportType::Unknown;

    struct Entry { uint8_t id; HidReportType type; };
    std::vector<Entry> typeMap;
    std::vector<HidReportType> collOrder;
    bool collHadInput = false;

    size_t i = 0;
    while (i < len) {
        uint8_t prefix = data[i++];
        if (prefix == 0xFE) {
            if (i + 1 >= len) break;
            uint8_t lsz = data[i++]; i++; i += lsz; continue;
        }
        uint8_t bSize = prefix & 0x03;
        uint8_t bType = (prefix >> 2) & 0x03;
        uint8_t bTag  = (prefix >> 4) & 0x0F;
        uint8_t dataLen = (bSize == 3) ? 4 : bSize;
        if (i + dataLen > len) break;
        uint32_t value = 0;
        for (uint8_t j = 0; j < dataLen; j++) value |= (uint32_t)data[i++] << (8 * j);

        if (bType == 0) {
            if (bTag == 0xA) {
                if (depth == 0 && value == 0x01) {
                    if      (usagePage == 0x01 && usage == 0x06) collType = HidReportType::Keyboard;
                    else if (usagePage == 0x01 && usage == 0x02) collType = HidReportType::Mouse;
                    else if (usagePage == 0x0C)                  collType = HidReportType::Consumer;
                    else                                          collType = HidReportType::Unknown;
                    collHadInput = false;
                }
                depth++; usage = 0;
            } else if (bTag == 0xC) {
                if (depth > 0) depth--;
                if (depth == 0) { collType = HidReportType::Unknown; collHadInput = false; }
                usage = 0;
            } else if (bTag == 0x8) {
                if (depth > 0 && collType != HidReportType::Unknown) {
                    if (!collHadInput) { collOrder.push_back(collType); collHadInput = true; }
                    if (reportId != 0) {
                        bool found = false;
                        for (const auto& e : typeMap) { if (e.id == reportId) { found = true; break; } }
                        if (!found) typeMap.push_back({reportId, collType});
                    }
                }
                usage = 0;
            } else { usage = 0; }
        } else if (bType == 1) {
            if      (bTag == 0x0) usagePage = (uint16_t)value;
            else if (bTag == 0x8) reportId  = (uint8_t)value;
        } else if (bType == 2) {
            if (bTag == 0x0) usage = (uint16_t)value;
        }
    }

    bool anyNonZeroId = false;
    for (const auto& rpt : ctx.inputRpts) { if (rpt.reportId != 0) { anyNonZeroId = true; break; } }

    size_t zeroRptIdx = 0;
    for (auto& rpt : ctx.inputRpts) {
        if (anyNonZeroId) {
            for (const auto& e : typeMap) { if (e.id == rpt.reportId) { rpt.type = e.type; break; } }
        } else {
            if (zeroRptIdx < collOrder.size()) rpt.type = collOrder[zeroRptIdx];
            zeroRptIdx++;
        }
        LOG_I(TAG, "Report val_handle=%d reportId=%d type=%d", rpt.valHandle, rpt.reportId, (int)rpt.type);
    }
    ctx.rptMap.clear();
}

// ---- Report Reference read chain ----

static void hidHostStartRptRefRead(HidHostCtx& ctx) {
    while (ctx.rptRefReadIdx < (int)ctx.inputRpts.size() &&
           ctx.inputRpts[ctx.rptRefReadIdx].rptRefHandle == 0) {
        ctx.rptRefReadIdx++;
    }
    if (ctx.rptRefReadIdx >= (int)ctx.inputRpts.size()) {
        hidHostReadReportMap(ctx);
        return;
    }
    uint16_t handle = ctx.inputRpts[ctx.rptRefReadIdx].rptRefHandle;
    int rc = ble_gattc_read(ctx.connHandle, handle, [](uint16_t conn_handle,
                             const struct ble_gatt_error* error,
                             struct ble_gatt_attr* attr, void* /*arg*/) -> int {
        if (!hid_host_ctx) return 0;
        auto& ctx = *hid_host_ctx;
        if (conn_handle != ctx.connHandle) return 0;
        if (error->status == BLE_HS_EDONE) return 0;
        if (error->status == 0 && attr != nullptr) {
            if (OS_MBUF_PKTLEN(attr->om) >= 2 && ctx.rptRefReadIdx < (int)ctx.inputRpts.size()) {
                uint8_t rpt_ref[2] = {};
                os_mbuf_copydata(attr->om, 0, 2, rpt_ref);
                ctx.inputRpts[ctx.rptRefReadIdx].reportId = rpt_ref[0];
                LOG_I(TAG, "Report[%d] val_handle=%d reportId=%d", ctx.rptRefReadIdx,
                            ctx.inputRpts[ctx.rptRefReadIdx].valHandle, rpt_ref[0]);
            }
        }
        ctx.rptRefReadIdx++;
        hidHostStartRptRefRead(ctx);
        return 0;
    }, nullptr);
    if (rc != 0) {
        LOG_W(TAG, "rptRef read[%d] failed rc=%d — skipping", ctx.rptRefReadIdx, rc);
        ctx.rptRefReadIdx++;
        hidHostStartRptRefRead(ctx);
    }
}

// ---- Report Map read ----

static void hidHostReadReportMap(HidHostCtx& ctx) {
    if (ctx.rptMapHandle == 0) {
        LOG_I(TAG, "No Report Map char — skipping type resolution");
        ctx.typeResolutionDone = true;
        ctx.subscribeIdx = 0;
        hidHostSubscribeNext(ctx);
        return;
    }
    int rc = ble_gattc_read_long(ctx.connHandle, ctx.rptMapHandle, 0,
        [](uint16_t conn_handle, const struct ble_gatt_error* error,
           struct ble_gatt_attr* attr, void* /*arg*/) -> int {
            if (!hid_host_ctx) return 0;
            auto& ctx = *hid_host_ctx;
            if (conn_handle != ctx.connHandle) return 0;
            if (error->status == 0 && attr != nullptr) {
                uint16_t chunk = OS_MBUF_PKTLEN(attr->om);
                size_t old_sz = ctx.rptMap.size();
                ctx.rptMap.resize(old_sz + chunk);
                os_mbuf_copydata(attr->om, 0, chunk, ctx.rptMap.data() + old_sz);
                return 0;
            }
            if (!ctx.rptMap.empty()) {
                LOG_I(TAG, "Report map read (%d bytes)", (int)ctx.rptMap.size());
                applyReportMapTypes(ctx);
            } else {
                LOG_W(TAG, "Report map read failed — types remain Unknown");
            }
            ctx.typeResolutionDone = true;
            ctx.subscribeIdx = 0;
            hidHostSubscribeNext(ctx);
            return 0;
        }, nullptr);
    if (rc != 0) {
        LOG_W(TAG, "Report map read_long failed rc=%d — skipping", rc);
        ctx.typeResolutionDone = true;
        ctx.subscribeIdx = 0;
        hidHostSubscribeNext(ctx);
    }
}

// ---- CCCD subscription chain ----

static int hidHostCccdWriteCb(uint16_t conn_handle, const struct ble_gatt_error* error,
                               struct ble_gatt_attr* /*attr*/, void* /*arg*/) {
    if (!hid_host_ctx) return 0;
    auto& ctx = *hid_host_ctx;
    if (conn_handle != ctx.connHandle) return 0;

    if (error->status != 0 && error->status != BLE_HS_EDONE) {
        if ((error->status == BLE_HS_ATT_ERR(BLE_ATT_ERR_INSUFFICIENT_AUTHEN) ||
             error->status == BLE_HS_ATT_ERR(BLE_ATT_ERR_INSUFFICIENT_ENC))
            && !ctx.securityInitiated) {
            LOG_I(TAG, "CCCD auth required — initiating security");
            ctx.securityInitiated = true;
            ble_gap_security_initiate(conn_handle);
            return 0;
        }
        if (error->status == BLE_HS_ETIMEOUT) {
            LOG_W(TAG, "CCCD write timed out for report[%d] — skipping", ctx.subscribeIdx);
            ctx.subscribeIdx++;
            hidHostSubscribeNext(ctx);
            return 0;
        }
        if (error->status == BLE_HS_ENOTCONN) {
            LOG_W(TAG, "CCCD write failed — not connected");
            return 0;
        }
        LOG_W(TAG, "CCCD write failed status=%d", error->status);
    }
    ctx.subscribeIdx++;
    hidHostSubscribeNext(ctx);
    return 0;
}

// ---- Transient KEYBOARD_TYPE device ----
// While a BLE HID keyboard is connected a KEYBOARD_TYPE device is constructed so the rest of the
// system (lvgl_hardware_keyboard_is_available(), Tactility's KeyboardDeviceListener) sees a real
// hardware keyboard through the same generic device model as any other keyboard, and the on-screen
// keyboard is suppressed. Real key events flow through THIS device's read_key, which drains the
// same hid_host_key_queue the HID report handler fills.

static error_t btKbDeviceStart(Device* device) {
    device_set_driver_data(device, hid_host_key_queue);
    return ERROR_NONE;
}

static error_t btKbDeviceStop(Device* device) {
    device_set_driver_data(device, nullptr);
    return ERROR_NONE;
}

static error_t btKbDeviceReadKey(Device* device, KeyboardKeyData* data) {
    auto* queue = static_cast<QueueHandle_t>(device_get_driver_data(device));
    if (queue == nullptr || data == nullptr) {
        if (data) *data = {};
        return ERROR_NONE;
    }
    HidHostKeyEvt evt = {};
    if (xQueueReceive(queue, &evt, 0) != pdTRUE) {
        *data = {};
        return ERROR_NONE;
    }
    data->key           = evt.key;
    data->pressed       = evt.pressed;
    data->ctrl          = evt.ctrl;
    data->alt           = evt.alt;
    data->hid_keycode   = evt.hid_keycode;
    data->hid_modifier  = evt.hid_modifier;
    data->continue_reading = (uxQueueMessagesWaiting(queue) > 0);
    return ERROR_NONE;
}

static bool btKbDeviceIsPresent(Device* /*device*/) {
    return true; // the device only exists while a keyboard is connected
}

static const KeyboardApi bt_kb_api = {
    .read_key = btKbDeviceReadKey,
    .get_backlight = nullptr,
    .is_present = btKbDeviceIsPresent,
};

Driver bt_keyboard_driver = {
    .name         = "bluetooth_keyboard",
    .compatible   = nullptr,
    .start_device = btKbDeviceStart,
    .stop_device  = btKbDeviceStop,
    .api          = &bt_kb_api,
    .device_type  = &KEYBOARD_TYPE,
    .owner        = nullptr,
    .internal     = nullptr,
};

static void btHostKbDeviceConstruct() {
    if (g_bt_kb_active) {
        return;
    }

    // driver_bind() dereferences driver->internal (driver.cpp:127), which is only allocated by
    // driver_construct(). The static initializer sets .internal = nullptr (matches the USB HID
    // keyboard driver), so construct the driver's internal state once before any device_start();
    // otherwise device_start → driver_bind crashes with a Load access fault.
    if (!g_bt_kb_driver_state_ready) {
        if (driver_construct(&bt_keyboard_driver) != ERROR_NONE) {
            LOG_E(TAG, "failed to construct Bluetooth keyboard driver");
            return;
        }
        g_bt_kb_driver_state_ready = true;
    }

    Device* parent = nullptr;
    if (device_get_first_active_by_type(&BLUETOOTH_TYPE, &parent) != ERROR_NONE) {
        LOG_W(TAG, "No active BLE device — cannot attach keyboard device");
        return;
    }

    g_bt_kb_device = Device {
        .address  = 0,
        .name     = "bluetooth_keyboard0",
        .config   = nullptr,
        .parent   = nullptr,
        .internal = nullptr,
    };
    if (device_construct(&g_bt_kb_device) != ERROR_NONE) {
        LOG_E(TAG, "failed to construct Bluetooth keyboard device");
        device_put(parent);
        return;
    }
    device_set_parent(&g_bt_kb_device, parent);
    device_set_driver(&g_bt_kb_device, &bt_keyboard_driver);
    if (device_add(&g_bt_kb_device) != ERROR_NONE) {
        LOG_E(TAG, "failed to add Bluetooth keyboard device");
        device_destruct(&g_bt_kb_device);
        device_put(parent);
        return;
    }
    if (device_start(&g_bt_kb_device) != ERROR_NONE) {
        LOG_E(TAG, "failed to start Bluetooth keyboard device");
        device_remove(&g_bt_kb_device);
        device_destruct(&g_bt_kb_device);
        device_put(parent);
        return;
    }
    device_put(parent);
    g_bt_kb_active = true;
    LOG_I(TAG, "Keyboard device registered");
}

static void btHostKbDeviceDestruct() {
    if (!g_bt_kb_active) {
        return;
    }
    g_bt_kb_active = false;
    device_stop(&g_bt_kb_device);
    device_remove(&g_bt_kb_device);
    device_destruct(&g_bt_kb_device);
    LOG_I(TAG, "Keyboard device deregistered");
}

static void hidHostSubscribeNext(HidHostCtx& ctx) {
    if (ctx.subscribeIdx >= (int)ctx.inputRpts.size()) {
        if (ctx.readyBlockFired) {
            LOG_I(TAG, "Subscribe ready block already ran — ignoring duplicate");
            return;
        }
        ctx.readyBlockFired = true;
        LOG_I(TAG, "All %d reports subscribed — ready", (int)ctx.inputRpts.size());
        if (hid_enc_retry_timer) esp_timer_stop(hid_enc_retry_timer);

        if (!hid_host_key_queue) {
            hid_host_key_queue = xQueueCreate(HID_HOST_KEY_QUEUE_SIZE, sizeof(HidHostKeyEvt));
        }
        getMainDispatcher().dispatch([] {
            // Constructing the device fires DEVICE_EVENT_STARTED, which the KeyboardDeviceListener
            // handles by binding it to an lv_indev_t via lvgl_keyboard_add() (under the LVGL lock).
            // Registering a ready+present KEYBOARD_TYPE device is also what makes
            // lvgl_hardware_keyboard_is_available() return true, suppressing the on-screen keyboard.
            if (!hid_host_ctx) return;
            btHostKbDeviceConstruct();
        });

        auto peer_addr = ctx.peerAddr;
        getMainDispatcher().dispatch([peer_addr] {
            // Find name from cached scan results
            std::string name;
            {
                auto results = getScanResults();
                for (const auto& r : results) {
                    if (r.addr == peer_addr) { name = r.name; break; }
                }
            }
            settings::PairedDevice device;
            device.addr        = peer_addr;
            device.profileId   = BT_PROFILE_HID_HOST;
            device.autoConnect = true;
            const auto addr_hex = settings::addrToHex(peer_addr);
            settings::PairedDevice existing;
            if (settings::load(addr_hex, existing)) {
                device.autoConnect = existing.autoConnect;
            }
            device.name = name;
            settings::save(device);
            Device* dev;
            if (device_get_first_active_by_type(&BLUETOOTH_TYPE, &dev) == ERROR_NONE) {
                BtEvent e = {};
                e.type = BT_EVENT_PROFILE_STATE_CHANGED;
                e.profile_state.state = BT_PROFILE_STATE_CONNECTED;
                e.profile_state.profile = BT_PROFILE_HID_HOST;
                bluetooth_fire_event(dev, e);
                device_put(dev);
            }
        });
        return;
    }
    auto& rpt = ctx.inputRpts[ctx.subscribeIdx];
    if (rpt.cccdHandle == 0) {
        ctx.subscribeIdx++;
        hidHostSubscribeNext(ctx);
        return;
    }
    static const uint16_t notify_val = 0x0001;
    int rc = ble_gattc_write_flat(ctx.connHandle, rpt.cccdHandle,
                                   &notify_val, sizeof(notify_val),
                                   hidHostCccdWriteCb, nullptr);
    if (rc != 0) {
        LOG_W(TAG, "gattc_write_flat CCCD failed rc=%d", rc);
        ctx.subscribeIdx++;
        hidHostSubscribeNext(ctx);
    }
}

// ---- Descriptor discovery ----

static int hidHostDscDiscCb(uint16_t conn_handle, const struct ble_gatt_error* error,
                             uint16_t chr_val_handle, const struct ble_gatt_dsc* dsc, void* /*arg*/) {
    if (!hid_host_ctx) return 0;
    auto& ctx = *hid_host_ctx;
    if (conn_handle != ctx.connHandle) return 0;

    if (error->status == 0 && dsc != nullptr) {
        uint16_t dsc_uuid = ble_uuid_u16(&dsc->uuid.u);
        for (auto& rpt : ctx.inputRpts) {
            if (rpt.valHandle != chr_val_handle) continue;
            if (dsc_uuid == 0x2902) { rpt.cccdHandle   = dsc->handle; }
            else if (dsc_uuid == 0x2908) { rpt.rptRefHandle = dsc->handle; }
            break;
        }
    } else if (error->status == BLE_HS_EDONE) {
        int next_idx = ctx.dscDiscIdx + 1;
        if (next_idx < (int)ctx.inputRpts.size()) {
            ctx.dscDiscIdx = next_idx;
            auto& next_rpt = ctx.inputRpts[next_idx];
            uint16_t end = getDescEndHandle(ctx, next_rpt.valHandle);
            int rc = ble_gattc_disc_all_dscs(ctx.connHandle, next_rpt.valHandle, end,
                                              hidHostDscDiscCb, nullptr);
            if (rc != 0) {
                LOG_W(TAG, "disc_all_dscs[%d] failed rc=%d", next_idx, rc);
                ctx.rptRefReadIdx = 0;
                hidHostStartRptRefRead(ctx);
            }
        } else {
            ctx.rptRefReadIdx = 0;
            hidHostStartRptRefRead(ctx);
        }
    }
    return 0;
}

static uint16_t getDescEndHandle(const HidHostCtx& ctx, uint16_t valHandle) {
    for (uint16_t dh : ctx.allChrDefHandles) {
        if (dh > valHandle) return dh - 1;
    }
    return ctx.hidSvcEnd;
}

// ---- Characteristic discovery ----

static int hidHostChrDiscCb(uint16_t conn_handle, const struct ble_gatt_error* error,
                             const struct ble_gatt_chr* chr, void* /*arg*/) {
    if (!hid_host_ctx) return 0;
    auto& ctx = *hid_host_ctx;
    if (conn_handle != ctx.connHandle) return 0;

    if (error->status == 0 && chr != nullptr) {
        ctx.allChrDefHandles.push_back(chr->def_handle);
        uint16_t uuid16 = ble_uuid_u16(&chr->uuid.u);
        if (uuid16 == 0x2A4D && (chr->properties & BLE_GATT_CHR_PROP_NOTIFY)) {
            HidHostInputRpt rpt = {};
            rpt.valHandle = chr->val_handle;
            ctx.inputRpts.push_back(rpt);
            LOG_I(TAG, "Input Report chr val_handle=%d", chr->val_handle);
        } else if (uuid16 == 0x2A4B) {
            ctx.rptMapHandle = chr->val_handle;
        }
    } else if (error->status == BLE_HS_EDONE) {
        std::sort(ctx.allChrDefHandles.begin(), ctx.allChrDefHandles.end());
        if (ctx.inputRpts.empty()) {
            LOG_W(TAG, "No Input Report chars — disconnecting");
            ble_gap_terminate(ctx.connHandle, BLE_ERR_REM_USER_CONN_TERM);
            return 0;
        }
        ctx.dscDiscIdx = 0;
        auto& first = ctx.inputRpts[0];
        uint16_t end = getDescEndHandle(ctx, first.valHandle);
        int rc = ble_gattc_disc_all_dscs(ctx.connHandle, first.valHandle, end,
                                          hidHostDscDiscCb, nullptr);
        if (rc != 0) {
            LOG_W(TAG, "disc_all_dscs[0] failed rc=%d", rc);
            ctx.rptRefReadIdx = 0;
            hidHostStartRptRefRead(ctx);
        }
    }
    return 0;
}

// ---- Service discovery ----

static int hidHostSvcDiscCb(uint16_t conn_handle, const struct ble_gatt_error* error,
                             const struct ble_gatt_svc* svc, void* /*arg*/) {
    if (!hid_host_ctx) return 0;
    auto& ctx = *hid_host_ctx;
    if (conn_handle != ctx.connHandle) return 0;

    if (error->status == 0 && svc != nullptr) {
        if (ble_uuid_u16(&svc->uuid.u) == 0x1812) {
            ctx.hidSvcStart = svc->start_handle;
            ctx.hidSvcEnd   = svc->end_handle;
            LOG_I(TAG, "HID service start=%d end=%d", ctx.hidSvcStart, ctx.hidSvcEnd);
        }
    } else if (error->status == BLE_HS_EDONE) {
        if (ctx.hidSvcStart == 0) {
            LOG_W(TAG, "No HID service found — disconnecting");
            fireHidHostFailure(ctx.peerAddr);
            ble_gap_terminate(ctx.connHandle, BLE_ERR_REM_USER_CONN_TERM);
            return 0;
        }
        int rc = ble_gattc_disc_all_chrs(ctx.connHandle, ctx.hidSvcStart, ctx.hidSvcEnd,
                                          hidHostChrDiscCb, nullptr);
        if (rc != 0) {
            LOG_W(TAG, "disc_all_chrs failed rc=%d", rc);
            ble_gap_terminate(ctx.connHandle, BLE_ERR_REM_USER_CONN_TERM);
        }
    }
    return 0;
}

// ---- GAP callback for HID host central connection ----

static int hidHostGapCb(struct ble_gap_event* event, void* /*arg*/) {
    if (!hid_host_ctx) return 0;
    auto& ctx = *hid_host_ctx;

    switch (event->type) {
        case BLE_GAP_EVENT_CONNECT:
            if (event->connect.status == 0) {
                ctx.connHandle = event->connect.conn_handle;
                LOG_I(TAG, "Connected (handle=%d)", ctx.connHandle);
                // A connect established — cancel any pending EALREADY retry.
                if (hid_host_connect_retry_timer != nullptr) {
                    esp_timer_stop(hid_host_connect_retry_timer);
                }
                hid_host_connect_retry_scheduled = false;
                int rc = ble_gattc_disc_all_svcs(ctx.connHandle, hidHostSvcDiscCb, nullptr);
                if (rc != 0) {
                    LOG_W(TAG, "disc_all_svcs failed rc=%d", rc);
                    ble_gap_terminate(ctx.connHandle, BLE_ERR_REM_USER_CONN_TERM);
                }
            } else {
                LOG_W(TAG, "Connect failed status=%d", event->connect.status);
                hid_host_ctx.reset();
                Device* dev;
                if (device_get_first_active_by_type(&BLUETOOTH_TYPE, &dev) == ERROR_NONE) {
                    bluetooth_set_hid_host_active(dev, false);
                    BtEvent e = {};
                    e.type = BT_EVENT_PROFILE_STATE_CHANGED;
                    e.profile_state.state = BT_PROFILE_STATE_IDLE;
                    e.profile_state.profile = BT_PROFILE_HID_HOST;
                    bluetooth_fire_event(dev, e);
                    device_put(dev);
                }
            }
            break;

        case BLE_GAP_EVENT_DISCONNECT: {
            LOG_I(TAG, "Disconnected reason=%d", event->disconnect.reason);
            lv_indev_t* saved_mouse  = hid_host_ctx ? hid_host_ctx->mouseIndev  : nullptr;
            lv_obj_t*   saved_cursor = hid_host_ctx ? hid_host_ctx->mouseCursor : nullptr;
            QueueHandle_t saved_queue = hid_host_key_queue;
            hid_host_ctx.reset();
            hid_host_key_queue = nullptr;
            std::memset(hid_host_prev_keys, 0, sizeof(hid_host_prev_keys));
            hid_host_mouse_x.store(0);
            hid_host_mouse_y.store(0);
            hid_host_mouse_btn.store(false);
            hid_host_mouse_active.store(false);
            if (hid_host_connect_retry_timer != nullptr) {
                esp_timer_stop(hid_host_connect_retry_timer);
            }
            hid_host_connect_retry_scheduled = false;

            Device* dev;
            if (device_get_first_active_by_type(&BLUETOOTH_TYPE, &dev) == ERROR_NONE) {
                bluetooth_set_hid_host_active(dev, false);
                struct BtEvent e = {};
                e.type = BT_EVENT_PROFILE_STATE_CHANGED;
                e.profile_state.state = BT_PROFILE_STATE_IDLE;
                e.profile_state.profile = BT_PROFILE_HID_HOST;
                bluetooth_fire_event(dev, e);
                device_put(dev);
            }

            getMainDispatcher().dispatch([saved_mouse, saved_cursor, saved_queue] {
                // Detach the keyboard indev / unregister the KEYBOARD_TYPE device BEFORE freeing
                // the queue that its read_key drains. device_stop() fires DEVICE_EVENT_STOPPING,
                // which the KeyboardDeviceListener handles by removing the LVGL indev (taking the
                // LVGL lock itself), so this must run before we take the lock below.
                btHostKbDeviceDestruct();

                if (!lvgl_try_lock(1000)) {
                    LOG_W(TAG, "Failed to acquire LVGL lock for indev cleanup");
                    if (saved_queue) vQueueDelete(saved_queue);
                    return;
                }
                if (saved_mouse)  lv_indev_delete(saved_mouse);
                if (saved_cursor) lv_obj_delete(saved_cursor);
                lvgl_unlock();
                if (saved_queue) vQueueDelete(saved_queue);
            });
            break;
        }

        case BLE_GAP_EVENT_ENC_CHANGE:
            if (event->enc_change.conn_handle == ctx.connHandle) {
                if (event->enc_change.status == 0) {
                    LOG_I(TAG, "Encryption established — retrying CCCD in 500ms");
                    ctx.subscribeIdx = 0;
                    if (hid_enc_retry_timer) {
                        esp_timer_stop(hid_enc_retry_timer);
                        esp_timer_start_once(hid_enc_retry_timer, 500 * 1000);
                    } else {
                        hidHostSubscribeNext(ctx);
                    }
                } else {
                    LOG_W(TAG, "Encryption failed status=%d", event->enc_change.status);
                }
            }
            break;

        case BLE_GAP_EVENT_NOTIFY_RX:
            if (event->notify_rx.conn_handle == ctx.connHandle) {
                uint16_t len = OS_MBUF_PKTLEN(event->notify_rx.om);
                if (len > 0 && len <= 64) {
                    uint8_t buf[64] = {};
                    os_mbuf_copydata(event->notify_rx.om, 0, len, buf);
                    for (const auto& rpt : ctx.inputRpts) {
                        if (rpt.valHandle != event->notify_rx.attr_handle) continue;
                        switch (rpt.type) {
                            case HidReportType::Keyboard:  hidHostHandleKeyboardReport(ctx, buf, len); break;
                            case HidReportType::Mouse:     hidHostHandleMouseReport(buf, len); break;
                            case HidReportType::Consumer:
                                LOG_I(TAG, "Consumer report len=%d", len);
                                break;
                            case HidReportType::Unknown:
                                if      (len >= 6) hidHostHandleKeyboardReport(ctx, buf, len);
                                else if (len >= 3) hidHostHandleMouseReport(buf, len);
                                break;
                        }
                        break;
                    }
                }
            }
            break;

        default:
            break;
    }
    return 0;
}

// ---- Public functions ----

// Reports an unsuccessful HID host attach back to the UI. Without this a tap on a scan result
// that turns out not to be a HID peripheral (a speaker, a phone, ...) fails entirely silently:
// the connect is refused or the peer is dropped again after service discovery, and nothing ever
// reaches the app, so the button appears dead.
static void fireHidHostFailure(const std::array<uint8_t, 6>& addr) {
    Device* dev;
    if (device_get_first_active_by_type(&BLUETOOTH_TYPE, &dev) != ERROR_NONE) {
        return;
    }
    BtEvent e = {};
    e.type = BT_EVENT_PAIR_RESULT;
    std::memcpy(e.pair_result.addr, addr.data(), 6);
    e.pair_result.result = BT_PAIR_RESULT_FAILED;
    e.pair_result.profile = BT_PROFILE_HID_HOST;
    bluetooth_fire_event(dev, e);
    device_put(dev);
}

void hidHostConnect(const std::array<uint8_t, 6>& addr) {
    if (getRadioState() != RadioState::On) {
        LOG_W(TAG, "hidHostConnect: radio not on");
        return;
    }
    if (hid_host_ctx) {
        LOG_W(TAG, "hidHostConnect: already connecting/connected");
        return;
    }

    hid_host_mouse_x.store(0);
    hid_host_mouse_y.store(0);
    hid_host_mouse_btn.store(false);
    hid_host_mouse_active.store(false);

    hid_host_ctx = std::make_unique<HidHostCtx>();
    hid_host_ctx->peerAddr = addr;

    // Create enc retry timer lazily
    if (hid_enc_retry_timer == nullptr) {
        esp_timer_create_args_t args = {};
        args.callback        = hidEncRetryTimerCb;
        args.dispatch_method = ESP_TIMER_TASK;
        args.name            = "hid_enc_retry";
        if (esp_timer_create(&args, &hid_enc_retry_timer) != ESP_OK) {
            LOG_E(TAG, "Failed to create hid_enc_retry timer");
            hid_enc_retry_timer = nullptr;
        }
    }

    // Notify driver that a HID host central connection is starting.
    {
        Device* dev;
        if (device_get_first_active_by_type(&BLUETOOTH_TYPE, &dev) == ERROR_NONE) {
            bluetooth_set_hid_host_active(dev, true);
            device_put(dev);
        }
    }

    hidHostConnectInitiate();
}

// Retry callback: the controller was busy (name-resolution or a prior connect), so re-attempt the
// connect now that it may have freed up.
static void hidHostConnectRetryCb(void* /*arg*/) {
    hid_host_connect_retry_scheduled = false;
    if (hid_host_ctx) {
        LOG_I(TAG, "Retrying HID host connect");
        hidHostConnectInitiate();
    }
}

static void hidHostConnectInitiate() {
    if (hid_host_ctx == nullptr) {
        return;
    }

    // Stop any running scan before initiating a central connection. The BLE controller cannot scan
    // and connect simultaneously, so cancel discovery first to free the controller.
    {
        Device* scan_dev = nullptr;
        if (device_get_first_active_by_type(&BLUETOOTH_TYPE, &scan_dev) == ERROR_NONE) {
            if (bluetooth_is_scanning(scan_dev)) {
                LOG_I(TAG, "Stopping active scan before HID host connect");
                bluetooth_scan_stop(scan_dev);
            }
            device_put(scan_dev);
        }
    }

    // Look up the addr_type from the cached scan results.
    ble_addr_t ble_addr = {};
    ble_addr.type = BLE_ADDR_PUBLIC;
    std::memcpy(ble_addr.val, hid_host_ctx->peerAddr.data(), 6);
    uint8_t addr_type = 0;
    if (getCachedScanAddrType(hid_host_ctx->peerAddr.data(), &addr_type)) {
        ble_addr.type = addr_type;
    }

    uint8_t own_addr_type;
    if (ble_hs_id_infer_auto(0, &own_addr_type) != 0) {
        own_addr_type = BLE_OWN_ADDR_PUBLIC;
    }

    // Create the EALREADY retry timer lazily.
    if (hid_host_connect_retry_timer == nullptr) {
        esp_timer_create_args_t retry_args = {};
        retry_args.callback        = hidHostConnectRetryCb;
        retry_args.dispatch_method = ESP_TIMER_TASK;
        retry_args.name            = "hid_connect_retry";
        if (esp_timer_create(&retry_args, &hid_host_connect_retry_timer) != ESP_OK) {
            LOG_W(TAG, "Failed to create connect retry timer");
            hid_host_connect_retry_timer = nullptr;
        }
    }

    int rc = ble_gap_connect(own_addr_type, &ble_addr, 5000, nullptr, hidHostGapCb, nullptr);
    if (rc != 0) {
        if (rc == BLE_HS_EALREADY) {
            // Controller already connecting (name-resolution or a concurrent attempt). Retry
            // shortly instead of failing silently — this is what made a tap appear to do nothing.
            LOG_W(TAG, "Connect busy (EALREADY) — retrying in 500ms");
            if (hid_host_connect_retry_timer != nullptr && !hid_host_connect_retry_scheduled) {
                hid_host_connect_retry_scheduled = true;
                esp_timer_stop(hid_host_connect_retry_timer);
                esp_timer_start_once(hid_host_connect_retry_timer, 500 * 1000);
            }
            return;
        }
        LOG_W(TAG, "ble_gap_connect failed rc=%d", rc);
        fireHidHostFailure(hid_host_ctx->peerAddr);
        hid_host_ctx.reset();
        Device* dev;
        if (device_get_first_active_by_type(&BLUETOOTH_TYPE, &dev) == ERROR_NONE) {
            bluetooth_set_hid_host_active(dev, false);
            // Fire IDLE so bt_event_bridge can start a new scan and retry.
            BtEvent e = {};
            e.type = BT_EVENT_PROFILE_STATE_CHANGED;
            e.profile_state.state   = BT_PROFILE_STATE_IDLE;
            e.profile_state.profile = BT_PROFILE_HID_HOST;
            bluetooth_fire_event(dev, e);
            device_put(dev);
        }
    } else {
        if (hid_host_connect_retry_timer != nullptr) {
            esp_timer_stop(hid_host_connect_retry_timer);
        }
        hid_host_connect_retry_scheduled = false;
        LOG_I(TAG, "Connecting...");
    }
}

void hidHostDisconnect() {
    if (!hid_host_ctx || hid_host_ctx->connHandle == BLE_HS_CONN_HANDLE_NONE) return;
    ble_gap_terminate(hid_host_ctx->connHandle, BLE_ERR_REM_USER_CONN_TERM);
}

bool hidHostIsConnected() {
    return hid_host_ctx != nullptr &&
           hid_host_ctx->connHandle != BLE_HS_CONN_HANDLE_NONE &&
           !hid_host_ctx->inputRpts.empty() &&
           hid_host_ctx->subscribeIdx >= (int)hid_host_ctx->inputRpts.size();
}

bool hidHostGetConnectedPeer(std::array<uint8_t, 6>& addr_out) {
    if (!hidHostIsConnected()) return false;
    addr_out = hid_host_ctx->peerAddr;
    return true;
}

void autoConnectHidHost() {
    if (hidHostIsConnected()) return;

    // Connect to the first saved HID host peer that appeared in the last scan.
    // cacheScanAddr() is populated during scanning so addr_type is available for ble_gap_connect.
    auto scan = getScanResults();
    for (const auto& r : scan) {
        settings::PairedDevice stored;
        if (settings::load(settings::addrToHex(r.addr), stored) &&
            stored.autoConnect &&
            stored.profileId == BT_PROFILE_HID_HOST) {
            LOG_I(TAG, "Auto-connecting HID host to %s", settings::addrToHex(r.addr).c_str());
            hidHostConnect(r.addr);
            return;
        }
    }

    // Device not in the last scan. If we have an autoConnect HID host peer, restart
    // scanning so we keep checking until the device powers back on.
    auto peers = settings::loadAll();
    for (const auto& peer : peers) {
        if (peer.autoConnect && peer.profileId == BT_PROFILE_HID_HOST) {
            Device* dev;
            if (device_get_first_active_by_type(&BLUETOOTH_TYPE, &dev) == ERROR_NONE) {
                if (!bluetooth_is_scanning(dev)) {
                    LOG_I(TAG, "Auto-connect HID host: device not in scan, retrying scan");
                    bluetooth_scan_start(dev);
                }
                device_put(dev);
            }
            break;
        }
    }
}

} // namespace tt::bluetooth

#endif // CONFIG_BT_NIMBLE_ENABLED
