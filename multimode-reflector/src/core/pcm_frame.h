#pragma once

#include "../core/media_frame.h"

#include <cstdint>
#include <vector>

struct PCMFrame
{
    MediaProtocol sourceProtocol =
        MediaProtocol::UNKNOWN;

    MediaProtocol targetProtocol =
        MediaProtocol::UNKNOWN;

    uint16_t streamId = 0;

    uint8_t sequence = 0;

    bool endOfTransmission = false;

    int sampleRate = 8000;

    std::vector<int16_t> samples;
};
