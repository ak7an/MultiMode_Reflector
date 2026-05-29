#pragma once

#include "media_frame.h"
#include "pcm_frame.h"

#include <vector>
#include <cstdint>

class ProtocolCodecMapper
{
public:
    static std::vector<uint8_t> mapPCMToProtocolPayload(
        const PCMFrame& pcm,
        MediaProtocol targetProtocol);
};
