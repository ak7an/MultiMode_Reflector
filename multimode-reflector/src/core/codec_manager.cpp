#include "codec_manager.h"

#include "ambe_device_manager.h"
#include "logger.h"

CodecFrame CodecManager::decode(
    const CodecFrame& input)
{
    Logger::log(INFO,
        "CodecManager decode:"
        " STREAMID=" +
        std::to_string(input.streamId));

    if (input.codec == CodecType::AMBE)
    {
        return AMBEDeviceManager::decodeAMBE(
            input);
    }

    return input;
}

CodecFrame CodecManager::encode(
    const CodecFrame& input,
    CodecType targetCodec)
{
    Logger::log(INFO,
        "CodecManager encode:"
        " STREAMID=" +
        std::to_string(input.streamId));

    if (targetCodec == CodecType::AMBE)
    {
        return AMBEDeviceManager::encodeAMBE(
            input);
    }

    CodecFrame output =
        input;

    output.codec =
        targetCodec;

    return output;
}
