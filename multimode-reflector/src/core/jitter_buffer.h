#pragma once

#include <cstdint>
#include <string>

class JitterBuffer {
public:
    static void observe(
        const std::string& protocol,
        uint16_t streamId,
        uint8_t sequence);
};
