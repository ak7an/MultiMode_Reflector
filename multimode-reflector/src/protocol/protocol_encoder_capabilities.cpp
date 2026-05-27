#include "protocol_encoder_capabilities.h"

bool ProtocolEncoderCapabilities::canEncode(
    MediaProtocol protocol)
{
    switch (protocol) {

    case MediaProtocol::DSTAR:
    case MediaProtocol::YSF:
        return true;

    default:
        return false;
    }
}
