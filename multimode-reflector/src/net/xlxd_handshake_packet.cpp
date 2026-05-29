#include "xlxd_handshake_packet.h"

#include <cstring>

static constexpr const char* XLXD_HANDSHAKE_MAGIC = "XLXH";
static constexpr size_t XLXD_HANDSHAKE_MAGIC_LEN = 4;
static constexpr size_t XLXD_REFLECTOR_LEN = 6;
static constexpr size_t XLXD_HANDSHAKE_PACKET_LEN =
    XLXD_HANDSHAKE_MAGIC_LEN + XLXD_REFLECTOR_LEN + 1 + 4;

static void appendUint32(
    std::vector<uint8_t>& packet,
    uint32_t value)
{
    packet.push_back(
        static_cast<uint8_t>((value >> 24) & 0xff));
    packet.push_back(
        static_cast<uint8_t>((value >> 16) & 0xff));
    packet.push_back(
        static_cast<uint8_t>((value >> 8) & 0xff));
    packet.push_back(
        static_cast<uint8_t>(value & 0xff));
}

static uint32_t readUint32(
    const uint8_t* buffer)
{
    return
        (static_cast<uint32_t>(buffer[0]) << 24) |
        (static_cast<uint32_t>(buffer[1]) << 16) |
        (static_cast<uint32_t>(buffer[2]) << 8) |
        static_cast<uint32_t>(buffer[3]);
}

std::vector<uint8_t> XLXDHandshakePacket::build(
    const XLXDHandshakeData& data)
{
    std::vector<uint8_t> packet;

    packet.insert(
        packet.end(),
        XLXD_HANDSHAKE_MAGIC,
        XLXD_HANDSHAKE_MAGIC + XLXD_HANDSHAKE_MAGIC_LEN);

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

    appendUint32(
        packet,
        data.protocolVersion);

    return packet;
}

bool XLXDHandshakePacket::parse(
    const uint8_t* buffer,
    size_t length,
    XLXDHandshakeData& data)
{
    if (buffer == nullptr)
    {
        return false;
    }

    if (length < XLXD_HANDSHAKE_PACKET_LEN)
    {
        return false;
    }

    if (std::memcmp(
            buffer,
            XLXD_HANDSHAKE_MAGIC,
            XLXD_HANDSHAKE_MAGIC_LEN) != 0)
    {
        return false;
    }

    data.reflector =
        std::string(
            reinterpret_cast<const char*>(
                buffer + XLXD_HANDSHAKE_MAGIC_LEN),
            XLXD_REFLECTOR_LEN);

    while (!data.reflector.empty() &&
           data.reflector.back() == ' ')
    {
        data.reflector.pop_back();
    }

    data.module =
        static_cast<char>(
            buffer[XLXD_HANDSHAKE_MAGIC_LEN + XLXD_REFLECTOR_LEN]);

    data.protocolVersion =
        readUint32(
            buffer +
            XLXD_HANDSHAKE_MAGIC_LEN +
            XLXD_REFLECTOR_LEN +
            1);

    return true;
}
