#include "m17_protocol.h"
#include "m17_scaffold_frame.h"

#include "../core/logger.h"
#include "../core/media_router.h"

#include <chrono>

ProtocolResult M17Protocol::handle(
    const uint8_t* data,
    size_t length,
    const std::string& peer)
{
    ProtocolResult result{};
    result.forwardCurrent = false;

    MediaFrame frame{};

    if (!M17ScaffoldFrame::parse(
            data,
            length,
            frame))
    {
        Logger::log(WARN,
            "M17 packet rejected by scaffold parser");

        return result;
    }

    frame.sourcePeer =
        peer;

    frame.createdAt =
        std::chrono::steady_clock::now();

    Logger::log(INFO,
        "M17 MediaFrame created:"
        " STREAMID=" +
        std::to_string(frame.streamId) +
        " SEQ=" +
        std::to_string(frame.sequence) +
        " PAYLOAD=" +
        std::to_string(frame.payload.size()) +
        " NOTE=Codec2-path-pending");

    MediaRouteResult routeResult =
        MediaRouter::route(
            frame);

    result.destinations =
        routeResult.destinations;

    result.transcodedFrames =
        routeResult.transcodedFrames;

    result.forwardCurrent = true;

    return result;
}
