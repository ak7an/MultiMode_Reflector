#include "dstar_encoder.h"

#include "../core/logger.h"

std::vector<uint8_t> DStarEncoder::encode(
    const MediaFrame& frame)
{
    std::vector<uint8_t> packet(
        15 + frame.payload.size(),
        0);

    packet[0] = 'D';
    packet[1] = 'S';
    packet[2] = 'B';
    packet[3] = 'B';

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

    packet[8] = 'Y';
    packet[9] = 'S';
    packet[10] = 'F';
    packet[11] = 'B';

    packet[12] =
        static_cast<uint8_t>(
            (frame.payload.size() >> 8) & 0xFF);

    packet[13] =
        static_cast<uint8_t>(
            frame.payload.size() & 0xFF);

    packet[14] =
        static_cast<uint8_t>(
            frame.frameType == MediaFrameType::VOICE_EOT ? 3 : 2);

    std::copy(
        frame.payload.begin(),
        frame.payload.end(),
        packet.begin() + 15);

    Logger::log(INFO,
        "DStarEncoder synthetic bridge packet:"
        " STREAMID=" +
        std::to_string(frame.streamId) +
        " SEQ=" +
        std::to_string(frame.sequence) +
        " PAYLOAD=" +
        std::to_string(frame.payload.size()) +
        " LEN=" +
        std::to_string(packet.size()));

    return packet;
}
