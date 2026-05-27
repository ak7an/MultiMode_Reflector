#pragma once

#include "../core/media_frame.h"

#include <array>
#include <cstdint>

class YSFFich {
public:
    static std::array<uint8_t, 6> build(
        const MediaFrame& frame);
};
