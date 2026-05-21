#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include <ctime>

struct JitterStats {

    std::string protocol;

    uint16_t streamId;

    uint32_t packetsObserved;
    uint32_t duplicateCount;
    uint32_t jitterCount;
    uint32_t outOfOrderCount;

    uint8_t lastSequence;
    bool hasSequence;

    time_t lastActivity;
};

class JitterBuffer {
public:

    static void observe(
        const std::string& protocol,
        uint16_t streamId,
        uint8_t sequence);

    static void dump();

    static void cleanup();

private:

    static std::unordered_map<
        std::string,
        JitterStats> m_stats;

    static std::string makeKey(
        const std::string& protocol,
        uint16_t streamId);
};
