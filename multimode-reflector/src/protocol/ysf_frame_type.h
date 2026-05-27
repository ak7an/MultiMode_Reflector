#pragma once

#include <cstdint>

enum class YSFFrameType : uint8_t {

    UNKNOWN = 0,

    HEADER = 1,

    VOICE = 2,

    VOICE_EOT = 3,

    CONTROL = 4
};
