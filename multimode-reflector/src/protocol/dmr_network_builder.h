#pragma once

#include "../core/media_frame.h"

#include <vector>
#include <cstdint>

class DMRNetworkBuilder
{
public:
    static std::vector<uint8_t> build(
        const MediaFrame& frame);
};
