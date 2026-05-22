#include "media_router.h"

#include "logger.h"

void MediaRouter::route(
    const MediaFrame& frame)
{
    Logger::log(INFO,
        "MediaRouter route:"
        " STREAMID=" +
        std::to_string(frame.streamId) +
        " SEQ=" +
        std::to_string(frame.sequence));
}
