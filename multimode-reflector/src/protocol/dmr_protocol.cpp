#include "dmr_protocol.h"
#include "dmr_network_frame.h"

#include "../core/logger.h"
#include "../core/media_router.h"

#include <chrono>

ProtocolResult DMRProtocol::handle(
    const uint8_t* data,
    size_t length,
    const std::string& peer)
{
    ProtocolResult result{};
    result.forwardCurrent = false;

    MediaFrame frame{};

    if (!DMRNetworkFrame::parse(
            data,
            length,
            frame))
    {
        Logger::log(WARN,
            "DMR packet rejected by parser");

        return result;
    }

    frame.sourcePeer =
        peer;

    frame.createdAt =
        std::chrono::steady_clock::now();

    Logger::log(INFO,
        "DMR MediaFrame created:"
        " STREAMID=" +
        std::to_string(frame.streamId) +
        " SEQ=" +
        std::to_string(frame.sequence) +
        " PAYLOAD=" +
        std::to_string(frame.payload.size()));

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
