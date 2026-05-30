#include "ysf_scaffold_frame.h"

#include "ysf_fich.h"
#include "ysf_frame_mapper.h"
#include "ysf_voice_frame.h"

#include <algorithm>
#include <cstring>
#include <string>

namespace
{
    constexpr const char* YSF_SCAFFOLD_MAGIC = "YSFD";
    constexpr size_t YSF_SCAFFOLD_PACKET_SIZE = 155;
    constexpr size_t YSF_SCAFFOLD_HEADER_SIZE = 35;
    constexpr size_t YSF_SCAFFOLD_FICH_OFFSET = 29;
    constexpr size_t YSF_SCAFFOLD_PAYLOAD_OFFSET = 35;

    std::string padded(
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
}

std::vector<uint8_t>
YSFScaffoldFrame::build(
    const MediaFrame& frame)
{
    std::vector<uint8_t> packet(
        YSF_SCAFFOLD_PACKET_SIZE,
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
        static_cast<uint8_t>(
            YSFFrameMapper::fromMedia(
                frame.frameType));

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
        &packet[YSF_SCAFFOLD_FICH_OFFSET],
        fich.data(),
        fich.size());

    std::vector<uint8_t> voicePayload =
        YSFVoiceFrame::build(
            frame);

    const size_t copyLength =
        std::min(
            voicePayload.size(),
            packet.size() - YSF_SCAFFOLD_PAYLOAD_OFFSET);

    std::memcpy(
        &packet[YSF_SCAFFOLD_PAYLOAD_OFFSET],
        voicePayload.data(),
        copyLength);

    return packet;
}

bool YSFScaffoldFrame::parse(
    const uint8_t* data,
    size_t length,
    MediaFrame& frame)
{
    if (data == nullptr)
        return false;

    if (length < YSF_SCAFFOLD_HEADER_SIZE)
        return false;

    if (std::memcmp(data, YSF_SCAFFOLD_MAGIC, 4) != 0)
        return false;

    frame.protocol =
        MediaProtocol::YSF;

    frame.streamId =
        (data[4] << 8) |
         data[5];

    frame.sequence =
        data[6];

    frame.endOfTransmission =
        data[7] != 0;

    YSFFich::parse(
        &data[YSF_SCAFFOLD_FICH_OFFSET],
        6,
        frame);

    frame.frameType =
        YSFFrameMapper::toMedia(
            static_cast<YSFFrameType>(
                data[8]));

    frame.sourceCallsign =
        std::string(
            reinterpret_cast<
                const char*>(&data[9]),
            10);

    return YSFVoiceFrame::parse(
        data + YSF_SCAFFOLD_PAYLOAD_OFFSET,
        length - YSF_SCAFFOLD_PAYLOAD_OFFSET,
        frame);
}
