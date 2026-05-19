#pragma once

#include <string>
#include <unordered_map>
#include <ctime>
#include <cstdint>

struct DStarStream {

    uint16_t streamId;

    std::string peer;

    std::string mycall;
    std::string urcall;

    std::string rpt1;
    std::string rpt2;

    time_t lastActivity;
};

class DStarSessionManager {
public:

    static void createOrUpdate(
        uint16_t streamId,
        const std::string& peer,
        const std::string& mycall,
        const std::string& urcall,
        const std::string& rpt1,
        const std::string& rpt2);

    static void cleanup();

private:

    static std::unordered_map<
        uint16_t,
        DStarStream> m_streams;
};
