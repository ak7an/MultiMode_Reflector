#include "nxdn_scaffold_frame.h"

#include "../core/logger.h"

#include <cstring>

bool NXDNScaffoldFrame::parse(
    const uint8_t* data,
    size_t length,
    MediaFrame& frame)
{
    if (data == nullptr)
        return false;

    if (length < 12)
        return false;

    if (std::memcmp(data, "NXDN", 4) != 0)
        return false;

    frame.protocol =
        MediaProtocol::NXDN;

    frame.streamId =
        static_cast<uint16_t>(
            (data[4] << 8) |
             data[5]);

    frame.sequence =
        data[6];

    frame.endOfTransmission =
        data[7] != 0;

    frame.frameType =
        frame.endOfTransmission
            ? MediaFrameType::VOICE_EOT
            : MediaFrameType::VOICE;

    uint16_t payloadLength =
        static_cast<uint16_t>(
            (data[8] << 8) |
             data[9]);

    if (length < 10 + payloadLength)
        return false;

    frame.payload.assign(
        data + 10,
        data + 10 + payloadLength);

    Logger::log(INFO,
        "NXDNScaffoldFrame parse:"
        " STREAMID=" +
        std::to_string(frame.streamId) +
        " SEQ=" +
        std::to_string(frame.sequence) +
        " PAYLOAD=" +
        std::to_string(frame.payload.size()) +
        " EOT=" +
        std::to_string(frame.endOfTransmission ? 1 : 0));

    return true;
}
