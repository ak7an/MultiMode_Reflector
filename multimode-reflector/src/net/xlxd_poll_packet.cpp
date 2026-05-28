#include "xlxd_poll_packet.h"

std::vector<uint8_t> XLXDPollPacket::build()
{
    std::vector<uint8_t> packet;

    packet.push_back('X');
    packet.push_back('L');
    packet.push_back('X');
    packet.push_back('P');

    return packet;
}
