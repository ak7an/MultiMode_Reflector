#pragma once

#include "codec_frame.h"
#include "media_frame.h"

class CodecRouter
{
public:

    static CodecFrame decode(
        const MediaFrame& media);

    static MediaFrame encode(
        const CodecFrame& codec,
        MediaProtocol targetProtocol);
};
