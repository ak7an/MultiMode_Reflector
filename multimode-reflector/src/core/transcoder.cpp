#include "transcoder.h"

#include "codec_router.h"
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
        "Transcoder codec path:"
        " FROM=" +
        protocolToString(input.protocol) +
        " TO=" +
        protocolToString(targetProtocol) +
        " STREAMID=" +
        std::to_string(input.streamId) +
        " SEQ=" +
        std::to_string(input.sequence));

    CodecFrame codec =
        CodecRouter::decode(
            input);

    MediaFrame output =
        CodecRouter::encode(
            codec,
            targetProtocol);

    output.frameType =
        input.frameType;

    output.sourceCallsign =
        input.sourceCallsign;

    output.sourcePeer =
        input.sourcePeer;

    output.sourceReflector =
        input.sourceReflector;

    output.sourceModule =
        input.sourceModule;

    output.createdAt =
        std::chrono::steady_clock::now();

    return output;
}
