#pragma once

#include "media_frame.h"

#include <string>

enum class RouteAction {

    DROP,
    FORWARD,
    TRANSCODE,
    MIRROR,
    RECORD,
    MIX
};

struct MediaRouteResult {

    RouteAction action;

    std::string reason;
};

class MediaRouter {
public:

    static MediaRouteResult route(
        const MediaFrame& frame);
};
