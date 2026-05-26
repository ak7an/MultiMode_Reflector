#include "protocol_encoder.h"

#include "ysf_encoder.h"
#include "../core/logger.h"

std::vector<uint8_t> ProtocolEncoder::encode(
    const MediaFrame& frame)
{
    if (frame.protocol ==
        MediaProtocol::YSF)
    {
        return YSFEncoder::encode(
            frame);
    }

    Logger::log(INFO,
        "ProtocolEncoder unsupported media protocol");

    return {};
}
