#include "media_router.h"

#include "logger.h"

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

bool MediaRouter::route(
    const MediaFrame& frame)
{
    if (frame.protocol ==
        MediaProtocol::UNKNOWN)
    {
        Logger::log(INFO,
            "MediaRouter drop:"
            " UNKNOWN protocol");

        return false;
    }

    Logger::log(INFO,
        "MediaRouter route:"
        " PROTO=" +
        protocolToString(frame.protocol) +
        " STREAMID=" +
        std::to_string(frame.streamId) +
        " SEQ=" +
        std::to_string(frame.sequence) +
        " EOT=" +
        std::to_string(frame.endOfTransmission));

    return true;
}
