#include "protocol_peer_registry.h"

void ProtocolPeerRegistry::addPeer(
    const ProtocolPeer& peer)
{
    m_peers.push_back(peer);
}

std::vector<ProtocolPeer>
ProtocolPeerRegistry::peersForProtocol(
    ProtocolType proto) const
{
    std::vector<ProtocolPeer> result;

    for (const auto& peer : m_peers)
    {
        if (peer.protocol == proto)
        {
            result.push_back(peer);
        }
    }

    return result;
}
