#pragma once

#include "../core/media_frame.h"

#include <cstdint>
#include <vector>

class DStarEncoder {
public:

    static std::vector<uint8_t> encode(
        const MediaFrame& frame);
};
