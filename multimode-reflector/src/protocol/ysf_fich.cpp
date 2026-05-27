#include "ysf_fich.h"

std::array<uint8_t, 6> YSFFich::build(
    const MediaFrame& frame)
{
    std::array<uint8_t, 6> fich{};

    fich[0] = 0x01; // placeholder: data type
    fich[1] = 0x00; // placeholder
    fich[2] = 0x02; // placeholder: voice/full-rate marker
    fich[3] = 0x00; // placeholder
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
