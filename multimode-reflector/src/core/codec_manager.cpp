#include "codec_manager.h"

#include "logger.h"

CodecFrame CodecManager::decode(
    const CodecFrame& input)
{
    Logger::log(INFO,
        "CodecManager decode stub:"
        " STREAMID=" +
        std::to_string(input.streamId));

    return input;
}

CodecFrame CodecManager::encode(
    const CodecFrame& input,
    CodecType targetCodec)
{
    CodecFrame output =
        input;

    output.codec =
        targetCodec;

    Logger::log(INFO,
        "CodecManager encode stub:"
        " STREAMID=" +
        std::to_string(input.streamId));

    return output;
}
