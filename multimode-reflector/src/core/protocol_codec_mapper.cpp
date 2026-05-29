#include "protocol_codec_mapper.h"

#include "logger.h"

std::vector<uint8_t> ProtocolCodecMapper::mapPCMToProtocolPayload(
    const PCMFrame& pcm,
    MediaProtocol targetProtocol)
{
    Logger::log(INFO,
        "ProtocolCodecMapper map PCM:"
        " STREAMID=" +
        std::to_string(pcm.streamId) +
        " SAMPLES=" +
        std::to_string(pcm.samples.size()) +
        " TARGET=" +
        std::to_string(static_cast<int>(targetProtocol)));

    std::vector<uint8_t> payload;

    for (int16_t sample : pcm.samples)
    {
        uint16_t value =
            static_cast<uint16_t>(sample);

        payload.push_back(value & 0xff);
        payload.push_back((value >> 8) & 0xff);
    }

    return payload;
}
