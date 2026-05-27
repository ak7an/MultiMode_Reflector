#pragma once

#include <string>
#include <vector>
#include <cstdint>

class PacketCapture {
public:

    static void record(
        const std::string& proto,
        const std::vector<uint8_t>& packet);
};
