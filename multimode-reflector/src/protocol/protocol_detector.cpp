#include "protocol_detector.h"

#include <cstring>

ProtocolType ProtocolDetector::detect(
    const uint8_t* data,
    size_t length)
{
    if (length < 4) {
        return ProtocolType::UNKNOWN;
    }

    if (std::memcmp(data, "DSVT", 4) == 0 ||
        std::memcmp(data, "DSBB", 4) == 0)
    {
        return ProtocolType::DSTAR;
    }

    if (std::memcmp(data, "YSFD", 4) == 0 ||
        std::memcmp(data, "YSFB", 4) == 0)
    {
        return ProtocolType::YSF;
    }

    if (std::memcmp(data, "DMRD", 4) == 0)
    {
        return ProtocolType::DMR;
    }

    if (std::memcmp(data, "NXDN", 4) == 0)
    {
        return ProtocolType::NXDN;
    }

    return ProtocolType::UNKNOWN;
}
