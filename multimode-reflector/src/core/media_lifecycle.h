#pragma once

#include "media_frame.h"

class MediaLifecycle {
public:

    static void endStream(
        MediaProtocol protocol,
        uint16_t streamId);
};
