#include "ysf_encoder.h"

#include "../core/logger.h"

#include <cstring>

std::vector<uint8_t> YSFEncoder::encode(
    const MediaFrame& frame)
{
    std::vector<uint8_t> packet;

    packet.resize(32, 0);

    /*
     * Synthetic YSF marker for pipeline testing.
     * Real YSF framing comes later.
     */

    packet[0] = 'Y';
    packet[1] = 'S';
    packet[2] = 'F';
    packet[3] = '0';

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

    Logger::log(INFO,
        "YSFEncoder synthetic packet:"
        " STREAMID=" +
        std::to_string(frame.streamId) +
        " SEQ=" +
        std::to_string(frame.sequence) +
        " LEN=" +
        std::to_string(packet.size()));

    return packet;
}
