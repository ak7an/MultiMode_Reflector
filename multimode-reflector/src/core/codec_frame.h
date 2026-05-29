#pragma once

#include <cstdint>
#include <vector>

enum class CodecType
{
    NONE,
    AMBE,
    PCM
};

struct CodecFrame
{
    CodecType codec =
        CodecType::NONE;

    uint16_t streamId = 0;

    uint8_t sequence = 0;

    bool endOfTransmission = false;

    std::vector<uint8_t> payload;
};
