#include "ysf_encoder.h"

#include "../core/logger.h"
#include <sstream>
#include <iomanip>

std::vector<uint8_t> YSFEncoder::encode(
    const MediaFrame& frame)
{
    std::vector<uint8_t> packet;

    /*
     * Synthetic YSF-like bridge packet.
     * This is NOT final Yaesu/System Fusion framing.
     * It proves protocol regeneration and transport flow.
     */

    packet.resize(40, 0);

    packet[0] = 'Y';
    packet[1] = 'S';
    packet[2] = 'F';
    packet[3] = 'B'; // bridge/test marker

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

    packet[8] = 'D';
    packet[9] = 'S';
    packet[10] = 'T';
    packet[11] = 'R';

    packet[12] =
        static_cast<uint8_t>(
            frame.payload.size() >> 8);

    packet[13] =
        static_cast<uint8_t>(
            frame.payload.size() & 0xFF);

    std::ostringstream hex;

    for (size_t i = 0;
         i < packet.size();
         ++i)
    {
        hex
            << std::hex
            << std::setw(2)
            << std::setfill('0')
            << static_cast<int>(packet[i])
            << " ";
    }

    Logger::log(INFO,
        "YSFEncoder synthetic bridge packet:"
        " STREAMID=" +
        std::to_string(frame.streamId) +
        " SEQ=" +
        std::to_string(frame.sequence) +
        " PAYLOAD=" +
        std::to_string(frame.payload.size()) +
        " LEN=" +
        std::to_string(packet.size()));

    Logger::log(INFO,
        "YSF HEX: " +
        hex.str());

    return packet;
}
