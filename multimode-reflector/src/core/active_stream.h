#pragma once

#include "media_frame.h"

#include <chrono>
#include <cstdint>

class ActiveStream {
public:

    static bool accept(
        const MediaFrame& frame);

    static void end(
        MediaProtocol protocol,
        uint16_t streamId);

    static void checkTimeout(
        int idleTimeoutMs,
        int maxTxMs);

private:

    static bool m_active;
    static MediaProtocol m_protocol;
    static uint16_t m_streamId;
    static std::chrono::steady_clock::time_point m_startedAt;
    static std::chrono::steady_clock::time_point m_lastSeen;
};
