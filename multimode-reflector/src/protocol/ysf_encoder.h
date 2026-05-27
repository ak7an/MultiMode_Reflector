#pragma once

#include "../core/media_frame.h"

#include <vector>
#include <cstdint>
#include <string>

class YSFEncoder {
public:

    static void setFrameMode(
        const std::string& mode);

    static std::vector<uint8_t> encode(
        const MediaFrame& frame);

private:

    static std::vector<uint8_t> encodeSynthetic(
        const MediaFrame& frame);

    static std::vector<uint8_t> encodeNetwork(
        const MediaFrame& frame);
};
