#include "protocol_encoder_capabilities.h"

bool ProtocolEncoderCapabilities::canEncode(
    MediaProtocol protocol)
{
    switch (protocol) {

    case MediaProtocol::DSTAR:
    case MediaProtocol::YSF:
    case MediaProtocol::DMR:
        return true;

    default:
        return false;
    }
}
