#pragma once

#include <string>
#include <netinet/in.h>
#include <chrono>
#include <cstdint>

#include "../protocol/protocol_definitions.h"

struct ProtocolPeer
{
    ProtocolType protocol =
        ProtocolType::UNKNOWN;

    std::string host;

    int port = 0;

    std::string reflector;

    char module = 'A';

    sockaddr_in address {};

    std::chrono::steady_clock::time_point
        lastPollSent {};

    std::chrono::steady_clock::time_point
        lastPollReceived {};

    bool connected = false;

    bool sessionEstablished = false;

    uint32_t protocolVersion = 0;
};
