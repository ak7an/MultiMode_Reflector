#pragma once

#include "../core/media_frame.h"

#include <vector>
#include <cstdint>

class ProtocolEncoder {
public:

    static std::vector<uint8_t> encode(
        const MediaFrame& frame);
};
