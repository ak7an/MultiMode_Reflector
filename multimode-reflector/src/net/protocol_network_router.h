#pragma once

#include <vector>
#include <cstdint>

#include "protocol_peer_registry.h"

#include "../protocol/protocol_definitions.h"

class ProtocolNetworkRouter
{
public:

    explicit ProtocolNetworkRouter(
        ProtocolPeerRegistry& registry);

    void routePacket(
        ProtocolType proto,
        const std::vector<uint8_t>& packet);

private:

    ProtocolPeerRegistry& m_registry;
};
