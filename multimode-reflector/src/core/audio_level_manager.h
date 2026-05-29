#pragma once

#include "pcm_frame.h"

class AudioLevelManager
{
public:

    static PCMFrame normalize(
        const PCMFrame& input);
};
