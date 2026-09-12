#pragma once

#include "./Bindings.h"
#include "./State.h"

#include <array>
#include <cstdint>
#include <lvgl.h>
#include <vector>

namespace tt::app::btmanage {

class View final {

    Bindings* bindings;
    State* state;
    // Passed through to onBtToggled/onScanToggled/onPairPeer via lv_obj user_data - see
    // Bindings.h. Set in init(), before any callback can fire.
    void* context = nullptr;
    lv_obj_t* root = nullptr;
    lv_obj_t* enable_switch = nullptr;
    lv_obj_t* enable_on_boot_switch = nullptr;
    lv_obj_t* scanning_spinner = nullptr;
    lv_obj_t* peers_list = nullptr;

    // The peer address behind each rendered row, in row order, cleared and refilled on every
    // rebuild. A row carries an index into *this* rather than into the live scan results: the list
    // is rebuilt while a scan runs, so an index into the results would, by the time the click is
    // delivered, name whichever peer happens to sit at that position now - or nothing at all, in
    // which case the click did silently nothing. This way a click always acts on the row the user
    // pressed, even if the list has moved on underneath it.
    //
    // The alternative - a heap allocation per row holding its own address - is what View.cpp's
    // PeerListItemData note says leaked when LVGL cleared user_data before firing LV_EVENT_DELETE,
    // so the addresses are held here instead, in one vector owned by the view.
    std::vector<std::array<uint8_t, 6>> rowAddresses;

    // Signature of the last rendered peer list. BT_EVENT_PEER_FOUND fires many times per
    // second while scanning; without this the list would be torn down and rebuilt on every
    // one of them, which resets the scroll offset and cancels in-progress touch gestures.
    bool hasRenderedList = false;
    bluetooth::RadioState lastListRadioState = bluetooth::RadioState::Off;
    bool lastListScanning = false;
    size_t lastScanResultCount = 0;
    size_t lastPairedCount = 0;
    size_t lastConnectedCount = 0;
    uint32_t lastListRenderTick = 0;
    bool listRebuildPending = false;
    lv_timer_t* rebuild_timer = nullptr;

    void createEnableOnBootRow(lv_obj_t* parent);
    void updateBtToggle();
    void updateEnableOnBootToggle();
    void updateScanning();
    void updatePeerList();
    bool isUserInteractingWithList() const;

    void createPeerListItem(const bluetooth::PeerRecord& record, bool isPaired);

    static void onConnect(lv_event_t* event);

public:

    View(Bindings* bindings, State* state) : bindings(bindings), state(state) {}

    void init(void* context, lv_obj_t* parent);
    void update();
};

} // namespace tt::app::btmanage
