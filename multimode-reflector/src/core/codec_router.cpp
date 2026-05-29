#include "codec_router.h"

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

    return frame;
}

MediaFrame CodecRouter::encode(
    const CodecFrame& codec,
    MediaProtocol targetProtocol)
{
    MediaFrame media;

    media.protocol =
        targetProtocol;

    media.streamId =
        codec.streamId;

    media.sequence =
        codec.sequence;

    media.endOfTransmission =
        codec.endOfTransmission;

    media.payload =
        codec.payload;

    Logger::log(INFO,
        "CodecRouter encode stub:"
        " STREAMID=" +
        std::to_string(codec.streamId));

    return media;
}
