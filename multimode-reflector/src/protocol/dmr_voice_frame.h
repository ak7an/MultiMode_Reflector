#pragma once

#include "../core/media_frame.h"

#include <cstddef>
#include <cstdint>
#include <vector>

class DMRVoiceFrame
{
public:
    static std::vector<uint8_t> build(
        const MediaFrame& frame);

    static bool parse(
        const uint8_t* data,
        size_t length,
        MediaFrame& frame);
};
