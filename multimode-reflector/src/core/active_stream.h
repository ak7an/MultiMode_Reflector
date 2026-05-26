#pragma once

#include "media_frame.h"

#include <chrono>
#include <cstdint>
#include <string>

struct ActiveStreamStatus {

    bool active;
    bool hangActive;

    std::string callsign;

    MediaProtocol protocol;

    uint16_t streamId;

    long txAgeMs;
    long idleAgeMs;
    long hangRemainingMs;
};

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

    static ActiveStreamStatus status();

private:

    static bool m_active;
    static MediaProtocol m_protocol;
    static uint16_t m_streamId;
    static std::string m_callsign;
    static std::chrono::steady_clock::time_point m_startedAt;
    static std::chrono::steady_clock::time_point m_lastSeen;

    static bool m_hangActive;
    static std::chrono::steady_clock::time_point m_hangUntil;
};
