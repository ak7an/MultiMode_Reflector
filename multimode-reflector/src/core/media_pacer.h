#pragma once

#include "media_frame.h"

#include <chrono>
#include <cstdint>
#include <string>
#include <unordered_map>

class MediaPacer {
public:

    static void pace(
        const MediaFrame& frame,
        int intervalMs);

private:

    static std::unordered_map<
        std::string,
        std::chrono::steady_clock::time_point> m_nextSend;

    static std::string makeKey(
        MediaProtocol protocol,
        uint16_t streamId);
};
