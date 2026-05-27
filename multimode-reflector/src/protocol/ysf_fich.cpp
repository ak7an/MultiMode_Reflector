#include "ysf_fich.h"


YSFFichFields
YSFFich::fields(
    const MediaFrame& frame)
{
    YSFFichFields f;

    switch (frame.frameType) {

    case MediaFrameType::HEADER:
        f.frameInformation = 0x00;
        break;

    case MediaFrameType::VOICE:
        f.frameInformation = 0x01;
        break;

    case MediaFrameType::VOICE_EOT:
        f.frameInformation = 0x02;
        break;

    default:
        f.frameInformation = 0x01;
        break;
    }

    f.communicationType = 0x00;

    f.dataType = 0x02;

    f.callMode = 0x00;

    return f;
}

std::array<uint8_t, 6> YSFFich::build(
    const MediaFrame& frame)
{
    std::array<uint8_t, 6> fich{};

    auto f =
        fields(frame);

    fich[0] =
        f.frameInformation;

    fich[1] =
        f.communicationType;

    fich[2] =
        f.dataType;

    fich[3] =
        f.callMode;
    fich[4] = frame.sequence;
    fich[5] = frame.endOfTransmission ? 1 : 0;

    return fich;
}

bool YSFFich::parse(
    const uint8_t* data,
    size_t length,
    MediaFrame& frame)
{
    if (length < 6) {
        return false;
    }

    /*
     * Temporary synthetic FICH parsing.
     *
     * Future:
     * - frame subtype
     * - DT/FT bits
     * - FI/CS bits
     * - path/routing
     * - real bitfield decoding
     */

    frame.sequence =
        data[4];

    frame.endOfTransmission =
        data[5] != 0;

    return true;
}
