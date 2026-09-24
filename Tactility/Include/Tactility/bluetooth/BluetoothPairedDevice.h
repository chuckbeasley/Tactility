#pragma once

#include <array>
#include <cstdint>
#include <string>
#include <vector>

namespace tt::bluetooth::settings {

struct PairedDevice {
    std::string name;
    std::array<uint8_t, 6> addr;
    bool autoConnect = false;
    /** Profile used to pair (BtProfileId value). Defaults to BT_PROFILE_SPP=2. */
    int profileId = 2;
};

std::string addrToHex(const std::array<uint8_t, 6>& addr);

bool hasFileForDevice(const std::string& addr_hex);

bool load(const std::string& addr_hex, PairedDevice& device);

/**
 * Find a stored device by name and report the address it is filed under.
 *
 * This exists for peers that rotate their address and never bond. Such a peer has no identity address
 * to file it under, so a record keyed by its temporary address is a new record on every connection -
 * which is how one BLE keyboard ended up stored four times, each with autoConnect=true, and why the
 * "Paired" section filled up with rows that no longer match the device. Its name is the only stable
 * thing about it, so it is what a caller matches on to move the existing record instead of adding
 * another.
 *
 * @param[in] name the device name to match, compared exactly
 * @param[out] out_device the stored record
 * @param[out] out_addr_hex the address the record is filed under
 * @return true when a record with that name exists
 */
bool loadByName(const std::string& name, PairedDevice& out_device, std::string& out_addr_hex);

bool save(const PairedDevice& device);

bool remove(const std::string& addr_hex);

std::vector<PairedDevice> loadAll();

} // namespace tt::bluetooth::settings
