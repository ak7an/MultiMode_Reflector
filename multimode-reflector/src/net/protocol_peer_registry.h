#pragma once

#include <vector>

#include "protocol_peer.h"

class ProtocolPeerRegistry
{
public:

    void addPeer(
        const ProtocolPeer& peer);

    std::vector<ProtocolPeer>
    peersForProtocol(
        ProtocolType proto) const;

private:

    std::vector<ProtocolPeer> m_peers;
};
