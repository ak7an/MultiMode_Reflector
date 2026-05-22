#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include <map>
#include <ctime>
#include <vector>

struct BufferedFrame {

    uint8_t sequence;

    std::vector<uint8_t> payload;

    time_t arrivalTime;
};

struct JitterResult {

    bool releaseCurrent;

    std::vector<BufferedFrame> releasedFrames;
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

    std::map<uint8_t, BufferedFrame> pending;

    time_t lastActivity;
};

class JitterBuffer {
public:

    static JitterResult observe(
        const std::string& protocol,
        uint16_t streamId,
        uint8_t sequence,
        const uint8_t* data,
        size_t length);

    static void dump();

    static void cleanup();

private:

    static std::unordered_map<
        std::string,
        JitterStats> m_stats;

    static std::string makeKey(
        const std::string& protocol,
        uint16_t streamId);

    static JitterResult processQueue(
        JitterStats& s,
        uint8_t sequence);
};
