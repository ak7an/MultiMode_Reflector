#pragma once

#include "media_frame.h"

class MediaPacer {
public:

    static void pace(
        const MediaFrame& frame,
        int targetDelayMs);
};
