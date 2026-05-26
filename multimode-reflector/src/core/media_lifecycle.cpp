#include "media_lifecycle.h"

#include "logger.h"
#include "media_pacer.h"

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

void MediaLifecycle::endStream(
    MediaProtocol protocol,
    uint16_t streamId)
{
    Logger::log(INFO,
        "MediaLifecycle end stream:"
        " PROTO=" +
        protocolToString(protocol) +
        " STREAMID=" +
        std::to_string(streamId));

    MediaPacer::reset(
        protocol,
        streamId);
}
