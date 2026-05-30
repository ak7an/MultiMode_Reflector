#include "ysf_encoder.h"

#include "ysf_scaffold_frame.h"
#include "../core/logger.h"

#include <sstream>
#include <iomanip>

static std::string ysfFrameMode = "synthetic";

static uint8_t syntheticFrameTypeCode(
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

    packet.resize(
        15 + frame.payload.size(),
        0);

    packet[0] = 'Y';
    packet[1] = 'S';
    packet[2] = 'F';
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
        syntheticFrameTypeCode(
            frame.frameType);

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

    return packet;
}

std::vector<uint8_t> YSFEncoder::encodeNetwork(
    const MediaFrame& frame)
{
    std::vector<uint8_t> packet =
        YSFScaffoldFrame::build(
            frame);

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
