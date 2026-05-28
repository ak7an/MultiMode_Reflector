#pragma once

#include "../protocol/protocol_definitions.h"

struct ListenerSocket
{
    int fd = -1;
    ProtocolType protocol = ProtocolType::UNKNOWN;
    int port = 0;
};
