#include "dstar_session.h"

#include "../core/logger.h"

std::unordered_map<
    uint16_t,
    DStarStream>
DStarSessionManager::m_streams;

void DStarSessionManager::createOrUpdate(
    uint16_t streamId,
    const std::string& peer,
    const std::string& mycall,
    const std::string& urcall,
    const std::string& rpt1,
    const std::string& rpt2)
{
    bool isNew =
        !m_streams.count(streamId);

    DStarStream& s =
        m_streams[streamId];

    s.streamId = streamId;
    s.peer = peer;

    s.mycall = mycall;
    s.urcall = urcall;

    s.rpt1 = rpt1;
    s.rpt2 = rpt2;

    if (isNew) {
        s.lastSequence = 0;
        s.hasSequence = false;
    }

    s.lastActivity = time(nullptr);

    if (isNew) {

        Logger::log(INFO,
            "D-Star stream created: " +
            std::to_string(streamId) +
            " MYCALL=" + mycall);
    }
    else {

        Logger::log(INFO,
            "D-Star stream updated: " +
            std::to_string(streamId));
    }
}

bool DStarSessionManager::hasStream(
    uint16_t streamId)
{
    return m_streams.count(streamId) > 0;
}

std::string DStarSessionManager::getStreamPeer(
    uint16_t streamId)
{
    if (!m_streams.count(streamId))
        return "";

    return m_streams[streamId].peer;
}

void DStarSessionManager::touchStream(
    uint16_t streamId)
{
    if (!m_streams.count(streamId))
        return;

    m_streams[streamId].lastActivity =
        time(nullptr);
}

bool DStarSessionManager::acceptSequence(
    uint16_t streamId,
    uint8_t sequence)
{
    if (!m_streams.count(streamId))
        return false;

    DStarStream& s =
        m_streams[streamId];

    uint8_t cleanSeq =
        sequence & 0x1F;

    if (!s.hasSequence) {

        s.lastSequence = cleanSeq;
        s.hasSequence = true;

        return true;
    }

    if (cleanSeq == s.lastSequence) {

        Logger::log(INFO,
            "Duplicate D-Star sequence suppressed:"
            " STREAMID=" +
            std::to_string(streamId) +
            " SEQ=" +
            std::to_string(cleanSeq));

        return false;
    }

    uint8_t expected =
        (s.lastSequence + 1) % 21;

    if (cleanSeq != expected) {

        int forwardGap =
            (cleanSeq + 21 - expected) % 21;

        if (forwardGap <= 2) {

            Logger::log(INFO,
                "D-Star jitter tolerated:"
                " STREAMID=" +
                std::to_string(streamId) +
                " EXPECTED=" +
                std::to_string(expected) +
                " GOT=" +
                std::to_string(cleanSeq) +
                " GAP=" +
                std::to_string(forwardGap));
        }
        else {

            Logger::log(INFO,
                "Out-of-order D-Star sequence:"
                " STREAMID=" +
                std::to_string(streamId) +
                " EXPECTED=" +
                std::to_string(expected) +
                " GOT=" +
                std::to_string(cleanSeq) +
                " GAP=" +
                std::to_string(forwardGap));
        }
    }

    s.lastSequence = cleanSeq;

    return true;
}

void DStarSessionManager::endStream(
    uint16_t streamId)
{
    if (!m_streams.count(streamId))
        return;

    Logger::log(INFO,
        "D-Star stream ended: " +
        std::to_string(streamId));

    m_streams.erase(streamId);
}

void DStarSessionManager::cleanup()
{
    time_t now = time(nullptr);

    for (auto it = m_streams.begin();
         it != m_streams.end(); )
    {
        if ((now - it->second.lastActivity) > 30) {

            Logger::log(INFO,
                "D-Star stream timeout: " +
                std::to_string(it->first));

            it = m_streams.erase(it);
        }
        else {

            ++it;
        }
    }
}

std::string DStarSessionManager::getMyCall(
    uint16_t streamId)
{
    auto it =
        m_streams.find(
            streamId);

    if (it == m_streams.end()) {
        return "";
    }

    return it->second.mycall;
}
