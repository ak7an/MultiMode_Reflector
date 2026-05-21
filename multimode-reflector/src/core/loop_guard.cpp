#include "loop_guard.h"

std::unordered_map<
    std::string,
    time_t>
LoopGuard::m_seen;

bool LoopGuard::seenRecently(
    uint16_t streamId,
    uint8_t sequence,
    const std::string& peer)
{
    time_t now =
        time(nullptr);

    std::string key =
        peer + ":" +
        std::to_string(streamId) +
        ":" +
        std::to_string(sequence);

    auto it =
        m_seen.find(key);

    if (it != m_seen.end()) {

        if ((now - it->second) < 2) {

            return true;
        }
    }

    m_seen[key] = now;

    return false;
}
