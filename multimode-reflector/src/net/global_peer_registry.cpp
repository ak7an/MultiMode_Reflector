#include "global_peer_registry.h"

ProtocolPeerRegistry*
GlobalPeerRegistry::m_registry =
    nullptr;

void GlobalPeerRegistry::setRegistry(
    ProtocolPeerRegistry* registry)
{
    m_registry = registry;
}

ProtocolPeerRegistry*
GlobalPeerRegistry::registry()
{
    return m_registry;
}
