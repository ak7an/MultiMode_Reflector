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
