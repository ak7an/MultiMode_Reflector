#pragma once

#include <cstdint>
#include <cstddef>
#include "protocol_definitions.h"

class ProtocolDetector {
public:
    static ProtocolType detect(const uint8_t* data,
                               size_t length);
};
