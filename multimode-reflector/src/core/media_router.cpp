#include "media_router.h"

#include "transcoder.h"
#include "transcoding_matrix.h"
#include "transcoding_policy.h"
#include "logger.h"
#include "../protocol/protocol_capabilities.h"

static std::string frameTypeToString(
    MediaFrameType type)
{
    switch (type) {

    case MediaFrameType::HEADER:
        return "HEADER";

    case MediaFrameType::VOICE:
        return "VOICE";

    case MediaFrameType::VOICE_EOT:
        return "VOICE_EOT";

    case MediaFrameType::CONTROL:
        return "CONTROL";

    default:
        return "UNKNOWN";
    }
}

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

MediaRouteResult MediaRouter::route(
    const MediaFrame& frame)
{
    MediaRouteResult result{};

    result.action =
        RouteAction::DROP;

    result.reason =
        "unset";

    if (frame.protocol ==
        MediaProtocol::UNKNOWN)
    {
        result.reason =
            "unknown protocol";

        Logger::log(INFO,
            "MediaRouter drop: " +
            result.reason);

        return result;
    }

    if (frame.endOfTransmission) {

        result.action =
            RouteAction::RECORD;

        result.reason =
            "record eot";

        Logger::log(INFO,
            "MediaRouter action:"
            " RECORD"
            " STREAMID=" +
            std::to_string(frame.streamId));

        return result;
    }


    auto targets =
        TranscodingPolicy::getTargets(
            frame);

    if (!targets.empty())
    {
        result.action =
            RouteAction::TRANSCODE;

        for (auto target : targets)
        {
            MediaFrame transcoded =
                Transcoder::transcode(
                    frame,
                    target);

            Logger::log(INFO,
                "Transcoded frame ready:"
                " TARGET=" +
                protocolToString(target) +
                " STREAMID=" +
                std::to_string(
                    transcoded.streamId));

            result.transcodedFrames.push_back(
                transcoded);
        }

        result.reason =
            "dynamic-transcode";
    }
    else {


        result.action =
            RouteAction::FORWARD;

        result.reason =
            "accepted";
    }

    MediaDestination dest{};

    dest.type =
        MediaDestinationType::ALL;

    dest.value =
        "ALL";

    result.destinations.push_back(
        dest);

    Logger::log(INFO,
        "MediaRouter route:"
        " PROTO=" +
        protocolToString(frame.protocol) +
        " STREAMID=" +
        std::to_string(frame.streamId) +
        " TYPE=" +
        frameTypeToString(frame.frameType) +
        " SEQ=" +
        std::to_string(frame.sequence) +
        " EOT=" +
        std::to_string(frame.endOfTransmission) +
        " RESULT=" +
        result.reason +
        " DEST=ALL");

    return result;
}
