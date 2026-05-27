#include "ysf_frame_mapper.h"

YSFFrameType
YSFFrameMapper::fromMedia(
    MediaFrameType type)
{
    switch (type) {

    case MediaFrameType::HEADER:
        return YSFFrameType::HEADER;

    case MediaFrameType::VOICE:
        return YSFFrameType::VOICE;

    case MediaFrameType::VOICE_EOT:
        return YSFFrameType::VOICE_EOT;

    case MediaFrameType::CONTROL:
        return YSFFrameType::CONTROL;

    default:
        return YSFFrameType::UNKNOWN;
    }
}

MediaFrameType
YSFFrameMapper::toMedia(
    YSFFrameType type)
{
    switch (type) {

    case YSFFrameType::HEADER:
        return MediaFrameType::HEADER;

    case YSFFrameType::VOICE:
        return MediaFrameType::VOICE;

    case YSFFrameType::VOICE_EOT:
        return MediaFrameType::VOICE_EOT;

    case YSFFrameType::CONTROL:
        return MediaFrameType::CONTROL;

    default:
        return MediaFrameType::UNKNOWN;
    }
}
