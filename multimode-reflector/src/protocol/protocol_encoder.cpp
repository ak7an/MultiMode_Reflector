#include "protocol_encoder.h"

#include "ysf_encoder.h"
#include "dstar_encoder.h"
#include "dmr_network_builder.h"
#include "nxdn_network_builder.h"
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

    if (frame.protocol ==
        MediaProtocol::DSTAR)
    {
        return DStarEncoder::encode(
            frame);
    }

    if (frame.protocol ==
        MediaProtocol::DMR)
    {
        return DMRNetworkBuilder::build(
            frame);
    }

    if (frame.protocol ==
        MediaProtocol::NXDN)
    {
        return NXDNNetworkBuilder::build(
            frame);
    }

    Logger::log(INFO,
        "ProtocolEncoder unsupported media protocol");

    return {};
}
