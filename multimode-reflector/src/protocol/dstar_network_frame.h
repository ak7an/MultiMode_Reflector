#pragma once

#include "../core/media_frame.h"

#include <vector>
#include <cstdint>

class DStarNetworkFrame {
public:

    static std::vector<uint8_t> build(
        const MediaFrame& frame);

    static bool parse(
        const uint8_t* data,
        size_t length,
        MediaFrame& frame);
};
