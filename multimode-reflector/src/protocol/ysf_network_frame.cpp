#include "ysf_network_frame.h"
#include "ysf_fich.h"

#include <algorithm>
#include <cstring>

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

static std::string padded(
    const std::string& s,
    size_t len)
{
    std::string out = s;

    if (out.size() > len)
        out.resize(len);

    while (out.size() < len)
        out += ' ';

    return out;
}

std::vector<uint8_t>
YSFNetworkFrame::build(
    const MediaFrame& frame)
{
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

    packet[8] =
        frameTypeCode(
            frame.frameType);

    const std::string src =
        padded(
            frame.sourceCallsign.empty()
                ? "UNKNOWN"
                : frame.sourceCallsign,
            10);

    const std::string dst =
        padded(
            "ALL",
            10);

    std::memcpy(
        &packet[9],
        src.data(),
        10);

    std::memcpy(
        &packet[19],
        dst.data(),
        10);

    auto fich =
        YSFFich::build(
            frame);

    std::memcpy(
        &packet[29],
        fich.data(),
        fich.size());

    const size_t payloadOffset = 35;

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

bool YSFNetworkFrame::parse(
    const uint8_t* data,
    size_t length,
    MediaFrame& frame)
{
    if (length < 35) {
        return false;
    }

    if (std::memcmp(data, "YSFD", 4) != 0) {
        return false;
    }

    frame.protocol =
        MediaProtocol::YSF;

    frame.streamId =
        (data[4] << 8) |
         data[5];

    frame.sequence =
        data[6];

    frame.endOfTransmission =
        data[7] != 0;

    switch (data[8]) {

    case 1:
        frame.frameType =
            MediaFrameType::HEADER;
        break;

    case 2:
        frame.frameType =
            MediaFrameType::VOICE;
        break;

    case 3:
        frame.frameType =
            MediaFrameType::VOICE_EOT;
        break;

    case 4:
        frame.frameType =
            MediaFrameType::CONTROL;
        break;

    default:
        frame.frameType =
            MediaFrameType::UNKNOWN;
        break;
    }

    frame.sourceCallsign =
        std::string(
            reinterpret_cast<
                const char*>(&data[9]),
            10);

    frame.payload.assign(
        data + 35,
        data + length);

    return true;
}
