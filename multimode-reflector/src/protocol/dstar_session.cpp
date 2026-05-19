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

    s.lastActivity = time(nullptr);

    if (isNew) {

        Logger::log(INFO,
            "D-Star stream created: " +
            std::to_string(streamId) +
            " MYCALL=" + mycall);
    }
}

void DStarSessionManager::cleanup()
{
    time_t now = time(nullptr);

    for (auto it = m_streams.begin();
         it != m_streams.end();)
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
