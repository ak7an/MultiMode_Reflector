#pragma once

#include <vector>
#include <cstdint>
#include <string>

struct XLXDFrameData
{
    std::string reflector;
    char module = 'A';
    std::vector<uint8_t> payload;
};

class XLXDFramePacket
{
public:

    static std::vector<uint8_t> build(
        const XLXDFrameData& data);

    static bool parse(
        const uint8_t* buffer,
        size_t length,
        XLXDFrameData& data);
};
