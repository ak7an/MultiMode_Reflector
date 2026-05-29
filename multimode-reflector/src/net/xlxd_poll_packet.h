#pragma once

#include <vector>
#include <cstdint>
#include <string>

struct XLXDPollData
{
    std::string reflector;
    char module;
};

class XLXDPollPacket
{
public:

    static std::vector<uint8_t> build(
        const XLXDPollData& data);

    static bool parse(
        const uint8_t* buffer,
        size_t length,
        XLXDPollData& data);
};
