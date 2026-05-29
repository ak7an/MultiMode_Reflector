#pragma once

#include <vector>
#include <cstdint>
#include <string>

struct XLXDHandshakeData
{
    std::string reflector;
    char module = 'A';
    uint32_t protocolVersion = 1;
};

class XLXDHandshakePacket
{
public:

    static std::vector<uint8_t> build(
        const XLXDHandshakeData& data);

    static bool parse(
        const uint8_t* buffer,
        size_t length,
        XLXDHandshakeData& data);
};
