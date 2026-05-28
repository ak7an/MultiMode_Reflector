#include "protocol_peer_registry.h"

#include <chrono>

void ProtocolPeerRegistry::addPeer(
    const ProtocolPeer& peer)
{
    m_peers.push_back(peer);
}

std::vector<ProtocolPeer*>
ProtocolPeerRegistry::peersForProtocol(
    ProtocolType proto)
{
    std::vector<ProtocolPeer*> result;

    for (auto& peer : m_peers)
    {
        if (peer.protocol == proto)
        {
            result.push_back(&peer);
        }
    }

    return result;
}

void ProtocolPeerRegistry::markPeerReceived(
    ProtocolType proto,
    const std::string& host,
    int port)
{
    for (auto& peer : m_peers)
    {
        if (peer.protocol == proto &&
            peer.host == host &&
            peer.port == port)
        {
            peer.lastPollReceived =
                std::chrono::steady_clock::now();

            peer.connected = true;

            return;
        }
    }
}
