#include "dstar_network_frame.h"

#include <cstring>

std::vector<uint8_t>
DStarNetworkFrame::build(
    const MediaFrame& frame)
{
    std::vector<uint8_t> packet(
        27 + frame.payload.size(),
        0);

    packet[0] = 'D';
    packet[1] = 'S';
    packet[2] = 'V';
    packet[3] = 'T';

    packet[12] =
        static_cast<uint8_t>(
            frame.streamId >> 8);

    packet[13] =
        static_cast<uint8_t>(
            frame.streamId & 0xFF);

    packet[14] =
        frame.sequence;

    if (!frame.payload.empty()) {

        std::memcpy(
            &packet[27],
            frame.payload.data(),
            frame.payload.size());
    }

    return packet;
}

bool DStarNetworkFrame::parse(
    const uint8_t* data,
    size_t length,
    MediaFrame& frame)
{
    if (length < 27) {
        return false;
    }

    if (std::memcmp(data, "DSVT", 4) != 0) {
        return false;
    }

    frame.protocol =
        MediaProtocol::DSTAR;

    frame.streamId =
        (data[12] << 8) |
         data[13];

    frame.sequence =
        data[14];

    frame.frameType =
        MediaFrameType::VOICE;

    if (length > 27) {

        frame.payload.assign(
            data + 27,
            data + length);
    }

    return true;
}
