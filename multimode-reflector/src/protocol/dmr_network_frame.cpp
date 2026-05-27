#include "dmr_network_frame.h"

#include <cstring>

bool DMRNetworkFrame::parse(
    const uint8_t* data,
    size_t length,
    MediaFrame& frame)
{
    if (length < 55) {
        return false;
    }

    if (std::memcmp(data, "DMRD", 4) != 0) {
        return false;
    }

    frame.protocol =
        MediaProtocol::DMR;

    frame.streamId =
        (data[4] << 8) |
         data[5];

    frame.sequence =
        data[6];

    frame.frameType =
        MediaFrameType::VOICE;

    frame.payload.assign(
        data + 20,
        data + length);

    return true;
}
