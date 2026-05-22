#pragma once

#include "media_frame.h"

#include <string>

struct MediaRouteResult {

    bool forward;

    std::string reason;
};

class MediaRouter {
public:

    static MediaRouteResult route(
        const MediaFrame& frame);
};
