#include "codec_router.h"

#include "codec_manager.h"
#include "audio_level_manager.h"
#include "logger.h"

#include <cstdint>

static PCMFrame codecToPCMFrame(
    const CodecFrame& codec,
    MediaProtocol sourceProtocol,
    MediaProtocol targetProtocol)
{
    PCMFrame pcm;

    pcm.sourceProtocol =
        sourceProtocol;

    pcm.targetProtocol =
        targetProtocol;

    pcm.streamId =
        codec.streamId;

    pcm.sequence =
        codec.sequence;

    pcm.endOfTransmission =
        codec.endOfTransmission;

    pcm.sampleRate =
        8000;

    for (size_t i = 0; i + 1 < codec.payload.size(); i += 2)
    {
        int16_t sample =
            static_cast<int16_t>(
                static_cast<uint16_t>(codec.payload[i]) |
                (static_cast<uint16_t>(codec.payload[i + 1]) << 8));

        pcm.samples.push_back(
            sample);
    }

    return pcm;
}

static CodecFrame pcmFrameToCodec(
    const PCMFrame& pcm)
{
    CodecFrame codec;

    codec.codec =
        CodecType::PCM;

    codec.streamId =
        pcm.streamId;

    codec.sequence =
        pcm.sequence;

    codec.endOfTransmission =
        pcm.endOfTransmission;

    codec.payload.reserve(
        pcm.samples.size() * 2);

    for (int16_t sample : pcm.samples)
    {
        uint16_t value =
            static_cast<uint16_t>(
                sample);

        codec.payload.push_back(
            value & 0xff);

        codec.payload.push_back(
            (value >> 8) & 0xff);
    }

    return codec;
}

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
        "CodecRouter decode:"
        " STREAMID=" +
        std::to_string(media.streamId));

    return CodecManager::decode(
        frame);
}

MediaFrame CodecRouter::encode(
    const CodecFrame& codec,
    MediaProtocol targetProtocol)
{
    CodecFrame normalizedCodec =
        codec;

    if (codec.codec == CodecType::PCM)
    {
        PCMFrame pcm =
            codecToPCMFrame(
                codec,
                MediaProtocol::DSTAR,
                targetProtocol);

        PCMFrame normalized =
            AudioLevelManager::normalize(
                pcm);

        normalizedCodec =
            pcmFrameToCodec(
                normalized);

        Logger::log(INFO,
            "CodecRouter PCM normalized:"
            " STREAMID=" +
            std::to_string(codec.streamId) +
            " SAMPLES=" +
            std::to_string(normalized.samples.size()));
    }

    CodecFrame encoded =
        CodecManager::encode(
            normalizedCodec,
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
        "CodecRouter encode:"
        " STREAMID=" +
        std::to_string(codec.streamId));

    return media;
}
