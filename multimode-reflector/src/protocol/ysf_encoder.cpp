#include "ysf_encoder.h"

#include "../core/logger.h"

#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cstring>

static std::string ysfFrameMode = "synthetic";

static uint8_t frameTypeCode(
    MediaFrameType type)
{
    switch (type) {

    case MediaFrameType::HEADER:
        return 1;

    case MediaFrameType::VOICE:
        return 2;

    case MediaFrameType::VOICE_EOT:
        return 3;

    case MediaFrameType::CONTROL:
        return 4;

    default:
        return 0;
    }
}

void YSFEncoder::setFrameMode(
    const std::string& mode)
{
    if (mode == "network") {
        ysfFrameMode = "network";
    }
    else {
        ysfFrameMode = "synthetic";
    }

    Logger::log(INFO,
        "YSFEncoder frame mode: " +
        ysfFrameMode);
}

std::vector<uint8_t> YSFEncoder::encode(
    const MediaFrame& frame)
{
    if (ysfFrameMode == "network") {
        return encodeNetwork(frame);
    }

    return encodeSynthetic(frame);
}

std::vector<uint8_t> YSFEncoder::encodeSynthetic(
    const MediaFrame& frame)
{
    std::vector<uint8_t> packet;

    /*
     * Synthetic YSF-like bridge packet.
     * This is NOT final Yaesu/System Fusion framing.
     * It proves protocol regeneration and transport flow.
     */

    packet.resize(
        15 + frame.payload.size(),
        0);

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

    packet[14] =
        frameTypeCode(
            frame.frameType);

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

std::vector<uint8_t> YSFEncoder::encodeNetwork(
    const MediaFrame& frame)
{
    /*
     * Minimal YSF network-sized packet.
     *
     * This intentionally does NOT claim to be complete
     * real Yaesu/System Fusion voice framing yet.
     *
     * Purpose:
     * - move output size/layout toward real YSF network packets
     * - preserve MediaFrame abstraction
     * - preserve synthetic mode for regression tests
     */

    std::vector<uint8_t> packet(
        155,
        0);

    packet[0] = 'Y';
    packet[1] = 'S';
    packet[2] = 'F';
    packet[3] = 'D';

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

    packet[8] = frameTypeCode(
        frame.frameType);

    /*
     * Minimal YSF network structure.
     *
     * Layout (temporary):
     *
     * 0-3   : YSFD
     * 4-5   : stream id
     * 6     : sequence
     * 7     : eot
     * 8     : frame type
     * 9-18  : source callsign
     * 19-28 : destination
     * 29-34 : reserved/FICH placeholder
     * 35+   : payload
     */

    const std::string src =
        frame.sourceCallsign.empty()
            ? "UNKNOWN"
            : frame.sourceCallsign;

    const std::string dst =
        "ALL";

    std::memcpy(
        &packet[9],
        src.c_str(),
        std::min<size_t>(10, src.size()));

    std::memcpy(
        &packet[19],
        dst.c_str(),
        std::min<size_t>(10, dst.size()));

    const size_t payloadOffset = 35;

    const size_t maxCopy =
        std::min(
            frame.payload.size(),
            packet.size() - payloadOffset);

    std::copy(
        frame.payload.begin(),
        frame.payload.begin() + maxCopy,
        packet.begin() + payloadOffset);

    Logger::log(INFO,
        "YSFEncoder network packet:"
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
