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

bool JitterBuffer::observe(
    const std::string& protocol,
    uint16_t streamId,
    uint8_t sequence)
{
    std::string key =
        makeKey(protocol,
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

        s.hasSequence = false;
        s.expectedSequence = 0;
    }

    s.packetsObserved++;

    s.lastActivity =
        time(nullptr);

    s.pending[cleanSeq] =
        time(nullptr);

    Logger::log(INFO,
        "JitterBuffer queued:"
        " PROTO=" + protocol +
        " STREAMID=" +
        std::to_string(streamId) +
        " SEQ=" +
        std::to_string(cleanSeq));

    return processQueue(
        s,
        cleanSeq);
}

bool JitterBuffer::processQueue(
    JitterStats& s,
    uint8_t sequence)
{
    if (!s.hasSequence) {

        s.expectedSequence =
            sequence;

        s.hasSequence = true;
    }

    if (!s.pending.count(
            s.expectedSequence))
    {
        Logger::log(INFO,
            "JitterBuffer holding:"
            " STREAMID=" +
            std::to_string(s.streamId) +
            " EXPECTED=" +
            std::to_string(s.expectedSequence) +
            " PENDING=" +
            std::to_string(s.pending.size()));

        return false;
    }

    Logger::log(INFO,
        "JitterBuffer release:"
        " STREAMID=" +
        std::to_string(s.streamId) +
        " SEQ=" +
        std::to_string(
            s.expectedSequence));

    s.lastSequence =
        s.expectedSequence;

    s.pending.erase(
        s.expectedSequence);

    s.expectedSequence =
        (s.expectedSequence + 1) % 21;

    return true;
}

void JitterBuffer::dump()
{
    Logger::log(INFO,
        "===== JITTER BUFFER STATS =====");

    for (const auto& [key, s] : m_stats) {

        Logger::log(INFO,
            "JitterStats:"
            " PROTO=" + s.protocol +
            " STREAMID=" +
            std::to_string(s.streamId) +
            " PACKETS=" +
            std::to_string(
                s.packetsObserved) +
            " PENDING=" +
            std::to_string(
                s.pending.size()));
    }
}

void JitterBuffer::cleanup()
{
    time_t now =
        time(nullptr);

    for (auto it =
         m_stats.begin();
         it != m_stats.end(); )
    {
        if ((now -
            it->second.lastActivity) > 60)
        {
            Logger::log(INFO,
                "Removing jitter stats: " +
                it->first);

            it =
                m_stats.erase(it);
        }
        else {

            ++it;
        }
    }
}
