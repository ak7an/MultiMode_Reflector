#pragma once

#include <cstdint>
#include <cstddef>

enum class ProtocolType {
    UNKNOWN,
    DSTAR,
    DMR,
    YSF,
    NXDN,
    P25,
    M17,
    XLX
};

class ProtocolDetector {
public:
    static ProtocolType detect(const uint8_t* data,
                               size_t length);
};
