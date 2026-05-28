#pragma once

#include <vector>

#include "protocol_peer.h"

class ProtocolPeerRegistry
{
public:

    void addPeer(
        const ProtocolPeer& peer);

    std::vector<ProtocolPeer*>
    peersForProtocol(
        ProtocolType proto);

    void markPeerReceived(
        ProtocolType proto,
        const std::string& host,
        int port);

private:

    std::vector<ProtocolPeer> m_peers;
};
