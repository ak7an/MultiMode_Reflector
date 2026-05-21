#pragma once

#include <unordered_map>
#include <string>
#include <ctime>
#include <cstdint>

class LoopGuard {
public:

    static bool seenRecently(
        uint16_t streamId,
        uint8_t sequence,
        const std::string& peer);

private:

    static std::unordered_map<
        std::string,
        time_t> m_seen;
};
