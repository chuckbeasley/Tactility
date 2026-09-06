#include "PcapWriter.h"

#include <cstring>

namespace tt::app::wifimonitor {

namespace {

// PCAP global header constants (little-endian on ESP32, which is RISC-V LE).
constexpr size_t RADIOTAP_HEADER_LEN = 16;

void writeLe16(uint8_t* out, uint16_t value) {
    out[0] = static_cast<uint8_t>(value & 0xff);
    out[1] = static_cast<uint8_t>((value >> 8) & 0xff);
}

void writeLe32(uint8_t* out, uint32_t value) {
    out[0] = static_cast<uint8_t>(value & 0xff);
    out[1] = static_cast<uint8_t>((value >> 8) & 0xff);
    out[2] = static_cast<uint8_t>((value >> 16) & 0xff);
    out[3] = static_cast<uint8_t>((value >> 24) & 0xff);
}

uint16_t channelToFrequency(uint8_t channel) {
    if (channel == 0) {
        return 2412; // default to channel 1
    }
    return channel <= 14 ? static_cast<uint16_t>(2407 + 5 * channel)
                         : static_cast<uint16_t>(5000 + 5 * channel);
}

uint16_t channelToFlags(uint8_t channel) {
    // 0x0080 = 2 GHz spectrum, 0x0010 = 5 GHz spectrum.
    return channel <= 14 ? 0x0080 : 0x0010;
}

} // namespace

PcapWriter::~PcapWriter() {
    close();
}

bool PcapWriter::open(const char* path) {
    close();
    file_ = fopen(path, "wb");
    if (file_ == nullptr) {
        return false;
    }

    // Buffer the stream in 16 KB chunks: the default small stdio buffer forces a
    // flash write every few packets, and FATFS/NOR flash writes are far faster in
    // large batches. This lets the capture writer keep up and cuts drops.
    setvbuf(file_, nullptr, _IOFBF, 16 * 1024);

    // Global header: magic 0xa1b2c3d4 (LE, microsecond), version 2.4, snaplen
    // 65535, link type 127 (IEEE802_11_RADIOTAP).
    uint8_t header[24] = {};
    header[0] = 0xd4; header[1] = 0xc3; header[2] = 0xb2; header[3] = 0xa1;
    header[4] = 0x02; header[5] = 0x00; // version major
    header[6] = 0x04; header[7] = 0x00; // version minor
    // thiszone (4) and sigfigs (4) stay zero.
    header[16] = 0xff; header[17] = 0xff; // snaplen = 65535
    header[20] = 127; // network = radiotap

    if (fwrite(header, 1, sizeof(header), file_) != sizeof(header)) {
        close();
        return false;
    }

    bytes_written_ = sizeof(header);
    packet_count_ = 0;
    return true;
}

bool PcapWriter::writePacket(uint32_t ts_sec, uint32_t ts_usec, const uint8_t* payload, size_t length, int8_t rssi, uint8_t channel) {
    if (file_ == nullptr) {
        return false;
    }

    // Radiotap header: Flags + Channel + dBm antenna signal.
    uint8_t radiotap[RADIOTAP_HEADER_LEN] = {};
    radiotap[0] = 0; // version
    radiotap[1] = 0; // pad
    writeLe16(radiotap + 2, RADIOTAP_HEADER_LEN);
    writeLe32(radiotap + 4, (1u << 1) | (1u << 3) | (1u << 5)); // present: flags|channel|dbm
    // Flags: set "FCS at end" (0x10). The ESP32 promiscuous callback reports
    // sig_len *including* the 4-byte 802.11 FCS and the payload buffer contains
    // it, so Wireshark must be told to strip it, otherwise every frame parses
    // 4 bytes too long and shows as malformed/truncated.
    radiotap[8] = 0x10;
    radiotap[9] = 0; // pad to align the 2-byte channel field
    writeLe16(radiotap + 10, channelToFrequency(channel));
    writeLe16(radiotap + 12, channelToFlags(channel));
    radiotap[14] = static_cast<uint8_t>(rssi); // dBm antenna signal (signed)
    radiotap[15] = 0; // pad

    const size_t captured_len = RADIOTAP_HEADER_LEN + length;

    uint8_t pcap_header[16] = {};
    writeLe32(pcap_header + 0, ts_sec);
    writeLe32(pcap_header + 4, ts_usec);
    writeLe32(pcap_header + 8, static_cast<uint32_t>(captured_len)); // incl_len
    writeLe32(pcap_header + 12, static_cast<uint32_t>(captured_len)); // orig_len

    if (fwrite(pcap_header, 1, sizeof(pcap_header), file_) != sizeof(pcap_header)) {
        return false;
    }
    if (fwrite(radiotap, 1, sizeof(radiotap), file_) != sizeof(radiotap)) {
        return false;
    }
    if (length > 0 && fwrite(payload, 1, length, file_) != length) {
        return false;
    }

    bytes_written_ += sizeof(pcap_header) + sizeof(radiotap) + length;
    packet_count_++;

    // Flush periodically so captured data isn't lost to a crash / power cut.
    // Kept far enough apart that it doesn't add flash-write pressure beyond the
    // 16 KB stdio buffer's own auto-flush (which already batches nearby packets).
    if (packet_count_ % 250 == 0) {
        fflush(file_);
    }
    return true;
}

void PcapWriter::close() {
    if (file_ != nullptr) {
        fflush(file_);
        fclose(file_);
        file_ = nullptr;
    }
}

} // namespace tt::app::wifimonitor
