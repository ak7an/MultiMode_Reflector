#pragma once

#include "../core/media_frame.h"

#include <vector>
#include <cstdint>

class YSFEncoder {
public:

    static std::vector<uint8_t> encode(
        const MediaFrame& frame);
};
