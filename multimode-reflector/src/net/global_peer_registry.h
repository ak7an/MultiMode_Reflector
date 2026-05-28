#pragma once

#include "protocol_peer_registry.h"

class GlobalPeerRegistry
{
public:

    static void setRegistry(
        ProtocolPeerRegistry* registry);

    static ProtocolPeerRegistry*
    registry();

private:

    static ProtocolPeerRegistry*
        m_registry;
};
