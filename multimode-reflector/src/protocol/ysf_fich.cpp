#include "ysf_fich.h"

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
        MediaFrameType type)
    {
        switch (type) {
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
}

YSFFichFields
YSFFich::fields(
    const MediaFrame& frame)
{
    YSFFichFields f;

    f.frameInformation =
        frameInformationFor(
            frame.frameType);

    f.communicationType = CS_DIGITAL;
    f.callMode = CM_OPEN;
    f.dataType = DT_VOICE;

    /*
     * Scaffold frame numbering.
     *
     * These are now explicit YSF FICH concepts.
     * The exact Yaesu bit layout and protected encoding will be added later.
     */

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
    std::array<uint8_t, FICH_SIZE> fich{};

    const YSFFichFields f =
        fields(frame);

    /*
     * Temporary structured scaffold packing.
     *
     * This replaces the previous loose six-byte synthetic FICH.
     *
     * Current layout:
     *
     * byte 0: FI / CS / CM
     * byte 1: BN / BT
     * byte 2: FN / FT
     * byte 3: DT / MR / VOIP / DEV
     * byte 4: SQL / SQ
     * byte 5: EOT marker
     *
     * Future layout:
     *
     * - real YSF FICH bit packing
     * - CRC
     * - convolutional encoding
     * - interleaving
     */

    fich[0] =
        static_cast<uint8_t>(
            ((f.frameInformation & 0x03) << 6) |
            ((f.communicationType & 0x03) << 4) |
            ((f.callMode & 0x0F)));

    fich[1] =
        static_cast<uint8_t>(
            ((f.blockNumber & 0x0F) << 4) |
            ((f.blockTotal & 0x0F)));

    fich[2] =
        static_cast<uint8_t>(
            ((f.frameNumber & 0x0F) << 4) |
            ((f.frameTotal & 0x0F)));

    fich[3] =
        static_cast<uint8_t>(
            ((f.dataType & 0x03) << 6) |
            ((f.mr & 0x03) << 4) |
            ((f.voip & 0x01) << 3) |
            ((f.dev & 0x07)));

    fich[4] =
        static_cast<uint8_t>(
            ((f.sql & 0x0F) << 4) |
            ((f.sq & 0x0F)));

    fich[5] =
        frame.endOfTransmission ? 1 : 0;

    Logger::log(INFO,
        "YSFFich build:"
        " STREAMID=" +
        std::to_string(frame.streamId) +
        fieldSummary(f));

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
        data[5] != 0;

    Logger::log(INFO,
        "YSFFich parse:"
        " STREAMID=" +
        std::to_string(frame.streamId) +
        fieldSummary(f));

    return true;
}
