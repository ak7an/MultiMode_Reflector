#pragma once

#include <string>
#include <unordered_map>
#include <ctime>
#include <cstdint>

class LoopGuard {
public:

    static bool seenRecently(
        uint16_t streamId,
        const std::string& peer);

private:

    static std::unordered_map<
        std::string,
        time_t> m_seen;
};
