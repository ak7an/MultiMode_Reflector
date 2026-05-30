#include "ysf_fich.h"

#include "protocol_bit_buffer.h"
#include "protocol_crc.h"

#include "../core/logger.h"

#include <sstream>

namespace
{
    constexpr uint8_t FI_HEADER = 0x00;
    constexpr uint8_t FI_VOICE  = 0x01;
    constexpr uint8_t FI_EOT    = 0x02;

    constexpr uint8_t CS_DIGITAL = 0x00;
    constexpr uint8_t CM_OPEN    = 0x00;
    constexpr uint8_t DT_VOICE   = 0x02;

    uint8_t frameInformationFor(
        const MediaFrame& frame)
    {
        if (frame.endOfTransmission ||
            frame.frameType == MediaFrameType::VOICE_EOT)
        {
            return FI_EOT;
        }

        switch (frame.frameType) {
        case MediaFrameType::HEADER:
            return FI_HEADER;

        case MediaFrameType::VOICE:
            return FI_VOICE;

        case MediaFrameType::VOICE_EOT:
            return FI_EOT;

        default:
            return FI_VOICE;
        }
    }

    std::string fieldSummary(
        const YSFFichFields& f)
    {
        std::ostringstream ss;

        ss << " FI=" << static_cast<int>(f.frameInformation)
           << " CS=" << static_cast<int>(f.communicationType)
           << " CM=" << static_cast<int>(f.callMode)
           << " BN=" << static_cast<int>(f.blockNumber)
           << " BT=" << static_cast<int>(f.blockTotal)
           << " FN=" << static_cast<int>(f.frameNumber)
           << " FT=" << static_cast<int>(f.frameTotal)
           << " DT=" << static_cast<int>(f.dataType)
           << " MR=" << static_cast<int>(f.mr)
           << " VOIP=" << static_cast<int>(f.voip)
           << " DEV=" << static_cast<int>(f.dev)
           << " SQL=" << static_cast<int>(f.sql)
           << " SQ=" << static_cast<int>(f.sq);

        return ss.str();
    }

    std::array<uint8_t, YSFFich::FICH_SIZE> packScaffold(
        const YSFFichFields& f)
    {
        std::array<uint8_t, YSFFich::FICH_SIZE> fich{};

        ProtocolBitBuffer bits;

        bits.pushBits(f.frameInformation & 0x03, 2);
        bits.pushBits(f.communicationType & 0x03, 2);
        bits.pushBits(f.callMode & 0x0F, 4);

        bits.pushBits(f.blockNumber & 0x0F, 4);
        bits.pushBits(f.blockTotal & 0x0F, 4);

        bits.pushBits(f.frameNumber & 0x0F, 4);
        bits.pushBits(f.frameTotal & 0x0F, 4);

        bits.pushBits(f.dataType & 0x03, 2);
        bits.pushBits(f.mr & 0x03, 2);
        bits.pushBits(f.voip & 0x01, 1);
        bits.pushBits(f.dev & 0x07, 3);

        bits.pushBits(f.sql & 0x0F, 4);
        bits.pushBits(f.sq & 0x0F, 4);

        const auto& packed =
            bits.bytes();

        for (size_t i = 0;
             i < packed.size() && i < 5;
             ++i)
        {
            fich[i] = packed[i];
        }

        fich[5] =
            ProtocolCRC::crc8(
                fich.data(),
                5);

        return fich;
    }
}

YSFFichFields
YSFFich::fields(
    const MediaFrame& frame)
{
    YSFFichFields f;

    f.frameInformation =
        frameInformationFor(
            frame);

    f.communicationType = CS_DIGITAL;
    f.callMode = CM_OPEN;
    f.dataType = DT_VOICE;

    f.blockNumber =
        static_cast<uint8_t>(
            frame.sequence & 0x03);

    f.blockTotal = 0x03;

    f.frameNumber =
        static_cast<uint8_t>(
            frame.sequence & 0x07);

    f.frameTotal = 0x07;

    f.mr = 0x00;
    f.voip = 0x00;
    f.dev = 0x00;
    f.sql = 0x00;
    f.sq = 0x00;

    return f;
}

std::array<uint8_t, YSFFich::FICH_SIZE>
YSFFich::build(
    const MediaFrame& frame)
{
    const YSFFichFields f =
        fields(frame);

    auto fich =
        packScaffold(f);

    Logger::log(INFO,
        "YSFFich build:"
        " STREAMID=" +
        std::to_string(frame.streamId) +
        fieldSummary(f) +
        " CRC8=" +
        std::to_string(fich[5]));

    return fich;
}

bool YSFFich::parse(
    const uint8_t* data,
    size_t length,
    MediaFrame& frame)
{
    if (data == nullptr)
        return false;

    if (length < FICH_SIZE)
        return false;

    const uint8_t expectedCrc =
        ProtocolCRC::crc8(
            data,
            5);

    const bool crcValid =
        expectedCrc == data[5];

    YSFFichFields f;

    f.frameInformation =
        static_cast<uint8_t>(
            (data[0] >> 6) & 0x03);

    f.communicationType =
        static_cast<uint8_t>(
            (data[0] >> 4) & 0x03);

    f.callMode =
        static_cast<uint8_t>(
            data[0] & 0x0F);

    f.blockNumber =
        static_cast<uint8_t>(
            (data[1] >> 4) & 0x0F);

    f.blockTotal =
        static_cast<uint8_t>(
            data[1] & 0x0F);

    f.frameNumber =
        static_cast<uint8_t>(
            (data[2] >> 4) & 0x0F);

    f.frameTotal =
        static_cast<uint8_t>(
            data[2] & 0x0F);

    f.dataType =
        static_cast<uint8_t>(
            (data[3] >> 6) & 0x03);

    f.mr =
        static_cast<uint8_t>(
            (data[3] >> 4) & 0x03);

    f.voip =
        static_cast<uint8_t>(
            (data[3] >> 3) & 0x01);

    f.dev =
        static_cast<uint8_t>(
            data[3] & 0x07);

    f.sql =
        static_cast<uint8_t>(
            (data[4] >> 4) & 0x0F);

    f.sq =
        static_cast<uint8_t>(
            data[4] & 0x0F);

    frame.sequence =
        f.frameNumber;

    frame.endOfTransmission =
        f.frameInformation == FI_EOT;

    Logger::log(
        crcValid ? INFO : WARN,
        "YSFFich parse:"
        " STREAMID=" +
        std::to_string(frame.streamId) +
        fieldSummary(f) +
        " CRC8_EXPECTED=" +
        std::to_string(expectedCrc) +
        " CRC8_RECEIVED=" +
        std::to_string(data[5]) +
        " CRC_VALID=" +
        std::to_string(crcValid ? 1 : 0));

    return true;
}
