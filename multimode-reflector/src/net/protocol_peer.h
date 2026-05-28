#pragma once

#include <string>
#include <netinet/in.h>

#include "../protocol/protocol_definitions.h"

struct ProtocolPeer
{
    ProtocolType protocol =
        ProtocolType::UNKNOWN;

    std::string host;

    int port = 0;

    sockaddr_in address {};
};
