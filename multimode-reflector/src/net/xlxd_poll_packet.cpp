#include "xlxd_poll_packet.h"

#include <cstring>

static constexpr const char* XLXD_POLL_MAGIC = "XLXP";
static constexpr size_t XLXD_POLL_MAGIC_LEN = 4;
static constexpr size_t XLXD_REFLECTOR_LEN = 6;
static constexpr size_t XLXD_POLL_PACKET_LEN =
    XLXD_POLL_MAGIC_LEN + XLXD_REFLECTOR_LEN + 1;

std::vector<uint8_t> XLXDPollPacket::build(
    const XLXDPollData& data)
{
    std::vector<uint8_t> packet;

    packet.insert(
        packet.end(),
        XLXD_POLL_MAGIC,
        XLXD_POLL_MAGIC + XLXD_POLL_MAGIC_LEN);

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

    return packet;
}

bool XLXDPollPacket::parse(
    const uint8_t* buffer,
    size_t length,
    XLXDPollData& data)
{
    if (buffer == nullptr)
    {
        return false;
    }

    if (length < XLXD_POLL_PACKET_LEN)
    {
        return false;
    }

    if (std::memcmp(
            buffer,
            XLXD_POLL_MAGIC,
            XLXD_POLL_MAGIC_LEN) != 0)
    {
        return false;
    }

    data.reflector =
        std::string(
            reinterpret_cast<const char*>(
                buffer + XLXD_POLL_MAGIC_LEN),
            XLXD_REFLECTOR_LEN);

    while (!data.reflector.empty() &&
           data.reflector.back() == ' ')
    {
        data.reflector.pop_back();
    }

    data.module =
        static_cast<char>(
            buffer[XLXD_POLL_MAGIC_LEN + XLXD_REFLECTOR_LEN]);

    return true;
}
