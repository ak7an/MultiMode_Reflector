#pragma once

#include "../core/media_frame.h"
#include "ysf_fich_fields.h"

#include <array>
#include <cstddef>
#include <cstdint>

class YSFFich {
public:
    static constexpr size_t FICH_SIZE = 6;

    static YSFFichFields fields(
        const MediaFrame& frame);

    static std::array<uint8_t, FICH_SIZE> build(
        const MediaFrame& frame);

    static bool parse(
        const uint8_t* data,
        size_t length,
        MediaFrame& frame);
};
