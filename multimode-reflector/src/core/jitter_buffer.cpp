#include "jitter_buffer.h"

#include "logger.h"

std::unordered_map<
    std::string,
    JitterStats>
JitterBuffer::m_stats;

static std::string protocolToString(
    MediaProtocol protocol)
{
    switch (protocol) {

    case MediaProtocol::DSTAR:
        return "DSTAR";

    case MediaProtocol::DMR:
        return "DMR";

    case MediaProtocol::YSF:
        return "YSF";

    case MediaProtocol::P25:
        return "P25";

    case MediaProtocol::NXDN:
        return "NXDN";

    case MediaProtocol::M17:
        return "M17";

    default:
        return "UNKNOWN";
    }
}

std::string JitterBuffer::makeKey(
    MediaProtocol protocol,
    uint16_t streamId)
{
    return protocolToString(protocol) + ":" +
        std::to_string(streamId);
}

JitterResult JitterBuffer::observe(
    const MediaFrame& frame)
{
    std::string protocolName =
        protocolToString(frame.protocol);

    std::string key =
        makeKey(
            frame.protocol,
            frame.streamId);

    JitterStats& s =
        m_stats[key];

    uint8_t cleanSeq =
        frame.sequence & 0x1F;

    if (s.packetsObserved == 0) {

        s.protocol = protocolName;
        s.streamId = frame.streamId;

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

    MediaFrame queuedFrame =
        frame;

    queuedFrame.sequence =
        cleanSeq;

    s.pending[cleanSeq] =
        queuedFrame;

    Logger::log(INFO,
        "JitterBuffer queued:"
        " PROTO=" + protocolName +
        " STREAMID=" +
        std::to_string(frame.streamId) +
        " SEQ=" +
        std::to_string(cleanSeq));

    return processQueue(
        s,
        cleanSeq);
}

JitterResult JitterBuffer::processQueue(
    JitterStats& s,
    uint8_t sequence)
{
    JitterResult result{};

    result.releaseCurrent = false;

    if (!s.hasSequence) {

        s.expectedSequence =
            sequence;

        s.hasSequence = true;
    }

    while (s.pending.count(
        s.expectedSequence))
    {
        uint8_t releasedSeq =
            s.expectedSequence;

        MediaFrame frame =
            s.pending[releasedSeq];

        Logger::log(INFO,
            "JitterBuffer release:"
            " STREAMID=" +
            std::to_string(s.streamId) +
            " SEQ=" +
            std::to_string(releasedSeq));

        if (releasedSeq ==
            (sequence & 0x1F))
        {
            result.releaseCurrent = true;
        }
        else {

            result.releasedFrames.push_back(
                frame);
        }

        s.lastSequence =
            releasedSeq;

        s.pending.erase(
            releasedSeq);

        s.expectedSequence =
            (s.expectedSequence + 1) % 21;
    }

    if (!result.releaseCurrent &&
        result.releasedFrames.empty())
    {
        Logger::log(INFO,
            "JitterBuffer holding:"
            " STREAMID=" +
            std::to_string(s.streamId) +
            " EXPECTED=" +
            std::to_string(s.expectedSequence) +
            " PENDING=" +
            std::to_string(s.pending.size()));
    }

    return result;
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
