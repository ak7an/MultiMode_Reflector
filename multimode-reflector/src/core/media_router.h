#pragma once

#include "media_frame.h"

#include <string>
#include <vector>
#include "media_destination.h"

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
    std::vector<MediaDestination> destinations;

};


class MediaRouter {
public:

    static MediaRouteResult route(
        const MediaFrame& frame);
};
