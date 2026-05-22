#pragma once

#include "media_frame.h"

#include <cstdint>
#include <string>
#include <unordered_map>
#include <map>
#include <ctime>
#include <vector>

struct JitterResult {

    bool releaseCurrent;

    std::vector<MediaFrame> releasedFrames;
};

struct JitterStats {

    std::string protocol;

    uint16_t streamId;

    uint32_t packetsObserved;
    uint32_t duplicateCount;
    uint32_t jitterCount;
    uint32_t outOfOrderCount;

    uint8_t lastSequence;
    bool hasSequence;

    uint8_t expectedSequence;

    std::map<uint8_t, MediaFrame> pending;

    time_t lastActivity;
};

class JitterBuffer {
public:

    static JitterResult observe(
        const MediaFrame& frame);

    static void dump();

    static void cleanup();

private:

    static std::unordered_map<
        std::string,
        JitterStats> m_stats;

    static std::string makeKey(
        MediaProtocol protocol,
        uint16_t streamId);

    static JitterResult processQueue(
        JitterStats& s,
        uint8_t sequence);
};
