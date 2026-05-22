#pragma once

#include "media_frame.h"

#include <string>
#include <vector>

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

    std::vector<std::string> destinations;
};


class MediaRouter {
public:

    static MediaRouteResult route(
        const MediaFrame& frame);
};
