#include "media_pacer.h"

#include "logger.h"

#include <chrono>
#include <thread>

std::unordered_map<
    std::string,
    std::chrono::steady_clock::time_point>
MediaPacer::m_nextSend;

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

std::string MediaPacer::makeKey(
    MediaProtocol protocol,
    uint16_t streamId)
{
    return protocolToString(protocol) +
        ":" +
        std::to_string(streamId);
}

void MediaPacer::pace(
    const MediaFrame& frame,
    int intervalMs)
{
    auto now =
        std::chrono::steady_clock::now();

    std::string key =
        makeKey(
            frame.protocol,
            frame.streamId);

    auto& next =
        m_nextSend[key];

    if (next.time_since_epoch().count() == 0) {

        next =
            now;
    }

    if (now < next) {

        auto sleepMs =
            std::chrono::duration_cast<
                std::chrono::milliseconds>(
                    next - now).count();

        Logger::log(INFO,
            "MediaPacer stream delay:"
            " KEY=" +
            key +
            " SLEEP_MS=" +
            std::to_string(sleepMs));

        std::this_thread::sleep_for(
            next - now);
    }

    next =
        std::chrono::steady_clock::now() +
        std::chrono::milliseconds(
            intervalMs);

    Logger::log(INFO,
        "MediaPacer scheduled next:"
        " KEY=" +
        key +
        " INTERVAL_MS=" +
        std::to_string(intervalMs));
}
