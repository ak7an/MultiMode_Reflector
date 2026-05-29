#include "xlxd_frame_packet.h"

#include <cstring>

static constexpr const char* XLXD_FRAME_MAGIC = "XLXF";
static constexpr size_t XLXD_FRAME_MAGIC_LEN = 4;
static constexpr size_t XLXD_REFLECTOR_LEN = 6;
static constexpr size_t XLXD_FRAME_HEADER_LEN =
    XLXD_FRAME_MAGIC_LEN + XLXD_REFLECTOR_LEN + 1 + 2;

static void appendUint16(
    std::vector<uint8_t>& packet,
    uint16_t value)
{
    packet.push_back(
        static_cast<uint8_t>((value >> 8) & 0xff));
    packet.push_back(
        static_cast<uint8_t>(value & 0xff));
}

static uint16_t readUint16(
    const uint8_t* buffer)
{
    return
        (static_cast<uint16_t>(buffer[0]) << 8) |
        static_cast<uint16_t>(buffer[1]);
}

std::vector<uint8_t> XLXDFramePacket::build(
    const XLXDFrameData& data)
{
    std::vector<uint8_t> packet;

    packet.insert(
        packet.end(),
        XLXD_FRAME_MAGIC,
        XLXD_FRAME_MAGIC + XLXD_FRAME_MAGIC_LEN);

    std::string reflector =
        data.reflector;

    if (reflector.size() > XLXD_REFLECTOR_LEN)
    {
        reflector =
            reflector.substr(0, XLXD_REFLECTOR_LEN);
    }

    while (reflector.size() < XLXD_REFLECTOR_LEN)
    {
        reflector.push_back(' ');
    }

    packet.insert(
        packet.end(),
        reflector.begin(),
        reflector.end());

    packet.push_back(
        static_cast<uint8_t>(data.module));

    uint16_t payloadLength =
        static_cast<uint16_t>(
            data.payload.size());

    appendUint16(
        packet,
        payloadLength);

    packet.insert(
        packet.end(),
        data.payload.begin(),
        data.payload.end());

    return packet;
}

bool XLXDFramePacket::parse(
    const uint8_t* buffer,
    size_t length,
    XLXDFrameData& data)
{
    if (buffer == nullptr)
    {
        return false;
    }

    if (length < XLXD_FRAME_HEADER_LEN)
    {
        return false;
    }

    if (std::memcmp(
            buffer,
            XLXD_FRAME_MAGIC,
            XLXD_FRAME_MAGIC_LEN) != 0)
    {
        return false;
    }

    data.reflector =
        std::string(
            reinterpret_cast<const char*>(
                buffer + XLXD_FRAME_MAGIC_LEN),
            XLXD_REFLECTOR_LEN);

    while (!data.reflector.empty() &&
           data.reflector.back() == ' ')
    {
        data.reflector.pop_back();
    }

    data.module =
        static_cast<char>(
            buffer[XLXD_FRAME_MAGIC_LEN + XLXD_REFLECTOR_LEN]);

    uint16_t payloadLength =
        readUint16(
            buffer +
            XLXD_FRAME_MAGIC_LEN +
            XLXD_REFLECTOR_LEN +
            1);

    if (length <
        XLXD_FRAME_HEADER_LEN + payloadLength)
    {
        return false;
    }

    data.payload.assign(
        buffer + XLXD_FRAME_HEADER_LEN,
        buffer + XLXD_FRAME_HEADER_LEN + payloadLength);

    return true;
}
