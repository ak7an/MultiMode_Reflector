#pragma once

#include "../core/media_frame.h"

#include <cstddef>
#include <cstdint>
#include <vector>

class YSFVoiceFrame
{
public:
    static constexpr size_t YSF_PAYLOAD_SIZE = 120;
    static constexpr size_t YSF_SLOT_COUNT = 5;
    static constexpr size_t YSF_SLOT_SIZE = 18;
    static constexpr size_t AMBE_BYTES_PER_SLOT = 9;

    static std::vector<uint8_t> build(
        const MediaFrame& frame);

    static bool parse(
        const uint8_t* data,
        size_t length,
        MediaFrame& frame);
};
