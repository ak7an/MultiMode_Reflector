#include "ysf_protocol.h"

#include "../core/logger.h"
#include "../core/media_router.h"
#include "../core/media_output_queue.h"

#include <cstring>

static MediaFrameType frameTypeFromCode(
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

ProtocolResult YSFProtocol::handle(
    const uint8_t* data,
    size_t length,
    const std::string& peer)
{
    ProtocolResult result{};
    result.forwardCurrent = false;

    MediaFrame frame{};

    if (!parseSyntheticBridge(
            data,
            length,
            peer,
            frame))
    {
        Logger::log(WARN,
            "YSF packet rejected by parser");

        return result;
    }

    Logger::log(INFO,
        "YSF media frame accepted:"
        " STREAMID=" +
        std::to_string(frame.streamId) +
        " SEQ=" +
        std::to_string(frame.sequence));

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

bool YSFProtocol::parseSyntheticBridge(
    const uint8_t* data,
    size_t length,
    const std::string& peer,
    MediaFrame& frame)
{
    if (length < 15) {
        return false;
    }

    bool synthetic =
        std::memcmp(data, "YSFB", 4) == 0;

    bool network =
        std::memcmp(data, "YSFD", 4) == 0;

    if (!synthetic && !network) {
        return false;
    }

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

        uint16_t payloadLength =
            (data[12] << 8) |
             data[13];

        if (length >= 15 + payloadLength) {

            frame.payload.assign(
                data + 15,
                data + 15 + payloadLength);
        }
    }
    else {

        const size_t payloadOffset = 35;

        if (length > payloadOffset) {

            frame.payload.assign(
                data + payloadOffset,
                data + length);
        }
    }

    frame.sourcePeer =
        peer;

    Logger::log(INFO,
        std::string("YSF parsed: MODE=") +
        (synthetic ? "synthetic" : "network") +
        " STREAMID=" +
        std::to_string(frame.streamId) +
        " SEQ=" +
        std::to_string(frame.sequence) +
        " PAYLOAD=" +
        std::to_string(frame.payload.size()));

    return true;
}
