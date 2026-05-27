#pragma once

#include "../core/media_frame.h"

class ProtocolEncoderCapabilities
{
public:
    static bool canEncode(
        MediaProtocol protocol);
};
