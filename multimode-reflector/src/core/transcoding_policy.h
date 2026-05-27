#pragma once

#include "media_frame.h"

#include <vector>

class TranscodingPolicy
{
public:
    static std::vector<MediaProtocol>
    getTargets(
        const MediaFrame& frame);
};
