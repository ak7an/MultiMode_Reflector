#pragma once

#include "../core/media_frame.h"

#include <cstdint>
#include <vector>

class NXDNNetworkBuilder
{
public:
    static std::vector<uint8_t> build(
        const MediaFrame& frame);
};
