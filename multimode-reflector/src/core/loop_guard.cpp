#include "loop_guard.h"

#include <cstdint>

std::unordered_map<
    std::string,
    time_t>
LoopGuard::m_seen;

bool LoopGuard::seenRecently(
    uint16_t streamId,
    const std::string& peer)
{
    std::string key =
        peer + ":" +
        std::to_string(streamId);

    time_t now =
        time(nullptr);

    auto it =
        m_seen.find(key);

    if (it != m_seen.end()) {

        double age =
            difftime(now,
                     it->second);

        if (age < 2) {
            return true;
        }
    }

    m_seen[key] = now;

    return false;
}
