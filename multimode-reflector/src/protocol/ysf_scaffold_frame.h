#pragma once

#include "../core/media_frame.h"

#include <cstdint>
#include <cstddef>
#include <vector>

class YSFScaffoldFrame {
public:

    /*
     * Project-defined YSF scaffold frame.
     *
     * This is NOT real Yaesu YSF wire format.
     *
     * Magic:
     *   YSFD
     *
     * Purpose:
     *   - internal protocol-construction testing
     *   - MediaFrame round-trip validation
     *   - YSFFich / YSFVoiceFrame development
     *
     * Future real YSF wire format should live in a separate
     * YSFScaffoldFrame implementation.
     */

    static std::vector<uint8_t> build(
        const MediaFrame& frame);

    static bool parse(
        const uint8_t* data,
        size_t length,
        MediaFrame& frame);
};
