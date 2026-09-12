#pragma once

#include <cstddef>
#include <cstdint>
#include <cstdio>

namespace tt::app::wifimonitor {

/**
 * Writes captured 802.11 frames to a standard PCAP file with a per-packet
 * radiotap header (link type IEEE802_11_RADIOTAP), so Wireshark sees the
 * channel/frequency and RSSI for each frame.
 */
class PcapWriter {
public:
    ~PcapWriter();

    /** Open the file and write the PCAP global header. Returns true on success. */
    bool open(const char* path);

    /**
     * Append one captured frame.
     * @param[in] ts_sec  seconds portion of the capture timestamp
     * @param[in] ts_usec microseconds portion of the capture timestamp
     */
    bool writePacket(uint32_t ts_sec, uint32_t ts_usec, const uint8_t* payload, size_t length, int8_t rssi, uint8_t channel);

    /** Flush and close the file. */
    void close();

    bool isOpen() const { return file_ != nullptr; }
    /** False if the stream is running unbuffered, which makes writing pathologically slow. */
    bool hasWriteBuffer() const { return write_buffer_ != nullptr; }
    size_t getBytesWritten() const { return bytes_written_; }
    size_t getPacketCount() const { return packet_count_; }

private:
    static constexpr size_t WRITE_BUFFER_SIZE = 64 * 1024;

    FILE* file_ = nullptr;
    // Owned here rather than left to stdio. setvbuf with a null buffer allocates from internal RAM,
    // which is scarce on this target, and if that allocation fails stdio silently falls back to
    // unbuffered writes - so each packet's three fwrite calls reach FatFS and the wear-levelling
    // layer individually. That is the difference between a capture draining in milliseconds and
    // taking over a minute for under a megabyte.
    char* write_buffer_ = nullptr;
    size_t bytes_written_ = 0;
    size_t packet_count_ = 0;
};

} // namespace tt::app::wifimonitor
