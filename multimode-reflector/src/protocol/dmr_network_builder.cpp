#include "dmr_network_builder.h"

#include <cstring>

std::vector<uint8_t>
DMRNetworkBuilder::build(
    const MediaFrame& frame)
{
    std::vector<uint8_t> packet(
        55,
        0);

    packet[0] = 'D';
    packet[1] = 'M';
    packet[2] = 'R';
    packet[3] = 'D';

    packet[4] =
        static_cast<uint8_t>(
            frame.streamId >> 8);

    packet[5] =
        static_cast<uint8_t>(
            frame.streamId & 0xFF);

    packet[6] =
        frame.sequence;

    const size_t payloadOffset = 20;

    const size_t copyLength =
        std::min(
            frame.payload.size(),
            packet.size() - payloadOffset);

    std::memcpy(
        &packet[payloadOffset],
        frame.payload.data(),
        copyLength);

    return packet;
}
