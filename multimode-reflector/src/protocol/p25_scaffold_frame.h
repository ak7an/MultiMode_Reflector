#pragma once

#include "../core/media_frame.h"

#include <cstddef>
#include <cstdint>

class P25ScaffoldFrame
{
public:
    static bool parse(
        const uint8_t* data,
        size_t length,
        MediaFrame& frame);
};
