#include "dmr_protocol.h"

#include "../core/logger.h"

ProtocolResult DMRProtocol::handle(
    const uint8_t* data,
    size_t length,
    const std::string& peer)
{
    ProtocolResult result{};
    result.forwardCurrent = false;

    Logger::log(INFO,
        "DMR protocol skeleton received:"
        " PEER=" + peer +
        " LEN=" + std::to_string(length));

    return result;
}
