#pragma once

#include "media_frame.h"

#include <vector>

class TranscodingTargets
{
public:
    static std::vector<MediaProtocol>
    getTargets(
        MediaProtocol source);
};
