#include "ysf_protocol.h"
#include "ysf_scaffold_frame.h"

#include "../core/logger.h"
#include "../core/media_router.h"
#include "../core/media_output_queue.h"

#include <cstring>

namespace
{
    constexpr const char* YSF_SYNTHETIC_MAGIC = "YSFB";
    constexpr const char* YSF_SCAFFOLD_MAGIC  = "YSFD";

    MediaFrameType frameTypeFromCode(
        uint8_t code)
    {
        switch (code) {

        case 1:
            return MediaFrameType::HEADER;

        case 2:
            return MediaFrameType::VOICE;

        case 3:
            return MediaFrameType::VOICE_EOT;

        case 4:
            return MediaFrameType::CONTROL;

        default:
            return MediaFrameType::UNKNOWN;
        }
    }
}

ProtocolResult YSFProtocol::handle(
    const uint8_t* data,
    size_t length,
    const std::string& peer)
{
    ProtocolResult result{};
    result.forwardCurrent = false;

    MediaFrame frame{};

    if (!parseProjectBridgeFrame(
            data,
            length,
            peer,
            frame))
    {
        Logger::log(WARN,
            "YSF packet rejected: unsupported YSF format");

        return result;
    }

    Logger::log(INFO,
        "YSF media frame accepted:"
        " STREAMID=" +
        std::to_string(frame.streamId) +
        " SEQ=" +
        std::to_string(frame.sequence) +
        " NOTE=project-scaffold-format");

    MediaRouteResult routeResult =
        MediaRouter::route(
            frame);

    for (const auto& media :
         routeResult.transcodedFrames)
    {
        result.transcodedFrames.push_back(
            media);
    }

    return result;
}

bool YSFProtocol::parseProjectBridgeFrame(
    const uint8_t* data,
    size_t length,
    const std::string& peer,
    MediaFrame& frame)
{
    if (data == nullptr)
        return false;

    if (length < 15)
        return false;

    const bool synthetic =
        std::memcmp(data, YSF_SYNTHETIC_MAGIC, 4) == 0;

    const bool scaffoldNetwork =
        std::memcmp(data, YSF_SCAFFOLD_MAGIC, 4) == 0;

    if (!synthetic && !scaffoldNetwork)
        return false;

    frame.protocol =
        MediaProtocol::YSF;

    frame.streamId =
        (data[4] << 8) |
         data[5];

    frame.sequence =
        data[6];

    frame.endOfTransmission =
        data[7] != 0;

    frame.frameType =
        frameTypeFromCode(
            data[8]);

    frame.payload.clear();

    if (synthetic) {

        const uint16_t payloadLength =
            (data[12] << 8) |
             data[13];

        if (length < 15 + payloadLength)
            return false;

        frame.payload.assign(
            data + 15,
            data + 15 + payloadLength);
    }
    else {

        if (!YSFScaffoldFrame::parse(
                data,
                length,
                frame))
        {
            return false;
        }
    }

    frame.sourcePeer =
        peer;

    Logger::log(INFO,
        std::string("YSF parsed project bridge frame:")
        + " MODE=" +
        (synthetic ? "YSFB-synthetic" : "YSFD-scaffold-network") +
        " STREAMID=" +
        std::to_string(frame.streamId) +
        " SEQ=" +
        std::to_string(frame.sequence) +
        " PAYLOAD=" +
        std::to_string(frame.payload.size()) +
        " REAL_YSF=0");

    return true;
}
