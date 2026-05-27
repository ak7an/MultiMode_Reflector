#include "transcoder.h"

#include "logger.h"
#include <chrono>

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

MediaFrame Transcoder::transcode(
    const MediaFrame& input,
    MediaProtocol targetProtocol)
{
    Logger::log(INFO,
        "Transcoder stub:"
        " FROM=" +
        protocolToString(input.protocol) +
        " TO=" +
        protocolToString(targetProtocol) +
        " STREAMID=" +
        std::to_string(input.streamId) +
        " SEQ=" +
        std::to_string(input.sequence));

    MediaFrame output =
        input;

    output.createdAt =
        std::chrono::steady_clock::now();

    output.protocol =
        targetProtocol;

    return output;
}
