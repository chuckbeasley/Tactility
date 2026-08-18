#include <string>
#include <cstdio>
#include <cstring>

#include <Tactility/network/HttpdReq.h>
#include <Tactility/app/wifimanage/View.h>
#include <Tactility/app/wifimanage/WifiManagePrivate.h>
#include <Tactility/lvgl/Style.h>
#include <Tactility/lvgl/Toolbar.h>
#include <Tactility/service/wifi/Wifi.h>
#include <Tactility/service/wifi/WifiSettings.h>
#include <Tactility/Tactility.h>

#include <app/event.h>
#include <lvgl/widgets/toolbar.h>

#include <tactility/log.h>
#include <lvgl/lvgl.h>

namespace tt::app::wifimanage {

constexpr auto* TAG = "WifiManageView";
static constexpr size_t WIFI_LIST_PAGE_SIZE = 8;

static void onBackPressed(lv_event_t* event) {
    auto* appInstanceId = static_cast<uint32_t*>(lv_event_get_user_data(event));
    // Async, non-blocking - must NOT call app_manager_stop() directly here: that bound-waits
    // (thread_join) for this app's own thread to finish, which needs the LVGL lock
    // (window_manager_remove()) - but this callback runs ON the LVGL task, which would
    // deadlock against itself.
    AppEvent closeEvent { .type = APP_EVENT_CLOSE, .timestamp = 0, .result = {} };
    app_event_emit(*appInstanceId, &closeEvent);
}

static uint8_t mapRssiToPercentage(int rssi) {
    auto abs_rssi = std::abs(rssi);
    if (abs_rssi < 30U) {
        abs_rssi = 30U;
    } else if (abs_rssi > 90U) {
        abs_rssi = 90U;
    }

    auto percentage = (float)(90U - abs_rssi) / 60.f * 100.f;
    return (uint8_t)percentage;
}

static void onEnableSwitchChanged(lv_event_t* event) {
    auto* enable_switch = static_cast<lv_obj_t*>(lv_event_get_target(event));
    bool is_on = lv_obj_has_state(enable_switch, LV_STATE_CHECKED);
    auto* bindings = static_cast<Bindings*>(lv_event_get_user_data(event));
    bindings->onWifiToggled(is_on);
}

static void onEnableOnBootSwitchChanged(lv_event_t* event) {
    auto* enable_switch = static_cast<lv_obj_t*>(lv_event_get_target(event));
    bool is_on = lv_obj_has_state(enable_switch, LV_STATE_CHECKED);
    // Dispatch it, so file IO doesn't block the UI
    getMainDispatcher().dispatch([is_on] {
        service::wifi::settings::setEnableOnBoot(is_on);
    });
}

static void onEnableOnBootParentClicked(lv_event_t* event) {
    auto* enable_switch = static_cast<lv_obj_t*>(lv_event_get_user_data(event));
    if (lv_obj_has_state(enable_switch, LV_STATE_CHECKED)) {
        lv_obj_remove_state(enable_switch, LV_STATE_CHECKED);
    } else {
        lv_obj_add_state(enable_switch, LV_STATE_CHECKED);
    }
}

static void onConnectToHiddenClicked(lv_event_t* event) {
    auto* bindings = (Bindings*)lv_event_get_user_data(event);
    bindings->onConnectToHidden();
}

// region Secondary updates

void View::connect(lv_event_t* event) {
    LOG_D(TAG, "connect()");
    auto* widget = lv_event_get_current_target_obj(event);
    auto index = reinterpret_cast<size_t>(lv_obj_get_user_data(widget));
    auto* self = static_cast<View*>(lv_event_get_user_data(event));
    auto ap_records = self->state->getApRecords();

    if (index < ap_records.size()) {
        LOG_I(TAG, "Clicked %zu/%zu", index, ap_records.size() - 1);
        std::string ssid = ap_records[index].ssid;
        LOG_I(TAG, "Clicked AP: %s", ssid.c_str());
        std::string connection_target = service::wifi::getConnectionTarget();
        if (connection_target == ssid) {
            self->bindings->onDisconnect();
        } else {
            self->bindings->onConnectSsid(ssid);
        }
    } else {
        LOG_W(TAG, "Clicked AP: record %zu/%zu does not exist", index, ap_records.size() - 1);
    }
}

void View::showDetails(lv_event_t* event) {
    LOG_D(TAG, "showDetails()");
    auto* widget = lv_event_get_current_target_obj(event);
    auto index = reinterpret_cast<size_t>(lv_obj_get_user_data(widget));
    auto* self = static_cast<View*>(lv_event_get_user_data(event));
    auto ap_records = self->state->getApRecords();

    if (index < ap_records.size()) {
        std::string ssid = ap_records[index].ssid;
        LOG_I(TAG, "Clicked AP: %s", ssid.c_str());
        self->bindings->onShowApSettings(ssid);
    } else {
        LOG_W(TAG, "Clicked AP: record %zu/%zu does not exist", index, ap_records.size() - 1);
    }
}

void View::onShowMoreClicked(lv_event_t* event) {
    auto* self = static_cast<View*>(lv_event_get_user_data(event));
    self->visibleNetworksLimit += WIFI_LIST_PAGE_SIZE;
    self->updateNetworkList();
    self->updateConnectToHidden();
}

void View::createSsidListItem(const WifiApRecord& record, bool isConnecting, size_t index) {
    if (isConnecting) {
        auto* button = lv_list_add_button(networks_list, LV_SYMBOL_WIFI, record.ssid);
        lv_obj_add_event_cb(button, showDetails, LV_EVENT_SHORT_CLICKED, this);
    } else {
        const char* auth_info = (record.authentication_type == WIFI_AUTHENTICATION_TYPE_OPEN) ? "(open) " : "";
        const auto percentage = mapRssiToPercentage(record.rssi);
        char label[96];
        std::snprintf(label, sizeof(label), "%s %s%u%%", record.ssid, auth_info, percentage);
        auto* button = lv_list_add_button(networks_list, nullptr, label);
        lv_obj_set_user_data(button, reinterpret_cast<void*>(index));
        // Avoid filesystem checks in the LVGL render path; onConnect handles known/unknown SSIDs.
        lv_obj_add_event_cb(button, connect, LV_EVENT_SHORT_CLICKED, this);
    }
}

void View::updateConnectToHidden() {
    if (connect_to_hidden == nullptr) {
        return;
    }

    using enum service::wifi::RadioState;
    switch (state->getRadioState()) {
        case On:
        case ConnectionPending:
        case ConnectionActive:
            lv_obj_remove_flag(connect_to_hidden, LV_OBJ_FLAG_HIDDEN);
            break;

        case OnPending:
        case OffPending:
        case Off:
            lv_obj_add_flag(connect_to_hidden, LV_OBJ_FLAG_HIDDEN);
            break;
    }
}

void View::updateNetworkList() {
    lv_obj_clean(networks_list);

    // Enable on boot

    auto* enable_on_boot_wrapper = lv_obj_create(networks_list);
    lv_obj_set_size(enable_on_boot_wrapper, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_set_style_pad_all(enable_on_boot_wrapper, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(enable_on_boot_wrapper, 0, LV_STATE_DEFAULT);

    auto* enable_label = lv_label_create(enable_on_boot_wrapper);
    lv_label_set_text(enable_label, "Enable on boot");
    lv_obj_align(enable_label, LV_ALIGN_LEFT_MID, 0, 0);

    enable_on_boot_switch = lv_switch_create(enable_on_boot_wrapper);
    lv_obj_align(enable_on_boot_switch, LV_ALIGN_RIGHT_MID, 0, 0);
    lv_obj_add_event_cb(enable_on_boot_switch, onEnableOnBootSwitchChanged, LV_EVENT_VALUE_CHANGED, bindings);
    lv_obj_add_event_cb(enable_on_boot_wrapper, onEnableOnBootParentClicked, LV_EVENT_SHORT_CLICKED, enable_on_boot_switch);

    if (lvgl_get_ui_density() == LVGL_UI_DENSITY_COMPACT) {
        lv_obj_set_style_pad_ver(enable_on_boot_wrapper, 2, LV_STATE_DEFAULT);
    } else {
        lv_obj_set_style_pad_ver(enable_on_boot_wrapper, 8, LV_STATE_DEFAULT);
    }

    updateEnableOnBootToggle();

    switch (state->getRadioState()) {
        using enum service::wifi::RadioState;
        case OnPending:
        case On:
        case ConnectionPending:
        case ConnectionActive: {
            const auto ui_radio_state = state->getRadioState();
            const std::string connection_target = service::wifi::getConnectionTarget();
            const bool is_connected = !connection_target.empty() && ui_radio_state == ConnectionActive;
            const bool is_connecting_state = ui_radio_state == ConnectionPending && !connection_target.empty();

            bool added_connected = false;
            bool has_records = false;
            size_t shown_networks = 0;
            bool truncated = false;

            state->withApRecords([&](const std::vector<WifiApRecord>& ap_records) {
                has_records = !ap_records.empty();
                if (!has_records) {
                    return;
                }

                if (is_connected) {
                    for (size_t i = 0; i < ap_records.size(); ++i) {
                        const auto& record = ap_records[i];
                        if (record.ssid == connection_target) {
                            lv_list_add_text(networks_list, "Connected");
                            createSsidListItem(record, false, i);
                            added_connected = true;
                            break;
                        }
                    }
                }

                lv_list_add_text(networks_list, "Other networks");
                for (size_t i = 0; i < ap_records.size(); ++i) {
                    const auto& record = ap_records[i];

                    // De-duplicate SSIDs without heap allocations.
                    bool seen = false;
                    for (size_t j = 0; j < i; ++j) {
                        if (std::strcmp(ap_records[j].ssid, record.ssid) == 0) {
                            seen = true;
                            break;
                        }
                    }
                    if (seen) {
                        continue;
                    }

                    const bool connection_target_match = (record.ssid == connection_target);
                    const bool is_connecting = connection_target_match && is_connecting_state;
                    const bool skip = connection_target_match && added_connected;
                    if (skip) {
                        continue;
                    }

                    if (shown_networks >= visibleNetworksLimit) {
                        truncated = true;
                        break;
                    }
                    createSsidListItem(record, is_connecting, i);
                    shown_networks++;
                }
            });

            if (has_records) {
                if (truncated) {
                    auto* show_more = lv_list_add_button(networks_list, nullptr, "Show more networks");
                    lv_obj_add_event_cb(show_more, onShowMoreClicked, LV_EVENT_SHORT_CLICKED, this);
                }
                lv_obj_clear_flag(networks_list, LV_OBJ_FLAG_HIDDEN);
            } else if (!state->hasScannedAfterRadioOn() || state->isScanning()) {
                // hasScannedAfterRadioOn() prevents briefly showing "No networks found" when turning radio on.
                lv_obj_add_flag(networks_list, LV_OBJ_FLAG_HIDDEN);
            } else {
                lv_obj_clear_flag(networks_list, LV_OBJ_FLAG_HIDDEN);
                lv_obj_t* label = lv_label_create(networks_list);
                lv_label_set_text(label, "No networks found.");
            }

            connect_to_hidden = lv_button_create(networks_list);
            lv_obj_set_width(connect_to_hidden, LV_PCT(100));
            lv_obj_set_style_margin_ver(connect_to_hidden, 4, LV_STATE_DEFAULT);
            auto* connect_to_hidden_label = lv_label_create(connect_to_hidden);
            lv_label_set_text(connect_to_hidden_label, "Connect to hidden SSID");
            lv_obj_add_event_cb(connect_to_hidden, onConnectToHiddenClicked, LV_EVENT_SHORT_CLICKED, bindings);
            break;
        }

        default:
            connect_to_hidden = nullptr;
            // Nothing to do
            break;
    }

}

void View::updateScanning() {
    if (state->getRadioState() == service::wifi::RadioState::On && state->isScanning()) {
        lv_obj_remove_flag(scanning_spinner, LV_OBJ_FLAG_HIDDEN);
    } else {
        lv_obj_add_flag(scanning_spinner, LV_OBJ_FLAG_HIDDEN);
    }
}

void View::updateWifiToggle() {
    lv_obj_clear_state(enable_switch, LV_STATE_ANY);
    switch (state->getRadioState()) {
        using enum service::wifi::RadioState;
        case On:
        case ConnectionPending:
        case ConnectionActive:
            lv_obj_add_state(enable_switch, LV_STATE_CHECKED);
            break;
        case OnPending:
            lv_obj_add_state(enable_switch, LV_STATE_CHECKED);
            lv_obj_add_state(enable_switch, LV_STATE_DISABLED);
            break;
        case Off:
            lv_obj_remove_state(enable_switch, LV_STATE_CHECKED);
            lv_obj_remove_state(enable_switch, LV_STATE_DISABLED);
            break;
        case OffPending:
            lv_obj_remove_state(enable_switch, LV_STATE_CHECKED);
            lv_obj_add_state(enable_switch, LV_STATE_DISABLED);
            break;
    }
}

void View::updateEnableOnBootToggle() {
    if (enable_on_boot_switch != nullptr) {
        lv_obj_clear_state(enable_on_boot_switch, LV_STATE_ANY);
        if (service::wifi::settings::shouldEnableOnBoot()) {
            lv_obj_add_state(enable_on_boot_switch, LV_STATE_CHECKED);
        } else {
            lv_obj_remove_state(enable_on_boot_switch, LV_STATE_CHECKED);
        }
    }
}

// endregion Secondary updates

// region Main

void View::init(uint32_t newAppInstanceId, lv_obj_t* parent) {
    appInstanceId = newAppInstanceId;
    visibleNetworksLimit = WIFI_LIST_PAGE_SIZE;

    lv_obj_set_flex_flow(parent, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_row(parent, 0, LV_STATE_DEFAULT);

    root = parent;

    // Toolbar

    lv_obj_t* toolbar = lvgl_toolbar_create(parent, "Wi-Fi");
    lvgl_toolbar_set_nav_action(toolbar, LV_SYMBOL_CLOSE, onBackPressed, &appInstanceId);

    scanning_spinner = lvgl_toolbar_add_spinner_action(toolbar);

    enable_switch = lvgl_toolbar_add_switch_action(toolbar);
    lv_obj_add_event_cb(enable_switch, onEnableSwitchChanged, LV_EVENT_VALUE_CHANGED, bindings);

     // Networks

    networks_list = lv_list_create(parent);
    lv_obj_set_flex_grow(networks_list, 1);
    lv_obj_set_width(networks_list, LV_PCT(100));
}

void View::update() {
    if (root == nullptr) {
        // Buried (or not yet built) - see reset().
        return;
    }
    auto current_radio = state->getRadioState();
    auto current_scanning = state->isScanning();
    auto current_ap_count = state->getApRecordCount();
    auto current_connection_target = service::wifi::getConnectionTarget();

    updateWifiToggle();
    updateScanning();

    const bool list_changed = !hasRenderedList ||
        current_radio != lastListRadioState ||
        current_ap_count != lastApCount ||
        current_connection_target != lastConnectionTarget;
    if (list_changed) {
        updateNetworkList();
        updateConnectToHidden();
        hasRenderedList = true;
        lastListRadioState = current_radio;
        lastListScanning = current_scanning;
        lastApCount = current_ap_count;
        lastConnectionTarget = current_connection_target;
    }
}

void View::reset() {
    root = nullptr;
    enable_switch = nullptr;
    enable_on_boot_switch = nullptr;
    scanning_spinner = nullptr;
    networks_list = nullptr;
    connect_to_hidden = nullptr;
    visibleNetworksLimit = WIFI_LIST_PAGE_SIZE;
    hasRenderedList = false;
    lastConnectionTarget.clear();
}

} // namespace
