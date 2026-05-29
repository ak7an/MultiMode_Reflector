#pragma once

#include <cstdint>
#include <string>
#include <vector>

class HexDump
{
public:

    static std::string toHex(
        const std::vector<uint8_t>& data);
};
