#include "dmr_voice_frame.h"

#include "../core/logger.h"

#include <algorithm>
#include <cstring>

namespace
{
    constexpr size_t DMR_VOICE_PAYLOAD_SIZE = 35;
}

std::vector<uint8_t> DMRVoiceFrame::build(
    const MediaFrame& frame)
{
    std::vector<uint8_t> payload(
        DMR_VOICE_PAYLOAD_SIZE,
        0);

    const size_t copyLength =
        std::min(
            frame.payload.size(),
            payload.size());

    if (copyLength > 0)
    {
        std::memcpy(
            payload.data(),
            frame.payload.data(),
            copyLength);
    }

    Logger::log(INFO,
        "DMRVoiceFrame build:"
        " STREAMID=" +
        std::to_string(frame.streamId) +
        " INPUT_PAYLOAD=" +
        std::to_string(frame.payload.size()) +
        " DMR_PAYLOAD=" +
        std::to_string(payload.size()));

    return payload;
}

bool DMRVoiceFrame::parse(
    const uint8_t* data,
    size_t length,
    MediaFrame& frame)
{
    if (length == 0)
    {
        return false;
    }

    frame.payload.assign(
        data,
        data + length);

    Logger::log(INFO,
        "DMRVoiceFrame parse:"
        " STREAMID=" +
        std::to_string(frame.streamId) +
        " PAYLOAD=" +
        std::to_string(frame.payload.size()));

    return true;
}
