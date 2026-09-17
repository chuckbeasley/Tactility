// SPDX-License-Identifier: Apache-2.0
//
// Builds the radar screen's imagery: a base map with a series of radar frames on top, all rendered
// by servers for the zoom level being shown rather than magnified on the device.
//
// Two services, both open and neither needing a key:
//
//   base map   USGS National Map, "USGSTopo", fetched through its ArcGIS REST export endpoint, which
//              renders one PNG for a bounding box at whatever size is asked for. The tile endpoint
//              would be the obvious choice but it answers JPEG, and this build has no JPEG decoder.
//   radar      NWS GeoServer (the service radar.weather.gov's own page names in its initial state),
//              fetched as WMS with a bounding box and a time, which is how that page animates too.
//
// The product is a fixed 600x550 GIF covering about 420 km, so the only way to see closer is to ask
// the server for a closer view - which is what a bbox is. `conus:conus_bref_qcd` publishes 60
// two-minute time steps and snaps to the nearest, so a series of frames is a series of requests.
//
// The radar layer is transparent and drawn over the base map here rather than by either server,
// because they are different hosts. Each frame is flattened into RGB565 as it arrives, which costs
// one buffer per frame instead of an alpha channel and lets playback be a pointer swap.
#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace tt::app::radar {

/** Where and how closely to look, in web mercator's terms: a centre and a zoom level. */
struct MapView {
    double latitude = 0.0;
    double longitude = 0.0;
    int32_t zoom = 8;
    int32_t width = 480;
    int32_t height = 231;
};

/**
 * One ready-to-display frame: RGB565 pixels with the base map and the radar already combined.
 * The pixels belong to the RadarFrames that produced it, which keeps them in one block.
 */
struct MapFrame {
    const uint8_t* pixels = nullptr;
    int32_t width = 0;
    int32_t height = 0;
};

/**
 * A fetched series. All frames share one allocation, so this is the object that owns the imagery.
 */
class RadarFrames {
public:
    static constexpr int32_t FRAME_COUNT = 6;

    std::vector<MapFrame> frames;

    /** What was fetched, for the caption and the log. */
    std::string description;

    /**
     * Fetches the base map and the radar series for @a view and flattens them into RGB565 frames.
     *
     * @param[in] view where to look and how closely
     * @param[in] station the radar site being shown, used only to describe the result
     * @param[out] outError a short reason on failure, fit to show on screen
     * @return true when at least one frame was produced
     */
    bool fetch(const MapView& view, const std::string& station, std::string& outError);

    /** Drops every frame and the pixels behind them. */
    void clear();

    bool isEmpty() const { return frames.empty(); }

private:
    /** One block for every frame, so this is the only thing that owns imagery. */
    std::vector<uint8_t> pixels;
};

} // namespace tt::app::radar
