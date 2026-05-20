#include "lastheard_manager.h"

#include "logger.h"

#include <iostream>

std::vector<
    LastHeardEntry>
LastHeardManager::m_entries;

void LastHeardManager::addEntry(
    const std::string& callsign,
    const std::string& peer,
    uint16_t streamId)
{
    LastHeardEntry entry{};

    entry.callsign = callsign;
    entry.peer = peer;
    entry.streamId = streamId;
    entry.timestamp = time(nullptr);

    m_entries.push_back(entry);

    Logger::log(INFO,
        "LastHeard: " +
        callsign +
        " via " +
        peer +
        " stream=" +
        std::to_string(streamId));

    /*
     * Prevent unbounded growth
     */

    if (m_entries.size() > 50) {

        m_entries.erase(
            m_entries.begin());
    }
}

void LastHeardManager::dump()
{
    Logger::log(INFO,
        "===== LAST HEARD =====");

    for (const auto& e : m_entries) {

        Logger::log(INFO,
            e.callsign +
            " via " +
            e.peer +
            " stream=" +
            std::to_string(
                e.streamId));
    }
}
