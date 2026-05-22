#pragma once

#include <cstdint>
#include <string>
#include <vector>

enum class MediaProtocol {

    DSTAR,
    DMR,
    YSF,
    P25,
    NXDN,
    M17,
    UNKNOWN
};

struct MediaFrame {

    MediaProtocol protocol;

    uint16_t streamId;

    uint8_t sequence;

    bool endOfTransmission;

    std::string sourcePeer;

    std::vector<uint8_t> payload;
};
