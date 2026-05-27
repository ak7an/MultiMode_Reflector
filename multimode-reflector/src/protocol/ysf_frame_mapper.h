#pragma once

#include "../core/media_frame.h"
#include "ysf_frame_type.h"

class YSFFrameMapper {
public:

    static YSFFrameType fromMedia(
        MediaFrameType type);

    static MediaFrameType toMedia(
        YSFFrameType type);
};
