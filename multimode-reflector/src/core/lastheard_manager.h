#pragma once

#include <string>
#include <vector>
#include <ctime>
#include <cstdint>

struct LastHeardEntry {

    std::string callsign;

    std::string peer;

    uint16_t streamId;

    time_t timestamp;
};

class LastHeardManager {
public:

    static void addEntry(
        const std::string& callsign,
        const std::string& peer,
        uint16_t streamId);

    static void dump();

private:

    static std::vector<
        LastHeardEntry> m_entries;
};
