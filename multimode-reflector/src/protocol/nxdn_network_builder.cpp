#include "nxdn_network_builder.h"

#include "../core/logger.h"

#include <algorithm>
#include <cstring>

namespace
{
    constexpr size_t NXDN_PACKET_SIZE = 55;
    constexpr size_t NXDN_PAYLOAD_OFFSET = 20;
    constexpr size_t NXDN_PAYLOAD_SIZE =
        NXDN_PACKET_SIZE - NXDN_PAYLOAD_OFFSET;
}

std::vector<uint8_t>
NXDNNetworkBuilder::build(
    const MediaFrame& frame)
{
    std::vector<uint8_t> packet(
        NXDN_PACKET_SIZE,
        0);

    packet[0] = 'N';
    packet[1] = 'X';
    packet[2] = 'D';
    packet[3] = 'N';

    packet[4] =
        static_cast<uint8_t>(
            frame.streamId >> 8);

    packet[5] =
        static_cast<uint8_t>(
            frame.streamId & 0xFF);

    packet[6] =
        frame.sequence;

    packet[7] =
        frame.endOfTransmission ? 1 : 0;

    packet[8] =
        static_cast<uint8_t>(
            NXDN_PAYLOAD_SIZE >> 8);

    packet[9] =
        static_cast<uint8_t>(
            NXDN_PAYLOAD_SIZE & 0xFF);

    const size_t copyLength =
        std::min(
            frame.payload.size(),
            NXDN_PAYLOAD_SIZE);

    if (copyLength > 0)
    {
        std::memcpy(
            &packet[NXDN_PAYLOAD_OFFSET],
            frame.payload.data(),
            copyLength);
    }

    Logger::log(INFO,
        "NXDNNetworkBuilder build:"
        " STREAMID=" +
        std::to_string(frame.streamId) +
        " INPUT_PAYLOAD=" +
        std::to_string(frame.payload.size()) +
        " NXDN_PAYLOAD=" +
        std::to_string(NXDN_PAYLOAD_SIZE) +
        " LEN=" +
        std::to_string(packet.size()));

    return packet;
}
