#pragma once

#include <string>
#include <cstdint>

struct DStarHeader {

    std::string mycall;
    std::string urcall;
    std::string rpt1;
    std::string rpt2;

    uint16_t streamId;

    bool valid;
};

class DStarHeaderParser {
public:
    static DStarHeader parse(
        const uint8_t* data,
        size_t length);
};
