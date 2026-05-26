#pragma once

#include "media_frame.h"

class Transcoder {
public:

    static MediaFrame transcode(
        const MediaFrame& input,
        MediaProtocol targetProtocol);
};
