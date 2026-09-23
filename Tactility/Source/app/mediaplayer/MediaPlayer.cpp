// SPDX-License-Identifier: Apache-2.0
//
// Media player: MP3, WAV, FLAC and M4A/AAC files from storage, and MP3 or ADTS-AAC internet radio,
// played through the audio stream device.
//
// Why the format handling is one call: decoding is delegated to Espressif's audio simple decoder
// (espressif/esp_audio_codec), which owns both the container parsing and the codec. That matters
// twice over on this board. Its MP3 decoder is fixed-point, which this target needs - the ESP32-C5
// is built with -march=rv32imac and has no FPU, so a float-based decoder would run in software
// emulation - and its parser means this app can feed it file bytes of any size instead of having to
// find frame boundaries itself.
//
// Why playback is split across two tasks: decoding and playing cannot share one. MP3 decoding costs
// this CPU roughly 0.35 s per second of audio, and the codec's blocking write costs about another
// second, because the write returns only as the DAC drains. In a single task those serialize to
// about 1.35 s of work per second of audio - more than real time allows - and the old one-loop
// version of this app measured exactly that: 96% busy while producing half of real-time audio, with
// the DAC starved and an internet station closing the connection because the socket was left
// undrained. So the app task now reads and decodes ahead into PcmRing, and a writer task does
// nothing but copy out of that ring into the codec. That also buys jitter tolerance: a stall in the
// network is absorbed by whatever is already buffered instead of being heard.
//
// The stream's sample rate, channel count and bit depth are not known until the first frame decodes,
// so the decoder is opened first and the output stream is opened from the decoder's own report.
#include <dirent.h>

#include <atomic>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <freertos/task.h>

#include <lvgl/lvgl.h>

#include <Tactility/Tactility.h>
#include <Tactility/app/inputdialog/InputDialog.h>
#include <Tactility/service/audio/Audio.h>
#include <Tactility/service/wifi/Wifi.h>

#include <app/event.h>
#include <app/manager.h>
#include <app/manifest.h>
#include <app/paths.h>
#include <app/scheduler.h>
#include <app/stream.h>

#include <lvgl_window_manager/window_manager.h>

#include <tactility/check.h>
#include <tactility/device.h>
#include <tactility/drivers/audio_codec.h>
#include <tactility/drivers/audio_stream.h>
#include <tactility/log.h>
#include <tactility/preferences.h>

#include <lvgl/widgets/sliderbox.h>
#include <lvgl/widgets/toolbar.h>

#include <esp_audio_dec_default.h>
#include <esp_audio_simple_dec.h>
#include <esp_audio_simple_dec_default.h>
#include <esp_crt_bundle.h>
#include <esp_http_client.h>

namespace tt::app::mediaplayer {

constexpr auto* TAG = "MediaPlayer";

// Defined at the bottom of this file; referenced by the preferences path helper above it.
extern const ::AppManifest manifest;

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

// How many times a dropped stream is reconnected before the player gives up on it.
constexpr int MAX_RECONNECT_ATTEMPTS = 3;

// How long to wait between reconnect attempts. Each attempt can itself block for the socket's own
// timeout, so this is a floor rather than the real interval.
constexpr TickType_t STREAM_RETRY_TICKS = pdMS_TO_TICKS(3000);

// Decode-ahead buffer, in bytes of decoded PCM. 8 seconds at 44.1 kHz stereo: long enough to ride
// out the stalls this board's link produces, and 1.4 MB, which is nothing against the 7.9 MB of
// PSRAM it is allocated from.
constexpr size_t PCM_RING_BYTES = 176400 * 8;

// How long the writer waits before looking at the ring again. Only a poll interval: the ring's
// semaphores do the waking, and a lost wakeup costs this much latency, not correctness.
constexpr TickType_t RING_POLL_TICKS = pdMS_TO_TICKS(20);

// Free space the decoder demands before it starts a pass, and the longest it will wait mid-pass for
// room. One pass turns a full input buffer into at most about 45 KB of PCM (4 KB of 128 kbps MP3 is
// 11 frames, each under 8 KB), so 64 KB makes a short write inside a pass impossible in practice -
// it exists so that a writer stopping mid-pass cannot hang this task, and it is counted if it fires.
constexpr size_t RING_WRITE_MARGIN = 64 * 1024;
constexpr TickType_t RING_WRITE_WAIT = pdMS_TO_TICKS(500);

// How long the ring must stay empty before the writer calls it an audible dropout. Below this the
// codec's own DMA buffering covers the gap, so a shorter one is not something a listener could hear.
constexpr TickType_t GAP_MIN_TICKS = pdMS_TO_TICKS(200);

// Stack and chunk size for the writer task. It only ever copies out of the ring and calls the codec.
constexpr uint32_t WRITER_STACK_WORDS = 3072 / sizeof(StackType_t);
constexpr size_t WRITER_CHUNK_BYTES = 8192;

struct Track {
    std::string path;
    std::string name;
    esp_audio_simple_dec_type_t type = ESP_AUDIO_SIMPLE_DEC_TYPE_NONE;
};

/** Where the bytes currently being decoded come from. */
enum class Source {
    None,
    File,
    Stream,
};

// region Decode-ahead buffer

/**
 * A byte ring between the task that decodes and the task that feeds the codec. See the file header
 * for why the two are separate tasks at all.
 *
 * Single producer, single consumer. The positions are guarded by a mutex; the two semaphores are
 * bare wakeups, so a lost one costs a poll interval rather than correctness.
 */
struct PcmRing {
    uint8_t* data = nullptr;
    size_t capacity = 0;
    size_t readPosition = 0;
    size_t writePosition = 0;
    size_t used = 0;
    // Set (under the mutex) when the writer is going away, so a producer blocked on a full ring
    // cannot wait forever for room that will never appear.
    bool aborted = false;
    SemaphoreHandle_t mutex = nullptr;
    SemaphoreHandle_t dataAvailable = nullptr;
    SemaphoreHandle_t spaceAvailable = nullptr;
};

void ringDestroy(PcmRing& ring) {
    if (ring.mutex != nullptr) {
        vSemaphoreDelete(ring.mutex);
    }
    if (ring.dataAvailable != nullptr) {
        vSemaphoreDelete(ring.dataAvailable);
    }
    if (ring.spaceAvailable != nullptr) {
        vSemaphoreDelete(ring.spaceAvailable);
    }
    free(ring.data);
    ring = PcmRing {};
}

bool ringCreate(PcmRing& ring, size_t capacity) {
    // Megabytes of audio, not kilobytes of state: on this board an allocation this size lands in
    // PSRAM by itself (CONFIG_SPIRAM_USE_MALLOC with a 256-byte internal threshold).
    ring.data = static_cast<uint8_t*>(malloc(capacity));
    ring.mutex = xSemaphoreCreateMutex();
    ring.dataAvailable = xSemaphoreCreateBinary();
    ring.spaceAvailable = xSemaphoreCreateBinary();
    if (ring.data == nullptr || ring.mutex == nullptr || ring.dataAvailable == nullptr ||
        ring.spaceAvailable == nullptr) {
        ringDestroy(ring);
        return false;
    }
    ring.capacity = capacity;
    return true;
}

/** Drops everything buffered. Called on stop, on track change and on close. */
void ringFlush(PcmRing& ring) {
    if (ring.mutex == nullptr) {
        return;
    }
    xSemaphoreTake(ring.mutex, portMAX_DELAY);
    ring.readPosition = 0;
    ring.writePosition = 0;
    ring.used = 0;
    xSemaphoreGive(ring.mutex);
}

/** Marks the ring as unusable so a blocked producer gives up instead of waiting for the writer. */
void ringAbort(PcmRing& ring) {
    if (ring.mutex == nullptr) {
        return;
    }
    xSemaphoreTake(ring.mutex, portMAX_DELAY);
    ring.aborted = true;
    xSemaphoreGive(ring.mutex);
    xSemaphoreGive(ring.spaceAvailable);
}

size_t ringUsed(PcmRing& ring) {
    if (ring.mutex == nullptr) {
        return 0;
    }
    xSemaphoreTake(ring.mutex, portMAX_DELAY);
    const size_t used = ring.used;
    xSemaphoreGive(ring.mutex);
    return used;
}

size_t ringFree(PcmRing& ring) {
    if (ring.mutex == nullptr) {
        return 0;
    }
    xSemaphoreTake(ring.mutex, portMAX_DELAY);
    const size_t freeBytes = ring.capacity - ring.used;
    xSemaphoreGive(ring.mutex);
    return freeBytes;
}

/**
 * Appends PCM for the writer, waiting up to @a wait for room when the ring is full - which is the
 * backpressure that stops the decoder running away with all the memory.
 *
 * Returns the bytes stored, which is less than @a length when the ring stayed full for the whole
 * wait. Callers must not treat that as normal: the caller here keeps the ring's free space above one
 * pass's worth of output precisely so it cannot happen, and counts it if it ever does.
 */
size_t ringWrite(PcmRing& ring, const uint8_t* source, size_t length, TickType_t wait) {
    size_t written = 0;
    const TickType_t giveUpAt = xTaskGetTickCount() + wait;
    while (written < length) {
        size_t chunk = 0;
        bool aborted = false;

        xSemaphoreTake(ring.mutex, portMAX_DELAY);
        aborted = ring.aborted;
        const size_t freeBytes = ring.capacity - ring.used;
        if (!aborted && freeBytes > 0) {
            // One contiguous copy per pass: up to the end of the buffer, then wrap.
            const size_t toEnd = ring.capacity - ring.writePosition;
            chunk = length - written;
            if (chunk > freeBytes) {
                chunk = freeBytes;
            }
            if (chunk > toEnd) {
                chunk = toEnd;
            }
            memcpy(ring.data + ring.writePosition, source + written, chunk);
            ring.writePosition = (ring.writePosition + chunk) % ring.capacity;
            ring.used += chunk;
        }
        xSemaphoreGive(ring.mutex);

        if (aborted) {
            break;
        }
        if (chunk == 0) {
            // Signed difference so this survives the tick counter wrapping around.
            if ((int32_t)(xTaskGetTickCount() - giveUpAt) >= 0) {
                break;
            }
            xSemaphoreTake(ring.spaceAvailable, RING_POLL_TICKS);
            continue;
        }
        written += chunk;
        xSemaphoreGive(ring.dataAvailable);
    }
    return written;
}

/**
 * Takes up to @a length bytes for the writer. Zero means there was nothing to take; the caller
 * decides whether that is a gap or simply the end of a track.
 */
size_t ringRead(PcmRing& ring, uint8_t* destination, size_t length, TickType_t wait) {
    size_t copied = 0;
    xSemaphoreTake(ring.mutex, portMAX_DELAY);
    if (ring.used > 0) {
        const size_t toEnd = ring.capacity - ring.readPosition;
        copied = ring.used < length ? ring.used : length;
        if (copied > toEnd) {
            copied = toEnd;
        }
        memcpy(destination, ring.data + ring.readPosition, copied);
        ring.readPosition = (ring.readPosition + copied) % ring.capacity;
        ring.used -= copied;
    }
    xSemaphoreGive(ring.mutex);

    if (copied > 0) {
        xSemaphoreGive(ring.spaceAvailable);
    } else if (wait > 0) {
        xSemaphoreTake(ring.dataAvailable, wait);
    }
    return copied;
}

// endregion

struct Context {
    uint32_t appInstanceId = 0;
    // Owned by appMain; the URL dialog's result arrives through this group.
    TaskEventGroup* eventGroup = nullptr;

    // Requests from the widget callbacks. Those run on the LVGL task, so they only set flags and
    // never touch the decoder, which belongs to the app task.
    std::atomic<bool> closeRequested{false};
    std::atomic<int> startTrackRequest{-1};
    std::atomic<bool> togglePauseRequest{false};
    std::atomic<bool> stopRequest{false};
    std::atomic<int> skipRequest{0};
    std::atomic<bool> startStreamRequest{false};

    // Set while the user wants a stream playing, which is what keeps a failed connection retrying;
    // cleared by a stop or by giving up.
    bool wantStream = false;
    TickType_t lastStreamAttempt = 0;
    // Whether this app turned WiFi power save off and therefore has to turn it back on.
    bool powerSaveRestoreNeeded = false;

    std::vector<Track> tracks;
    int currentTrack = -1;
    bool paused = false;
    bool failed = false;
    std::string status = "Stopped";
    TickType_t lastUiUpdate = 0;

    // Playback state, owned by the app task alone.
    Source source = Source::None;
    FILE* file = nullptr;
    esp_http_client_handle_t http = nullptr;
    std::string streamUrl;
    // Bytes read while identifying a stream's format, still waiting to be decoded.
    size_t pendingLength = 0;
    int reconnectAttempts = 0;
    esp_audio_simple_dec_handle_t decoder = nullptr;
    Device* streamDevice = nullptr;
    AudioStreamHandle stream = nullptr;
    // The handle the writer task writes through, published once the first frame has revealed the
    // format and cleared again whenever the output is closed. The writer never touches the app
    // task's own state; this and the ring are the whole interface between them.
    std::atomic<AudioStreamHandle> outputStream{nullptr};
    uint8_t* inputBuffer = nullptr;
    uint8_t* outputBuffer = nullptr;
    size_t outputCapacity = 0;
    // Encoded bytes pulled from the current source, and PCM bytes handed to the ring. The ratio
    // between them is the stream's bitrate; the counters are in the logs deliberately.
    uint64_t inputBytes = 0;
    uint64_t decodedBytes = 0;
    uint32_t shortWrites = 0;
    uint64_t droppedBytes = 0;

    // A file's input runs out seconds before its audio does, so the advance to the next track waits
    // until the writer has played the tail out of the ring.
    bool awaitingDrain = false;

    PcmRing ring {};
    TaskHandle_t writerTask = nullptr;
    std::atomic<bool> writerStopped{false};
    std::atomic<bool> stopWriter{false};
    std::atomic<bool> outputPaused{false};
    // True from the moment a source is opened until it is known to have no more audio. The writer
    // counts a gap whenever it finds the ring empty while this is set.
    std::atomic<bool> expectingAudio{false};
    // Gaps the listener would have heard, and how much audio is buffered ahead of the DAC. The first
    // is the only honest measure of whether playback was continuous, so it is counted and shown.
    std::atomic<uint32_t> underruns{0};
    std::atomic<uint32_t> bufferedMillis{0};
    // PCM bytes per second for the current output format, so ring occupancy can be shown as time.
    uint32_t outputBytesPerSecond = 0;

    // Text-entry dialog for the stream URL.
    AppStream inputDialogStream {};
    uint8_t inputDialogBuffer[256] {};

    // Widgets
    lv_obj_t* trackLabel = nullptr;
    lv_obj_t* statusLabel = nullptr;
    lv_obj_t* listWidget = nullptr;
    lv_obj_t* playPauseLabel = nullptr;
    lv_obj_t* volumeSlider = nullptr;
    lv_obj_t* streamLabel = nullptr;
    std::vector<lv_obj_t*> rowButtons;
    // The colour each row currently has, so that refreshHighlight() only touches rows that change.
    std::vector<lv_color_t> rowColors;
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
    // AAC in an MP4 container. Raw ADTS AAC files go through the plain AAC decoder instead, which is
    // the same path an ADTS stream takes.
    if (endsWithIgnoreCase(name, ".m4a") || endsWithIgnoreCase(name, ".mp4")) return ESP_AUDIO_SIMPLE_DEC_TYPE_M4A;
    if (endsWithIgnoreCase(name, ".aac")) return ESP_AUDIO_SIMPLE_DEC_TYPE_AAC;
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

// region Stream settings

constexpr auto* PREFERENCES_FILE = "mediaplayer.properties";
constexpr auto* PREFERENCES_KEY_STREAM_URL = "streamUrl";

std::string preferencesPath() {
    char directory[192];
    if (app_paths_get_user_data_directory(manifest.id, directory, sizeof(directory)) != ERROR_NONE) {
        return "";
    }
    return std::string(directory) + "/" + PREFERENCES_FILE;
}

void loadStreamUrl(Context* ctx) {
    const std::string path = preferencesPath();
    if (path.empty()) {
        return;
    }
    Preferences* preferences = preferences_open(path.c_str());
    if (preferences == nullptr) {
        LOG_W(TAG, "cannot open %s", path.c_str());
        return;
    }
    char value[256] = {};
    // preferences_opt_string() returns error_t, not bool: ERROR_NONE (0) means the key was found
    // and copied. Testing the result directly would read success as "not found".
    const error_t error = preferences_opt_string(preferences, PREFERENCES_KEY_STREAM_URL, value, sizeof(value));
    if (error == ERROR_NONE) {
        ctx->streamUrl = value;
    }
    LOG_I(TAG, "stream URL %s", error == ERROR_NONE ? ctx->streamUrl.c_str() : "(not set)");
    preferences_close(preferences);
}

void storeStreamUrl(Context* ctx) {
    const std::string path = preferencesPath();
    if (path.empty()) {
        return;
    }
    Preferences* preferences = preferences_open(path.c_str());
    if (preferences == nullptr) {
        return;
    }
    preferences_put_string(preferences, PREFERENCES_KEY_STREAM_URL, ctx->streamUrl.c_str());
    preferences_close(preferences);
}

/**
 * Works out a stream's format from its first bytes rather than its Content-Type header.
 *
 * The header would be the obvious source, but esp_http_client only retains response headers when
 * CONFIG_ESP_HTTP_CLIENT_SAVE_RESPONSE_HEADERS is enabled, and this build has it off. Sniffing is
 * also the more reliable signal: internet radio stations are frequently mislabelled, and the frame
 * sync of the first frame is what the decoder actually has to agree with.
 */
esp_audio_simple_dec_type_t sniffStreamType(const uint8_t* data, size_t length) {
    if (length < 4) {
        return ESP_AUDIO_SIMPLE_DEC_TYPE_NONE;
    }

    // ADTS AAC is tested first because its sync word is a subset of the MPEG audio one: an ADTS
    // header (0xFF 0xF1/0xF9) also satisfies the 11-set-bit test below, whereas no valid MPEG audio
    // frame satisfies this one (it requires layer bits 00, which is a reserved layer). Testing MP3
    // first would send every AAC stream to the MP3 decoder.
    if (data[0] == 0xFF && (data[1] & 0xF6) == 0xF0) {
        return ESP_AUDIO_SIMPLE_DEC_TYPE_AAC;
    }
    // MPEG audio frame sync: 11 set bits. MP3 is what internet radio overwhelmingly uses, and it is
    // the frame format this board has actually been verified decoding.
    if (data[0] == 0xFF && (data[1] & 0xE0) == 0xE0) {
        return ESP_AUDIO_SIMPLE_DEC_TYPE_MP3;
    }
    // ID3v2 tag: skipping it is the decoder's business, but a stream that starts with one is MP3 in
    // practice.
    if (memcmp(data, "ID3", 3) == 0) {
        return ESP_AUDIO_SIMPLE_DEC_TYPE_MP3;
    }

    // Ogg is deliberately not recognised, and the OGG branch that used to be here is gone with the
    // Vorbis decoder: the codec library is built with only the formats this player offers (see the
    // audio codec block in the device's sdkconfig), so claiming a stream format here would only turn
    // into "no decoder" a moment later. An Ogg stream now reports as unrecognised instead.
    return ESP_AUDIO_SIMPLE_DEC_TYPE_NONE;
}

// endregion

// region Playback (app task only)

bool openDecoder(Context* ctx, esp_audio_simple_dec_type_t type);

void closeStream(Context* ctx) {
    // Unpublish before closing: the writer checks this first, and the audio stream module makes the
    // close itself safe against a write that is already in flight.
    ctx->outputStream.store(nullptr);
    if (ctx->stream != nullptr) {
        audio_stream_close(ctx->stream);
        ctx->stream = nullptr;
    }
}

void closeHttp(Context* ctx) {
    if (ctx->http != nullptr) {
        esp_http_client_close(ctx->http);
        esp_http_client_cleanup(ctx->http);
        ctx->http = nullptr;
    }
}

/**
 * Tears down the current source and its transport.
 *
 * @a flushRing is what separates the two callers. A stop, a track change or a close is a break in
 * what the listener is hearing, so whatever is buffered goes. Reconnecting a live stream is not: the
 * audio in the ring is still the audio that should be playing, and keeping it is the whole reason a
 * station's dropouts are inaudible.
 */
void stopTransport(Context* ctx, bool flushRing) {
    if (flushRing) {
        ringFlush(ctx->ring);
        ctx->expectingAudio.store(false);
        ctx->bufferedMillis.store(0);
        ctx->awaitingDrain = false;
    }

    closeStream(ctx);
    closeHttp(ctx);
    if (ctx->decoder != nullptr) {
        esp_audio_simple_dec_close(ctx->decoder);
        ctx->decoder = nullptr;
    }
    if (ctx->file != nullptr) {
        fclose(ctx->file);
        ctx->file = nullptr;
    }
    ctx->source = Source::None;
    ctx->pendingLength = 0;
    if (flushRing) {
        ctx->decodedBytes = 0;
        ctx->inputBytes = 0;
        ctx->shortWrites = 0;
    }
}

void stopPlayback(Context* ctx) {
    stopTransport(ctx, true);
    ctx->wantStream = false;

    // Whatever this app did to the radio, it undoes here - the same store-and-restore discipline the
    // Radar app uses, so a user's own power-save preference survives.
    if (ctx->powerSaveRestoreNeeded) {
        tt::service::wifi::setPowerSaveEnabled(true);
        ctx->powerSaveRestoreNeeded = false;
    }
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

    if (!openDecoder(ctx, track.type)) {
        fclose(ctx->file);
        ctx->file = nullptr;
        return false;
    }
    ctx->currentTrack = index;
    ctx->paused = false;
    ctx->outputPaused.store(false);
    ctx->source = Source::File;
    ctx->expectingAudio.store(true);
    ctx->status = "Playing";
    LOG_I(TAG, "opening %s as %s", track.path.c_str(), esp_audio_simple_dec_get_name(track.type));
    return true;
}

bool openDecoder(Context* ctx, esp_audio_simple_dec_type_t type) {
    esp_audio_simple_dec_cfg_t cfg = {};
    cfg.dec_type = type;
    cfg.dec_cfg = nullptr;
    cfg.cfg_size = 0;
    // Container parsing is the point: the data is fed in arbitrary chunks.
    cfg.use_frame_dec = false;

    const esp_audio_err_t result = esp_audio_simple_dec_open(&cfg, &ctx->decoder);
    if (result != ESP_AUDIO_ERR_OK) {
        ctx->status = std::string("No decoder for ") + esp_audio_simple_dec_get_name(type);
        LOG_W(TAG, "decoder open failed for %s: %d", esp_audio_simple_dec_get_name(type), (int)result);
        ctx->failed = true;
        return false;
    }
    return true;
}

/**
 * Opens an HTTP(S) stream. The first bytes are read here, both to identify the format and because
 * the decoder has to exist before any of them can be fed to it.
 *
 * @a keepBuffered is set when this is a reconnect to the same live stream: the transport is replaced,
 * the decoded audio already in the ring is not.
 */
bool openStream(Context* ctx, const std::string& url, bool keepBuffered) {
    stopTransport(ctx, !keepBuffered);
    ctx->failed = false;

    if (url.empty()) {
        ctx->status = "No stream URL set";
        ctx->failed = true;
        return false;
    }
    ctx->streamUrl = url;

    esp_http_client_config_t config = {};
    config.url = ctx->streamUrl.c_str();
    config.method = HTTP_METHOD_GET;
    // Long enough to survive the gaps between radio bursts, short enough to give up on a dead link.
    config.timeout_ms = 10000;
    config.buffer_size = 4096;
    // HTTPS is verified against the CA bundle, which in turn needs a valid clock: without NTP the
    // certificate dates cannot be checked and an https:// stream will fail here.
    config.crt_bundle_attach = esp_crt_bundle_attach;

    ctx->http = esp_http_client_init(&config);
    if (ctx->http == nullptr) {
        ctx->status = "Cannot create HTTP client";
        ctx->failed = true;
        return false;
    }

    if (esp_http_client_open(ctx->http, 0) != ESP_OK) {
        ctx->status = "Cannot connect";
        ctx->failed = true;
        closeHttp(ctx);
        return false;
    }
    if (esp_http_client_fetch_headers(ctx->http) < 0) {
        ctx->status = "No response headers";
        ctx->failed = true;
        closeHttp(ctx);
        return false;
    }

    const int httpStatus = esp_http_client_get_status_code(ctx->http);
    if (httpStatus < 200 || httpStatus >= 300) {
        ctx->status = std::string("Server said ") + std::to_string(httpStatus);
        ctx->failed = true;
        closeHttp(ctx);
        return false;
    }

    const int initialRead = esp_http_client_read(ctx->http, reinterpret_cast<char*>(ctx->inputBuffer),
                                                 static_cast<int>(INPUT_BUFFER_SIZE));
    if (initialRead <= 0) {
        ctx->status = "Stream produced no data";
        ctx->failed = true;
        closeHttp(ctx);
        return false;
    }

    const auto type = sniffStreamType(ctx->inputBuffer, static_cast<size_t>(initialRead));
    if (type == ESP_AUDIO_SIMPLE_DEC_TYPE_NONE) {
        ctx->status = "Unrecognised stream format";
        LOG_W(TAG, "unrecognised stream, first bytes %02x %02x %02x %02x", ctx->inputBuffer[0],
              ctx->inputBuffer[1], ctx->inputBuffer[2], ctx->inputBuffer[3]);
        ctx->failed = true;
        closeHttp(ctx);
        return false;
    }

    if (!openDecoder(ctx, type)) {
        closeHttp(ctx);
        return false;
    }

    ctx->pendingLength = static_cast<size_t>(initialRead);
    ctx->source = Source::Stream;
    ctx->currentTrack = -1;
    ctx->paused = false;
    ctx->outputPaused.store(false);
    ctx->expectingAudio.store(true);
    ctx->wantStream = true;
    ctx->status = std::string("Streaming ") + esp_audio_simple_dec_get_name(type);

    // A stream is a latency problem in a way a file is not: with power save on, this board's station
    // link measured a 662 ms average round trip with 3.2 s spikes (a LAN peer answers in 2-10 ms),
    // and TCP throughput collapses with it - the same reason the web server's mirror session turns
    // power save off while it is being watched. Only done for streams, and only if it was on.
    if (tt::service::wifi::isPowerSaveEnabled()) {
        tt::service::wifi::setPowerSaveEnabled(false);
        ctx->powerSaveRestoreNeeded = true;
        LOG_I(TAG, "radio kept awake for streaming");
    }

    LOG_I(TAG, "streaming %s as %s", ctx->streamUrl.c_str(), esp_audio_simple_dec_get_name(type));
    return true;
}

/** Reads the next chunk of input from whichever source is active. 0 means the source has ended. */
size_t fillInput(Context* ctx) {
    if (ctx->pendingLength > 0) {
        const size_t length = ctx->pendingLength;
        ctx->pendingLength = 0;
        return length;
    }

    if (ctx->http != nullptr) {
        const int result = esp_http_client_read(ctx->http, reinterpret_cast<char*>(ctx->inputBuffer),
                                                static_cast<int>(INPUT_BUFFER_SIZE));
        if (result <= 0 && result != -ESP_ERR_HTTP_EAGAIN) {
            // Worth a line each time: it is the only explanation for a reconnect, and a read that
            // returns 0 means the far end closed while -1 means this end gave up waiting.
            LOG_I(TAG, "stream read returned %d after %llu bytes in", result,
                  (unsigned long long)ctx->inputBytes);
        }
        return result > 0 ? static_cast<size_t>(result) : 0;
    }

    if (ctx->file != nullptr) {
        return fread(ctx->inputBuffer, 1, INPUT_BUFFER_SIZE, ctx->file);
    }

    return 0;
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

    // Published last: the writer picks this up and starts draining the ring.
    ctx->outputStream.store(ctx->stream);
    // Bytes of PCM per second, from the format the decoder reported: what turns ring occupancy into
    // a time. Not assumed to be 44.1 kHz stereo.
    ctx->outputBytesPerSecond = (uint32_t)info.sample_rate * (bits / 8) * config.channels;
    LOG_I(TAG, "output opened: %u Hz, %u bit, %u channel(s)", (unsigned)info.sample_rate, (unsigned)bits,
          (unsigned)config.channels);
    return true;
}

/** Decodes one buffer of input into the ring. False when the source has produced its last byte. */
bool pumpOnce(Context* ctx) {
    // Backpressure before decoding rather than during: the decoder writes into one reusable output
    // buffer, so a hand-over that fails halfway would lose audio. Waiting out here also keeps this
    // task free to answer the UI, which is what makes pause work while the ring is full.
    if (ringFree(ctx->ring) < RING_WRITE_MARGIN) {
        return true;
    }

    const size_t readLength = fillInput(ctx);
    const bool endOfInput = readLength == 0;
    ctx->inputBytes += readLength;

    uint32_t offset = 0;
    bool produced = false;

    while (true) {
        esp_audio_simple_dec_raw_t raw = {};
        raw.buffer = ctx->inputBuffer + offset;
        raw.len = static_cast<uint32_t>(readLength) - offset;
        raw.eos = endOfInput;

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
            LOG_W(TAG, "decode error %d after %llu bytes in", (int)result, (unsigned long long)ctx->inputBytes);
            ctx->status = "Decode error";
            ctx->failed = true;
            return false;
        }

        if (out.decoded_size > 0) {
            produced = true;
            if (!ensureStreamOpen(ctx)) {
                return false;
            }
            // This can only wait for as long as RING_WRITE_WAIT, and only if the writer stopped
            // mid-pass: the ring is checked for room before a pass starts, and that margin covers
            // everything one pass can produce. Anything short here is audio that had to be dropped,
            // so it is counted and said out loud rather than shrugged off.
            const size_t stored = ringWrite(ctx->ring, ctx->outputBuffer, out.decoded_size, RING_WRITE_WAIT);
            if (stored != out.decoded_size) {
                const size_t lost = out.decoded_size - stored;
                if (ctx->droppedBytes == 0) {
                    LOG_W(TAG, "decode-ahead buffer full; dropping PCM");
                }
                ctx->droppedBytes += lost;
            }
            ctx->decodedBytes += stored;
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

    // At end of input the loop above has already asked the decoder to flush, so producing nothing
    // here means the source really is finished.
    return !(endOfInput && !produced);
}

// endregion

// region Writer task

/**
 * Drains the ring into the codec, and does nothing else.
 *
 * This is the half of playback that must never be late: the codec's write returns as the DAC drains,
 * so a task that did anything else between writes would starve the output. It reads the handle
 * through Context::outputStream because the output is opened, closed and reopened by the app task as
 * tracks change; a write error therefore means "the output changed underneath us", not a failure,
 * and the chunk is dropped rather than ending playback.
 */
void writerMain(void* parameter) {
    auto* ctx = static_cast<Context*>(parameter);
    auto* chunk = static_cast<uint8_t*>(malloc(WRITER_CHUNK_BYTES));
    if (chunk == nullptr) {
        LOG_E(TAG, "writer: no memory for its buffer");
        ctx->writerStopped.store(true);
        vTaskDelete(nullptr);
        return;
    }

    // A gap is only counted once the ring has stayed empty for longer than the codec's own
    // buffering can cover, and then only once per stretch of silence. Anything shorter than that is
    // inaudible, and counting it would make a healthy player look broken: passes through this loop
    // are milliseconds apart, so the ring is empty between writes all the time.
    bool inGap = false;
    bool empty = false;
    TickType_t emptySince = 0;
    bool sawAudio = false;
    uint32_t writeErrors = 0;

    while (!ctx->stopWriter.load()) {
        if (ctx->outputPaused.load()) {
            vTaskDelay(RING_POLL_TICKS);
            continue;
        }

        if (ctx->outputStream.load() == nullptr) {
            // No output: between tracks, or stopped. Nothing is late, so this is not a gap.
            sawAudio = false;
            empty = false;
            vTaskDelay(RING_POLL_TICKS);
            continue;
        }

        if (!ctx->expectingAudio.load()) {
            // The source has ended and its tail is draining out: a later gap belongs to whatever
            // comes next, so this source's history does not carry over.
            sawAudio = false;
        }

        const size_t got = ringRead(ctx->ring, chunk, WRITER_CHUNK_BYTES, RING_POLL_TICKS);
        if (got == 0) {
            const TickType_t now = xTaskGetTickCount();
            if (sawAudio && ctx->expectingAudio.load()) {
                if (!empty) {
                    empty = true;
                    emptySince = now;
                } else if (!inGap && (TickType_t)(now - emptySince) >= GAP_MIN_TICKS) {
                    inGap = true;
                    const uint32_t total = ++ctx->underruns;
                    LOG_W(TAG, "output ran dry (gap %u)", (unsigned)total);
                }
            }
            continue;
        }

        if (inGap) {
            LOG_I(TAG, "audio resumed after a %u ms gap",
                  (unsigned)((xTaskGetTickCount() - emptySince) * portTICK_PERIOD_MS));
        }
        inGap = false;
        empty = false;
        sawAudio = true;

        // audio_stream_write() reports how much the codec accepted, and it may accept less than a
        // full buffer: the driver's contract is that the out-parameter is the bytes consumed, not
        // the bytes offered. The remainder is retried rather than dropped.
        size_t offset = 0;
        while (offset < got) {
            const size_t remaining = got - offset;
            size_t written = 0;
            const error_t result = audio_stream_write(ctx->outputStream.load(), chunk + offset, remaining,
                                                      &written, pdMS_TO_TICKS(2000));
            if (result != ERROR_NONE) {
                // Highest seen count only, so a persistent error cannot flood the log.
                if (writeErrors++ < 3) {
                    LOG_W(TAG, "output write failed: %d", (int)result);
                }
                break;
            }
            if (written == 0) {
                break;
            }
            if (written < remaining) {
                ++ctx->shortWrites;
            }
            offset += written;
        }
    }

    free(chunk);
    // Releases a producer that might be waiting for room that will now never appear.
    ringAbort(ctx->ring);
    ctx->writerStopped.store(true);
    vTaskDelete(nullptr);
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
        // The writer stops taking from the ring, so the ring fills and the decoder waits on it: the
        // position in the audio survives a pause.
        ctx->outputPaused.store(ctx->paused);
        ctx->status = ctx->paused ? "Paused" : "Playing";
    }

    if (ctx->startStreamRequest.exchange(false)) {
        // A fresh request from the UI: whatever was playing is replaced, buffer included.
        ctx->wantStream = true;
        openStream(ctx, ctx->streamUrl, false);
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
    // A file's input runs out seconds before its audio does, so an advance waits here until the
    // writer has played the tail out. This is a state rather than a delay loop on purpose: the UI
    // and the event loop keep running while it drains.
    if (ctx->awaitingDrain) {
        if (ringUsed(ctx->ring) != 0) {
            return;
        }
        ctx->awaitingDrain = false;
        const int next = ctx->currentTrack + 1;
        if (next < static_cast<int>(ctx->tracks.size())) {
            openTrack(ctx, next);
        } else {
            stopPlayback(ctx);
            ctx->status = "Finished";
            ctx->currentTrack = -1;
        }
        return;
    }

    if (ctx->paused) {
        return;
    }

    // Reconnecting is driven from here rather than from the failure that caused it. A failed attempt
    // leaves no decoder behind, and an earlier version of this function returned as soon as there was
    // no decoder - so one failure to reconnect meant the player sat there for good, which is exactly
    // what a station's dropouts used to do on this board.
    if (ctx->decoder == nullptr) {
        if (!ctx->wantStream || ctx->streamUrl.empty()) {
            return;
        }
        if ((int32_t)(xTaskGetTickCount() - ctx->lastStreamAttempt) < (int32_t)STREAM_RETRY_TICKS) {
            return;
        }
        ctx->lastStreamAttempt = xTaskGetTickCount();
        if (ctx->reconnectAttempts >= MAX_RECONNECT_ATTEMPTS) {
            stopPlayback(ctx);
            ctx->wantStream = false;
            ctx->status = "Stream ended";
            return;
        }
        ++ctx->reconnectAttempts;
        ctx->status = std::string("Reconnecting (") + std::to_string(ctx->reconnectAttempts) + "/" +
                      std::to_string(MAX_RECONNECT_ATTEMPTS) + ")";
        LOG_I(TAG, "stream reconnect attempt %d", ctx->reconnectAttempts);
        // keepBuffered: what is in the ring is still audio worth playing while this is retried.
        openStream(ctx, ctx->streamUrl, true);
        return;
    }

    if (pumpOnce(ctx)) {
        // Data arrived, so whatever was attempted last is working: let a later drop retry afresh.
        ctx->reconnectAttempts = 0;
        return;
    }

    if (ctx->failed) {
        // A decode or output failure: stop rather than marching through every remaining track.
        stopPlayback(ctx);
        ctx->currentTrack = -1;
        ctx->failed = false;
        return;
    }

    // A stream ending is usually the station or the link dropping us, not the end of anything, so it
    // is worth retrying. The transport goes, the buffered audio stays, and the retry itself happens
    // at the top of this function. Files simply move on to the next track.
    if (ctx->source == Source::Stream) {
        LOG_I(TAG, "stream ended after %llu bytes in and %llu out",
              (unsigned long long)ctx->inputBytes, (unsigned long long)ctx->decodedBytes);
        stopTransport(ctx, false);
        return;
    }

    // The file is exhausted but the ring is not: wait for the writer rather than cutting the tail off.
    ctx->expectingAudio.store(false);
    ctx->awaitingDrain = true;
}

// endregion

// region Widgets

/**
 * Sets a label's text only when it differs from what is already there.
 *
 * This matters far more than it looks. lv_label_set_text() reallocates and invalidates the object
 * whether or not the text changed, and this app calls it on a timer to keep the status line fresh.
 * A needless invalidation redraws through the display's SPI bus, which the SD card shares: measured
 * on this board, the file read rate during playback fell from 339 KB/s (idle) to 16 KB/s because
 * every 200 ms pass was invalidating labels whose text had not changed. The starved reads left the
 * audio device underrunning, and for a stream they left the socket undrained, which is what made
 * the station time the connection out.
 */
void setLabelTextIfChanged(lv_obj_t* label, const char* text) {
    const char* current = lv_label_get_text(label);
    if (current == nullptr || strcmp(current, text) != 0) {
        lv_label_set_text(label, text);
    }
}

void refreshHighlight(Context* ctx) {
    // Same reasoning as setLabelTextIfChanged(): restyling a row that already has the wanted colour
    // would redraw it for nothing.
    for (size_t i = 0; i < ctx->rowButtons.size(); ++i) {
        const lv_color_t wanted = static_cast<int>(i) == ctx->currentTrack
            ? lv_palette_main(LV_PALETTE_BLUE)
            : lv_color_hex(0x303030);
        if (!lv_color_eq(ctx->rowColors[i], wanted)) {
            ctx->rowColors[i] = wanted;
            lv_obj_set_style_bg_color(ctx->rowButtons[i], wanted, LV_STATE_DEFAULT);
        }
    }
}

void updateStatusWidgets(Context* ctx) {
    if (ctx->statusLabel == nullptr) {
        return;
    }

    const char* name = ctx->currentTrack >= 0 ? ctx->tracks[ctx->currentTrack].name.c_str() : "-";
    setLabelTextIfChanged(ctx->trackLabel, name);

    // The status line carries the two numbers that say whether playback is actually healthy: how much
    // audio is buffered ahead of the DAC, and how many times it has run dry. Without them a starved
    // output looks exactly like a working one from the outside.
    std::string status = ctx->status;
    if (ctx->source != Source::None) {
        // Integer tenths rather than %f: float formatting in printf is not something this build
        // depends on being enabled.
        const uint32_t millis = ctx->bufferedMillis.load();
        char suffix[64];
        snprintf(suffix, sizeof(suffix), "  (%u.%us buffered", (unsigned)(millis / 1000),
                 (unsigned)((millis % 1000) / 100));
        status += suffix;
        const uint32_t gaps = ctx->underruns.load();
        if (gaps > 0) {
            snprintf(suffix, sizeof(suffix), ", %u gap%s)", (unsigned)gaps, gaps == 1 ? "" : "s");
            status += suffix;
        } else {
            status += ")";
        }
    }
    setLabelTextIfChanged(ctx->statusLabel, status.c_str());

    setLabelTextIfChanged(ctx->playPauseLabel, ctx->paused ? LV_SYMBOL_PLAY : LV_SYMBOL_PAUSE);
    if (ctx->streamLabel != nullptr) {
        setLabelTextIfChanged(ctx->streamLabel,
                              ctx->streamUrl.empty() ? "No stream URL set" : ctx->streamUrl.c_str());
    }
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

void openUrlDialog(Context* ctx) {
    inputdialog::start(ctx->appInstanceId, "Stream URL", "http:// or https://", ctx->streamUrl,
                       ctx->inputDialogStream, ctx->inputDialogBuffer, sizeof(ctx->inputDialogBuffer),
                       ctx->eventGroup);
}

void onStreamClicked(lv_event_t* event) {
    auto* ctx = static_cast<Context*>(lv_event_get_user_data(event));
    if (ctx->streamUrl.empty()) {
        // Nothing configured yet, so ask instead of failing with nothing on screen to explain why.
        openUrlDialog(ctx);
    } else {
        ctx->startStreamRequest.store(true);
    }
}

void onSetStreamUrlClicked(lv_event_t* event) {
    openUrlDialog(static_cast<Context*>(lv_event_get_user_data(event)));
}

/** Reads what the URL dialog returned. Runs on the app task, from appMain's event loop. */
void onDialogResult(Context* ctx, int32_t result) {
    if (result == 0) {
        char buffer[sizeof(ctx->inputDialogBuffer)];
        const size_t length = app_stream_read(&ctx->inputDialogStream, buffer, sizeof(buffer));
        if (length > 0) {
            ctx->streamUrl.assign(buffer, length);
            // The dialog's textarea can leave a trailing newline in what it returns.
            while (!ctx->streamUrl.empty() &&
                   (ctx->streamUrl.back() == '\n' || ctx->streamUrl.back() == '\r')) {
                ctx->streamUrl.pop_back();
            }
            storeStreamUrl(ctx);
            LOG_I(TAG, "stream URL set to %s", ctx->streamUrl.c_str());
        }
    }
    app_stream_unsubscribe(&ctx->inputDialogStream);
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

    // Stream row: the URL that "Stream" would open, a button to play it, and one to change it.
    auto* streamRow = lv_obj_create(parent);
    lv_obj_set_width(streamRow, LV_PCT(100));
    lv_obj_set_height(streamRow, LV_SIZE_CONTENT);
    lv_obj_set_style_pad_all(streamRow, 6, LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(streamRow, 0, LV_STATE_DEFAULT);
    lv_obj_set_flex_flow(streamRow, LV_FLEX_FLOW_ROW);
    lv_obj_set_style_pad_column(streamRow, 6, LV_STATE_DEFAULT);

    auto* streamButton = lv_button_create(streamRow);
    lv_obj_set_flex_grow(streamButton, 1);
    lv_obj_set_height(streamButton, 36);
    lv_obj_add_event_cb(streamButton, onStreamClicked, LV_EVENT_SHORT_CLICKED, ctx);

    auto* streamButtonLabel = lv_label_create(streamButton);
    lv_label_set_text(streamButtonLabel, LV_SYMBOL_WIFI " Stream");
    lv_obj_center(streamButtonLabel);

    auto* setUrlButton = lv_button_create(streamRow);
    lv_obj_set_height(setUrlButton, 36);
    lv_obj_add_event_cb(setUrlButton, onSetStreamUrlClicked, LV_EVENT_SHORT_CLICKED, ctx);

    auto* setUrlLabel = lv_label_create(setUrlButton);
    lv_label_set_text(setUrlLabel, LV_SYMBOL_EDIT);
    lv_obj_center(setUrlLabel);

    ctx->streamLabel = lv_label_create(parent);
    lv_label_set_long_mode(ctx->streamLabel, LV_LABEL_LONG_MODE_DOTS);
    lv_obj_set_width(ctx->streamLabel, LV_PCT(100));
    lv_obj_set_style_text_color(ctx->streamLabel, lv_palette_main(LV_PALETTE_GREY), LV_STATE_DEFAULT);

    ctx->listWidget = lv_list_create(parent);
    lv_obj_set_width(ctx->listWidget, LV_PCT(100));
    lv_obj_set_flex_grow(ctx->listWidget, 1);
    if (ctx->tracks.empty()) {
        lv_list_add_text(ctx->listWidget, "No audio files found");
        lv_list_add_text(ctx->listWidget, "Put .mp3/.wav/.flac/.m4a in /sdcard");
    } else {
        for (size_t i = 0; i < ctx->tracks.size(); ++i) {
            auto* button = lv_list_add_button(ctx->listWidget, LV_SYMBOL_AUDIO, ctx->tracks[i].name.c_str());
            lv_obj_set_style_bg_color(button, lv_color_hex(0x303030), LV_STATE_DEFAULT);
            lv_obj_set_user_data(button, reinterpret_cast<void*>(i + 1));
            lv_obj_add_event_cb(button, onTrackClicked, LV_EVENT_SHORT_CLICKED, ctx);
            ctx->rowButtons.push_back(button);
            // Matches what was just applied above, so the first refresh does not restyle every row.
            ctx->rowColors.push_back(lv_color_hex(0x303030));
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
    ctx->streamLabel = nullptr;
    ctx->rowButtons.clear();
    ctx->rowColors.clear();
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

    if (!ringCreate(ctx.ring, PCM_RING_BYTES)) {
        LOG_E(TAG, "decode-ahead buffer allocation failed");
        free(ctx.inputBuffer);
        free(ctx.outputBuffer);
        return -1;
    }

    // The writer runs for the app's lifetime and is started before any source exists: it simply has
    // nothing to do until the first frame opens the output. Above the app task's own priority,
    // because being late is the one thing it must never be.
    UBaseType_t writerPriority = uxTaskPriorityGet(nullptr) + 1;
    if (writerPriority >= configMAX_PRIORITIES) {
        writerPriority = configMAX_PRIORITIES - 1;
    }
    if (xTaskCreate(writerMain, "media_writer", WRITER_STACK_WORDS, &ctx, writerPriority, &ctx.writerTask) != pdPASS) {
        LOG_E(TAG, "could not create the writer task");
        ringDestroy(ctx.ring);
        free(ctx.inputBuffer);
        free(ctx.outputBuffer);
        return -1;
    }

    collectTracks(ctx.tracks);
    loadStreamUrl(&ctx);
    LOG_I(TAG, "found %u track(s)", (unsigned)ctx.tracks.size());

    TaskEventGroup eventGroup {};
    task_event_group_construct(&eventGroup);
    ctx.eventGroup = &eventGroup;

    AppEventSubscription subscription {};
    check(app_event_subscribe(&subscription, &eventGroup) == ERROR_NONE);

    WindowId window = window_manager_create_ext(ctx.appInstanceId, createWidgets, destroyWidgets, &ctx);

    while (!ctx.closeRequested.load()) {
        // A short wait rather than an indefinite one: this loop decodes ahead into the ring, and it
        // is the ring filling up - not the codec - that paces it.
        task_event_group_wait_any(&eventGroup, nullptr, pdMS_TO_TICKS(20));

        AppEvent event {};
        while (app_event_poll(&subscription, &event) == ERROR_NONE) {
            switch (event.type) {
                case APP_EVENT_CLOSE:
                    ctx.closeRequested.store(true);
                    break;
                case APP_EVENT_RESULT:
                    onDialogResult(&ctx, event.result.result);
                    app_manager_stop(event.result.launch_id);
                    break;
                default:
                    break;
            }
        }

        serviceRequests(&ctx);
        playLoop(&ctx);

        const TickType_t now = xTaskGetTickCount();
        if ((TickType_t)(now - ctx.lastUiUpdate) >= UI_REFRESH_INTERVAL_TICKS) {
            ctx.lastUiUpdate = now;
            // Read by the status line; the writer updates it as it drains. Equal to the buffer's
            // capacity in time when the decoder is ahead, which is its normal state.
            ctx.bufferedMillis.store(ctx.outputBytesPerSecond != 0
                ? (uint32_t)((uint64_t)ringUsed(ctx.ring) * 1000 / ctx.outputBytesPerSecond)
                : 0);

            if (lvgl_try_lock(pdMS_TO_TICKS(50))) {
                updateStatusWidgets(&ctx);
                lvgl_unlock();
            }
        }
    }

    // Close the output first, then stop the writer: closing waits for a write in flight, and stopping
    // the writer first would leave it blocked on a handle that no longer exists.
    stopPlayback(&ctx);
    ctx.stopWriter.store(true);
    for (int i = 0; i < 100 && !ctx.writerStopped.load(); ++i) {
        vTaskDelay(pdMS_TO_TICKS(20));
    }
    if (!ctx.writerStopped.load()) {
        LOG_W(TAG, "writer task did not stop in time");
    }

    window_manager_remove(window);
    check(app_event_unsubscribe(&subscription) == ERROR_NONE);
    task_event_group_destruct(&eventGroup);

    ringDestroy(ctx.ring);
    free(ctx.inputBuffer);
    free(ctx.outputBuffer);

    return 0;
}

extern const ::AppManifest manifest = {
    .id = "tactility.mediaplayer",
    .name = "Media Player",
    .category = APP_CATEGORY_USER,
    .location = { APP_LOCATION_MEMORY, reinterpret_cast<void*>(appMain) },
    // 32 KB: rendering rows into an LVGL list costs close to a kilobyte of this task's stack per row
    // on this board (measured on the Bluetooth app, which crashed from exactly that), and an https://
    // stream adds a TLS handshake on top. Decoding now happens in this task while the writer task
    // plays, but neither the list nor the TLS peak grows because of that - the writer has its own
    // 3 KB stack.
    .stack = { .depth = 8192 },
};

} // namespace tt::app::mediaplayer
