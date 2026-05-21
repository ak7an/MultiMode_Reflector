#include "jitter_buffer.h"

#include "logger.h"

std::unordered_map<
    std::string,
    JitterStats>
JitterBuffer::m_stats;

std::string JitterBuffer::makeKey(
    const std::string& protocol,
    uint16_t streamId)
{
    return protocol + ":" +
        std::to_string(streamId);
}

void JitterBuffer::observe(
    const std::string& protocol,
    uint16_t streamId,
    uint8_t sequence)
{
    std::string key =
        makeKey(
            protocol,
            streamId);

    JitterStats& s =
        m_stats[key];

    uint8_t cleanSeq =
        sequence & 0x1F;

    if (s.packetsObserved == 0) {

        s.protocol = protocol;
        s.streamId = streamId;
        s.packetsObserved = 0;
        s.duplicateCount = 0;
        s.jitterCount = 0;
        s.outOfOrderCount = 0;
        s.lastSequence = 0;
        s.hasSequence = false;
    }

    s.packetsObserved++;
    s.lastActivity =
        time(nullptr);

    if (!s.hasSequence) {

        s.lastSequence = cleanSeq;
        s.hasSequence = true;

        Logger::log(INFO,
            "JitterBuffer observe:"
            " PROTO=" + protocol +
            " STREAMID=" + std::to_string(streamId) +
            " SEQ=" + std::to_string(cleanSeq));

        return;
    }

    if (cleanSeq == s.lastSequence) {

        s.duplicateCount++;

        Logger::log(INFO,
            "JitterBuffer duplicate:"
            " PROTO=" + protocol +
            " STREAMID=" + std::to_string(streamId) +
            " SEQ=" + std::to_string(cleanSeq));

        return;
    }

    uint8_t expected =
        (s.lastSequence + 1) % 21;

    if (cleanSeq != expected) {

        int gap =
            (cleanSeq + 21 - expected) % 21;

        if (gap <= 2) {

            s.jitterCount++;

            Logger::log(INFO,
                "JitterBuffer jitter:"
                " PROTO=" + protocol +
                " STREAMID=" + std::to_string(streamId) +
                " EXPECTED=" + std::to_string(expected) +
                " GOT=" + std::to_string(cleanSeq) +
                " GAP=" + std::to_string(gap));
        }
        else {

            s.outOfOrderCount++;

            Logger::log(INFO,
                "JitterBuffer out-of-order:"
                " PROTO=" + protocol +
                " STREAMID=" + std::to_string(streamId) +
                " EXPECTED=" + std::to_string(expected) +
                " GOT=" + std::to_string(cleanSeq) +
                " GAP=" + std::to_string(gap));
        }
    }
    else {

        Logger::log(INFO,
            "JitterBuffer observe:"
            " PROTO=" + protocol +
            " STREAMID=" + std::to_string(streamId) +
            " SEQ=" + std::to_string(cleanSeq));
    }

    s.lastSequence = cleanSeq;
}

void JitterBuffer::dump()
{
    Logger::log(INFO,
        "===== JITTER BUFFER STATS =====");

    for (const auto& [key, s] : m_stats) {

        Logger::log(INFO,
            "JitterStats:"
            " PROTO=" + s.protocol +
            " STREAMID=" + std::to_string(s.streamId) +
            " PACKETS=" + std::to_string(s.packetsObserved) +
            " DUP=" + std::to_string(s.duplicateCount) +
            " JITTER=" + std::to_string(s.jitterCount) +
            " OOO=" + std::to_string(s.outOfOrderCount));
    }
}

void JitterBuffer::cleanup()
{
    time_t now =
        time(nullptr);

    for (auto it = m_stats.begin();
         it != m_stats.end(); )
    {
        if ((now - it->second.lastActivity) > 60) {

            Logger::log(INFO,
                "Removing jitter stats: " +
                it->first);

            it = m_stats.erase(it);
        }
        else {

            ++it;
        }
    }
}
