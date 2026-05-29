#include "codec_router.h"

#include "codec_manager.h"

#include "logger.h"

CodecFrame CodecRouter::decode(
    const MediaFrame& media)
{
    CodecFrame frame;

    frame.codec =
        CodecType::AMBE;

    frame.streamId =
        media.streamId;

    frame.sequence =
        media.sequence;

    frame.endOfTransmission =
        media.endOfTransmission;

    frame.payload =
        media.payload;

    Logger::log(INFO,
        "CodecRouter decode stub:"
        " STREAMID=" +
        std::to_string(media.streamId));

    return CodecManager::decode(
        frame);
}

MediaFrame CodecRouter::encode(
    const CodecFrame& codec,
    MediaProtocol targetProtocol)
{
    CodecFrame encoded =
        CodecManager::encode(
            codec,
            CodecType::AMBE);

    MediaFrame media;

    media.protocol =
        targetProtocol;

    media.streamId =
        encoded.streamId;

    media.sequence =
        encoded.sequence;

    media.endOfTransmission =
        encoded.endOfTransmission;

    media.payload =
        encoded.payload;

    Logger::log(INFO,
        "CodecRouter encode stub:"
        " STREAMID=" +
        std::to_string(codec.streamId));

    return media;
}
