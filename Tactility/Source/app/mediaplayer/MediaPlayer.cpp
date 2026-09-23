// SPDX-License-Identifier: Apache-2.0
//
// Media player: WAV, MP3 and FLAC files from storage, played through the audio stream device.
//
// Why the format handling is one call: decoding is delegated to Espressif's audio simple decoder
// (espressif/esp_audio_codec), which owns both the container parsing and the codec. That matters
// twice over on this board. Its MP3 decoder is fixed-point, which this target needs - the ESP32-C5
// is built with -march=rv32imac and has no FPU, so a float-based decoder would run in software
// emulation - and its parser means this app can feed it file bytes of any size instead of having to
// find frame boundaries itself.
//
// The stream's sample rate, channel count and bit depth are not known until the first frame decodes,
// so the decoder is opened first and the output stream is opened from the decoder's own report.
#include <dirent.h>

#include <atomic>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

#include <lvgl/lvgl.h>

#include <Tactility/Tactility.h>
#include <Tactility/service/audio/Audio.h>

#include <app/event.h>
#include <app/manager.h>
#include <app/manifest.h>
#include <app/scheduler.h>

#include <lvgl_window_manager/window_manager.h>

#include <tactility/check.h>
#include <tactility/device.h>
#include <tactility/drivers/audio_codec.h>
#include <tactility/drivers/audio_stream.h>
#include <tactility/log.h>

#include <lvgl/widgets/sliderbox.h>
#include <lvgl/widgets/toolbar.h>

#include <esp_audio_dec_default.h>
#include <esp_audio_simple_dec.h>
#include <esp_audio_simple_dec_default.h>

namespace tt::app::mediaplayer {

constexpr auto* TAG = "MediaPlayer";

// Where to look for audio: the TF card first, then a folder on the internal data partition, so the
// player is usable on a board with no card fitted.
constexpr const char* SEARCH_ROOTS[] = { "/sdcard", "/data/music" };

// How many tracks are rendered, and how deep the search goes. The cap is not only for legibility:
// this app's task has a fixed stack, and rendering a row into an LVGL list costs the better part of
// a kilobyte of it on this board - measured on the Bluetooth app, which crashed from exactly that.
// The list is therefore built once and never rebuilt; selecting a track only restyles one row.
constexpr size_t MAX_TRACKS = 30;
constexpr int MAX_SEARCH_DEPTH = 1;

// Decode buffers live on the heap deliberately: a decoder frame is several kilobytes, and this runs
// on the app's own task stack.
constexpr size_t INPUT_BUFFER_SIZE = 4096;
constexpr size_t OUTPUT_BUFFER_SIZE = 8192;

// The UI is refreshed at this rate rather than every loop pass, since the loop runs roughly every
// 20 ms to keep the decoder fed.
constexpr TickType_t UI_REFRESH_INTERVAL_TICKS = pdMS_TO_TICKS(200);

struct Track {
    std::string path;
    std::string name;
    esp_audio_simple_dec_type_t type = ESP_AUDIO_SIMPLE_DEC_TYPE_NONE;
};

struct Context {
    uint32_t appInstanceId = 0;

    // Requests from the widget callbacks. Those run on the LVGL task, so they only set flags and
    // never touch the decoder, which belongs to the app task.
    std::atomic<bool> closeRequested{false};
    std::atomic<int> startTrackRequest{-1};
    std::atomic<bool> togglePauseRequest{false};
    std::atomic<bool> stopRequest{false};
    std::atomic<int> skipRequest{0};

    std::vector<Track> tracks;
    int currentTrack = -1;
    bool paused = false;
    bool failed = false;
    std::string status = "Stopped";
    TickType_t lastUiUpdate = 0;

    // Playback state, owned by the app task alone.
    FILE* file = nullptr;
    esp_audio_simple_dec_handle_t decoder = nullptr;
    Device* streamDevice = nullptr;
    AudioStreamHandle stream = nullptr;
    uint8_t* inputBuffer = nullptr;
    uint8_t* outputBuffer = nullptr;
    size_t outputCapacity = 0;
    uint64_t decodedBytes = 0;

    // Widgets
    lv_obj_t* trackLabel = nullptr;
    lv_obj_t* statusLabel = nullptr;
    lv_obj_t* listWidget = nullptr;
    lv_obj_t* playPauseLabel = nullptr;
    lv_obj_t* volumeSlider = nullptr;
    std::vector<lv_obj_t*> rowButtons;
};

// region Scanning

bool endsWithIgnoreCase(const std::string& value, const char* suffix) {
    const size_t suffixLength = strlen(suffix);
    if (value.size() < suffixLength) {
        return false;
    }
    const size_t offset = value.size() - suffixLength;
    for (size_t i = 0; i < suffixLength; ++i) {
        if (tolower(static_cast<unsigned char>(value[offset + i])) != tolower(static_cast<unsigned char>(suffix[i]))) {
            return false;
        }
    }
    return true;
}

esp_audio_simple_dec_type_t typeForName(const std::string& name) {
    if (endsWithIgnoreCase(name, ".mp3")) return ESP_AUDIO_SIMPLE_DEC_TYPE_MP3;
    if (endsWithIgnoreCase(name, ".wav")) return ESP_AUDIO_SIMPLE_DEC_TYPE_WAV;
    if (endsWithIgnoreCase(name, ".flac")) return ESP_AUDIO_SIMPLE_DEC_TYPE_FLAC;
    return ESP_AUDIO_SIMPLE_DEC_TYPE_NONE;
}

void scanDirectory(const std::string& path, int depth, std::vector<Track>& out) {
    DIR* directory = opendir(path.c_str());
    if (directory == nullptr) {
        return;
    }

    while (out.size() < MAX_TRACKS) {
        dirent* entry = readdir(directory);
        if (entry == nullptr) {
            break;
        }
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        std::string child = path + "/" + entry->d_name;
        if (entry->d_type == DT_DIR) {
            if (depth < MAX_SEARCH_DEPTH) {
                scanDirectory(child, depth + 1, out);
            }
            continue;
        }

        const auto type = typeForName(entry->d_name);
        if (type != ESP_AUDIO_SIMPLE_DEC_TYPE_NONE) {
            out.push_back({ child, entry->d_name, type });
        }
    }

    closedir(directory);
}

void collectTracks(std::vector<Track>& out) {
    out.clear();
    for (const char* root : SEARCH_ROOTS) {
        scanDirectory(root, 0, out);
        if (!out.empty()) {
            break;
        }
    }
}

// endregion

// region Playback (app task only)

void closeStream(Context* ctx) {
    if (ctx->stream != nullptr) {
        audio_stream_close(ctx->stream);
        ctx->stream = nullptr;
    }
}

void stopPlayback(Context* ctx) {
    closeStream(ctx);
    if (ctx->decoder != nullptr) {
        esp_audio_simple_dec_close(ctx->decoder);
        ctx->decoder = nullptr;
    }
    if (ctx->file != nullptr) {
        fclose(ctx->file);
        ctx->file = nullptr;
    }
    ctx->decodedBytes = 0;
}

bool openTrack(Context* ctx, int index) {
    stopPlayback(ctx);
    ctx->failed = false;

    if (index < 0 || index >= static_cast<int>(ctx->tracks.size())) {
        return false;
    }

    const Track& track = ctx->tracks[index];
    ctx->file = fopen(track.path.c_str(), "rb");
    if (ctx->file == nullptr) {
        ctx->status = "Cannot open " + track.name;
        ctx->failed = true;
        return false;
    }

    esp_audio_simple_dec_cfg_t cfg = {};
    cfg.dec_type = track.type;
    cfg.dec_cfg = nullptr;
    cfg.cfg_size = 0;
    // Container parsing is the point: the file is fed in arbitrary chunks.
    cfg.use_frame_dec = false;
    const esp_audio_err_t openResult = esp_audio_simple_dec_open(&cfg, &ctx->decoder);
    if (openResult != ESP_AUDIO_ERR_OK) {
        ctx->status = std::string("No decoder for ") + esp_audio_simple_dec_get_name(track.type);
        LOG_W(TAG, "decoder open failed for %s: %d", track.name.c_str(), (int)openResult);
        ctx->failed = true;
        fclose(ctx->file);
        ctx->file = nullptr;
        return false;
    }

    ctx->currentTrack = index;
    ctx->paused = false;
    ctx->status = "Playing";
    LOG_I(TAG, "opening %s as %s", track.path.c_str(), esp_audio_simple_dec_get_name(track.type));
    return true;
}

/** Opens the output stream from what the decoder reports, once and only after the first frame. */
bool ensureStreamOpen(Context* ctx) {
    if (ctx->stream != nullptr) {
        return true;
    }
    if (ctx->streamDevice == nullptr) {
        ctx->status = "No audio output";
        ctx->failed = true;
        return false;
    }

    esp_audio_simple_dec_info_t info = {};
    if (esp_audio_simple_dec_get_info(ctx->decoder, &info) != ESP_AUDIO_ERR_OK || info.sample_rate == 0) {
        // Only known once a frame has decoded; the caller tries again on the next frame.
        return false;
    }

    const uint8_t bits = info.bits_per_sample != 0 ? info.bits_per_sample : 16;
    AudioStreamConfig config = { info.sample_rate, bits, static_cast<uint8_t>(info.channel != 0 ? info.channel : 2) };
    if (audio_stream_open_output(ctx->streamDevice, &config, &ctx->stream) != ERROR_NONE) {
        ctx->status = std::string("Output refuses ") + std::to_string(info.sample_rate) + " Hz";
        ctx->failed = true;
        return false;
    }

    LOG_I(TAG, "output opened: %u Hz, %u bit, %u channel(s)", (unsigned)info.sample_rate, (unsigned)bits,
          (unsigned)config.channels);
    return true;
}

/** Decodes and plays one buffer's worth of file data. False when the track has finished. */
bool pumpOnce(Context* ctx) {
    const size_t readLength = fread(ctx->inputBuffer, 1, INPUT_BUFFER_SIZE, ctx->file);
    const bool endOfFile = readLength == 0;

    uint32_t offset = 0;
    bool produced = false;

    while (true) {
        esp_audio_simple_dec_raw_t raw = {};
        raw.buffer = ctx->inputBuffer + offset;
        raw.len = static_cast<uint32_t>(readLength) - offset;
        raw.eos = endOfFile;

        esp_audio_simple_dec_out_t out = {};
        out.buffer = ctx->outputBuffer;
        out.len = static_cast<uint32_t>(ctx->outputCapacity);

        const esp_audio_err_t result = esp_audio_simple_dec_process(ctx->decoder, &raw, &out);
        offset += raw.consumed;

        if (result == ESP_AUDIO_ERR_BUFF_NOT_ENOUGH) {
            const size_t needed = out.needed_size != 0 ? out.needed_size : ctx->outputCapacity * 2;
            auto* grown = static_cast<uint8_t*>(realloc(ctx->outputBuffer, needed));
            if (grown == nullptr) {
                ctx->status = "Out of memory";
                ctx->failed = true;
                return false;
            }
            ctx->outputBuffer = grown;
            ctx->outputCapacity = needed;
            continue;
        }

        if (result != ESP_AUDIO_ERR_OK && result != ESP_AUDIO_ERR_CONTINUE && result != ESP_AUDIO_ERR_DATA_LACK) {
            LOG_W(TAG, "decode error %d after %llu bytes", (int)result, (unsigned long long)ctx->decodedBytes);
            ctx->status = "Decode error";
            ctx->failed = true;
            return false;
        }

        if (out.decoded_size > 0) {
            produced = true;
            if (!ensureStreamOpen(ctx)) {
                return false;
            }
            size_t written = 0;
            if (audio_stream_write(ctx->stream, ctx->outputBuffer, out.decoded_size, &written,
                                   pdMS_TO_TICKS(2000)) != ERROR_NONE) {
                ctx->status = "Output stalled";
                ctx->failed = true;
                return false;
            }
            ctx->decodedBytes += written;
        }

        // Stop when there is nothing left to do with what we hold: either the input chunk is used up
        // and the decoder produced nothing more, or it made no progress at all.
        if (raw.consumed == 0 && out.decoded_size == 0) {
            break;
        }
        if (offset >= readLength) {
            break;
        }
    }

    // At end of stream the loop above has already asked the decoder to flush, so producing nothing
    // here means the track really is finished.
    return !(endOfFile && !produced);
}

// endregion

// region State machine, run from appMain

void serviceRequests(Context* ctx) {
    if (ctx->stopRequest.exchange(false)) {
        stopPlayback(ctx);
        ctx->status = "Stopped";
        ctx->currentTrack = -1;
        ctx->failed = false;
    }

    if (ctx->togglePauseRequest.exchange(false) && ctx->decoder != nullptr) {
        ctx->paused = !ctx->paused;
        ctx->status = ctx->paused ? "Paused" : "Playing";
    }

    const int skip = ctx->skipRequest.exchange(0);
    if (skip != 0 && !ctx->tracks.empty()) {
        int next = ctx->currentTrack + skip;
        if (next < 0) {
            next = static_cast<int>(ctx->tracks.size()) - 1;
        } else if (next >= static_cast<int>(ctx->tracks.size())) {
            next = 0;
        }
        openTrack(ctx, next);
    }

    const int requested = ctx->startTrackRequest.exchange(-1);
    if (requested >= 0) {
        openTrack(ctx, requested);
    }
}

void playLoop(Context* ctx) {
    if (ctx->decoder == nullptr || ctx->paused) {
        return;
    }

    if (pumpOnce(ctx)) {
        return;
    }

    if (ctx->failed) {
        // A decode or output failure: stop rather than marching through every remaining track.
        stopPlayback(ctx);
        ctx->currentTrack = -1;
        ctx->failed = false;
        return;
    }

    const int next = ctx->currentTrack + 1;
    if (next < static_cast<int>(ctx->tracks.size())) {
        openTrack(ctx, next);
    } else {
        stopPlayback(ctx);
        ctx->status = "Finished";
        ctx->currentTrack = -1;
    }
}

// endregion

// region Widgets

void refreshHighlight(Context* ctx) {
    for (size_t i = 0; i < ctx->rowButtons.size(); ++i) {
        lv_obj_t* button = ctx->rowButtons[i];
        const bool active = static_cast<int>(i) == ctx->currentTrack;
        lv_obj_set_style_bg_color(button, active ? lv_palette_main(LV_PALETTE_BLUE) : lv_color_hex(0x303030),
                                 LV_STATE_DEFAULT);
    }
}

void updateStatusWidgets(Context* ctx) {
    if (ctx->statusLabel == nullptr) {
        return;
    }

    const char* name = ctx->currentTrack >= 0 ? ctx->tracks[ctx->currentTrack].name.c_str() : "-";
    lv_label_set_text(ctx->trackLabel, name);
    lv_label_set_text(ctx->statusLabel, ctx->status.c_str());
    lv_label_set_text(ctx->playPauseLabel, ctx->paused ? LV_SYMBOL_PLAY : LV_SYMBOL_PAUSE);
    refreshHighlight(ctx);
}

// endregion

// region Callbacks (these run on the LVGL task)

void onBackPressed(lv_event_t* event) {
    static_cast<Context*>(lv_event_get_user_data(event))->closeRequested.store(true);
}

void onTrackClicked(lv_event_t* event) {
    auto* button = static_cast<lv_obj_t*>(lv_event_get_target(event));
    auto* ctx = static_cast<Context*>(lv_event_get_user_data(event));
    // Stored as index + 1 so that the first row is not indistinguishable from "no user data".
    const auto stored = reinterpret_cast<uintptr_t>(lv_obj_get_user_data(button));
    ctx->startTrackRequest.store(static_cast<int>(stored) - 1);
}

void onPlayPauseClicked(lv_event_t* event) {
    auto* ctx = static_cast<Context*>(lv_event_get_user_data(event));
    if (ctx->decoder == nullptr && !ctx->tracks.empty()) {
        ctx->startTrackRequest.store(0);
    } else {
        ctx->togglePauseRequest.store(true);
    }
}

void onStopClicked(lv_event_t* event) {
    static_cast<Context*>(lv_event_get_user_data(event))->stopRequest.store(true);
}

void onNextClicked(lv_event_t* event) {
    static_cast<Context*>(lv_event_get_user_data(event))->skipRequest.store(1);
}

void onPreviousClicked(lv_event_t* event) {
    static_cast<Context*>(lv_event_get_user_data(event))->skipRequest.store(-1);
}

void onVolumeChanged(lv_event_t* event) {
    auto* sliderBox = static_cast<lv_obj_t*>(lv_event_get_target(event));
    service::audio::setOutputVolume(static_cast<float>(lvgl_sliderbox_get_value(sliderBox)));
}

// endregion

// region Window

lv_obj_t* createTransportButton(lv_obj_t* parent, const char* symbol, lv_event_cb_t callback, Context* ctx) {
    auto* button = lv_button_create(parent);
    lv_obj_set_flex_grow(button, 1);
    lv_obj_set_height(button, 40);
    lv_obj_add_event_cb(button, callback, LV_EVENT_SHORT_CLICKED, ctx);

    auto* label = lv_label_create(button);
    lv_label_set_text(label, symbol);
    lv_obj_center(label);
    return label;
}

void createWidgets(lv_obj_t* parent, void* userData) {
    auto* ctx = static_cast<Context*>(userData);

    lv_obj_set_flex_flow(parent, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_row(parent, 0, LV_STATE_DEFAULT);

    auto* toolbar = lvgl_toolbar_create(parent, "Media Player");
    lvgl_toolbar_set_nav_action(toolbar, LV_SYMBOL_CLOSE, onBackPressed, ctx);

    auto* header = lv_obj_create(parent);
    lv_obj_set_width(header, LV_PCT(100));
    lv_obj_set_height(header, LV_SIZE_CONTENT);
    lv_obj_set_style_pad_all(header, 6, LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(header, 0, LV_STATE_DEFAULT);
    lv_obj_set_flex_flow(header, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_row(header, 2, LV_STATE_DEFAULT);

    ctx->trackLabel = lv_label_create(header);
    lv_label_set_long_mode(ctx->trackLabel, LV_LABEL_LONG_MODE_DOTS);
    lv_obj_set_width(ctx->trackLabel, LV_PCT(100));
    lv_label_set_text(ctx->trackLabel, "-");

    ctx->statusLabel = lv_label_create(header);
    lv_obj_set_style_text_color(ctx->statusLabel, lv_palette_main(LV_PALETTE_GREY), LV_STATE_DEFAULT);
    lv_label_set_text(ctx->statusLabel, ctx->status.c_str());

    auto* transport = lv_obj_create(parent);
    lv_obj_set_width(transport, LV_PCT(100));
    lv_obj_set_height(transport, LV_SIZE_CONTENT);
    lv_obj_set_style_pad_all(transport, 4, LV_STATE_DEFAULT);
    lv_obj_set_style_pad_column(transport, 6, LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(transport, 0, LV_STATE_DEFAULT);
    lv_obj_set_flex_flow(transport, LV_FLEX_FLOW_ROW);

    createTransportButton(transport, LV_SYMBOL_PREV, onPreviousClicked, ctx);
    ctx->playPauseLabel = createTransportButton(transport, LV_SYMBOL_PAUSE, onPlayPauseClicked, ctx);
    createTransportButton(transport, LV_SYMBOL_STOP, onStopClicked, ctx);
    createTransportButton(transport, LV_SYMBOL_NEXT, onNextClicked, ctx);

    auto* volumeRow = lv_obj_create(parent);
    lv_obj_set_width(volumeRow, LV_PCT(100));
    lv_obj_set_height(volumeRow, LV_SIZE_CONTENT);
    lv_obj_set_style_pad_all(volumeRow, 6, LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(volumeRow, 0, LV_STATE_DEFAULT);

    auto* volumeLabel = lv_label_create(volumeRow);
    lv_label_set_text(volumeLabel, LV_SYMBOL_VOLUME_MAX);
    lv_obj_align(volumeLabel, LV_ALIGN_LEFT_MID, 0, 0);

    ctx->volumeSlider = lvgl_sliderbox_create(volumeRow, 0, 100, 10,
                                              static_cast<int32_t>(service::audio::getOutputVolume()));
    lv_obj_set_width(ctx->volumeSlider, LV_PCT(70));
    lv_obj_align(ctx->volumeSlider, LV_ALIGN_RIGHT_MID, 0, 0);
    lvgl_sliderbox_add_value_changed_cb(ctx->volumeSlider, onVolumeChanged, ctx);

    ctx->listWidget = lv_list_create(parent);
    lv_obj_set_width(ctx->listWidget, LV_PCT(100));
    lv_obj_set_flex_grow(ctx->listWidget, 1);

    if (ctx->tracks.empty()) {
        lv_list_add_text(ctx->listWidget, "No audio files found");
        lv_list_add_text(ctx->listWidget, "Put .mp3/.wav/.flac in /sdcard");
    } else {
        for (size_t i = 0; i < ctx->tracks.size(); ++i) {
            auto* button = lv_list_add_button(ctx->listWidget, LV_SYMBOL_AUDIO, ctx->tracks[i].name.c_str());
            lv_obj_set_style_bg_color(button, lv_color_hex(0x303030), LV_STATE_DEFAULT);
            lv_obj_set_user_data(button, reinterpret_cast<void*>(i + 1));
            lv_obj_add_event_cb(button, onTrackClicked, LV_EVENT_SHORT_CLICKED, ctx);
            ctx->rowButtons.push_back(button);
        }
    }

    updateStatusWidgets(ctx);
}

void destroyWidgets(void* userData) {
    auto* ctx = static_cast<Context*>(userData);
    ctx->trackLabel = nullptr;
    ctx->statusLabel = nullptr;
    ctx->listWidget = nullptr;
    ctx->playPauseLabel = nullptr;
    ctx->volumeSlider = nullptr;
    ctx->rowButtons.clear();
}

// endregion

int32_t appMain(int argc, char* argv[]) {
    Context ctx {};
    ctx.appInstanceId = app_scheduler_current_app_id();

    ctx.inputBuffer = static_cast<uint8_t*>(malloc(INPUT_BUFFER_SIZE));
    ctx.outputBuffer = static_cast<uint8_t*>(malloc(OUTPUT_BUFFER_SIZE));
    ctx.outputCapacity = OUTPUT_BUFFER_SIZE;
    if (ctx.inputBuffer == nullptr || ctx.outputBuffer == nullptr) {
        LOG_E(TAG, "decode buffer allocation failed");
        free(ctx.inputBuffer);
        free(ctx.outputBuffer);
        return -1;
    }

    device_get_by_name("audio-stream0", &ctx.streamDevice);
    if (ctx.streamDevice == nullptr) {
        LOG_E(TAG, "no audio-stream0 device; playback will not start");
    }

    // Both registrations are required, and the simple one alone is not enough: it registers only the
    // container decoders (WAV, M4A, TS, OGG), while MP3 has no container and lives in the frame
    // decoder registry. Without the first call, opening an MP3 fails with "no decoder" even though
    // CONFIG_AUDIO_DECODER_MP3_SUPPORT is set.
    esp_audio_dec_register_default();
    esp_audio_simple_dec_register_default();

    collectTracks(ctx.tracks);
    LOG_I(TAG, "found %u track(s)", (unsigned)ctx.tracks.size());

    TaskEventGroup eventGroup {};
    task_event_group_construct(&eventGroup);

    AppEventSubscription subscription {};
    check(app_event_subscribe(&subscription, &eventGroup) == ERROR_NONE);

    WindowId window = window_manager_create_ext(ctx.appInstanceId, createWidgets, destroyWidgets, &ctx);

    while (!ctx.closeRequested.load()) {
        // A short wait rather than an indefinite one: this same loop feeds the decoder, and the
        // blocking audio write is what paces playback.
        task_event_group_wait_any(&eventGroup, nullptr, pdMS_TO_TICKS(20));

        AppEvent event {};
        while (app_event_poll(&subscription, &event) == ERROR_NONE) {
            if (event.type == APP_EVENT_CLOSE) {
                ctx.closeRequested.store(true);
            }
        }

        serviceRequests(&ctx);
        playLoop(&ctx);

        const TickType_t now = xTaskGetTickCount();
        if ((TickType_t)(now - ctx.lastUiUpdate) >= UI_REFRESH_INTERVAL_TICKS) {
            ctx.lastUiUpdate = now;
            if (lvgl_try_lock(pdMS_TO_TICKS(50))) {
                updateStatusWidgets(&ctx);
                lvgl_unlock();
            }
        }
    }

    stopPlayback(&ctx);
    window_manager_remove(window);
    check(app_event_unsubscribe(&subscription) == ERROR_NONE);
    task_event_group_destruct(&eventGroup);

    free(ctx.inputBuffer);
    free(ctx.outputBuffer);

    return 0;
}

extern const ::AppManifest manifest = {
    .id = "tactility.mediaplayer",
    .name = "Media Player",
    .category = APP_CATEGORY_USER,
    .location = { APP_LOCATION_MEMORY, reinterpret_cast<void*>(appMain) },
    // 24 KB for the same reason the Bluetooth app needs it: rendering rows into an LVGL list costs
    // close to a kilobyte of this task's stack per row on this board, and the decoder runs here too.
    .stack = { .depth = 6144 },
};

} // namespace tt::app::mediaplayer
