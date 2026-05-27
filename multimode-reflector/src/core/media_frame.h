#pragma once

#include "media_frame_type.h"

#include <cstdint>
#include <string>
#include <vector>
#include <chrono>

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

    MediaFrameType frameType;

    uint16_t streamId;

    std::string sourceCallsign;

    uint8_t sequence;

    bool endOfTransmission;

    std::string sourcePeer;

    std::vector<uint8_t> payload;

    std::chrono::steady_clock::time_point createdAt;
};
