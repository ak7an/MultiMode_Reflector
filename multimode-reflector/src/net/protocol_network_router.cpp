#include "protocol_network_router.h"

#include "../core/logger.h"

ProtocolNetworkRouter::ProtocolNetworkRouter(
    ProtocolPeerRegistry& registry) :
    m_registry(registry)
{
}

void ProtocolNetworkRouter::routePacket(
    ProtocolType proto,
    const std::vector<uint8_t>& packet)
{
    auto peers =
        m_registry.peersForProtocol(proto);

    Logger::log(INFO,
        "ProtocolNetworkRouter route: PROTO=" +
        std::to_string(static_cast<int>(proto)) +
        " PEERS=" +
        std::to_string(peers.size()) +
        " LEN=" +
        std::to_string(packet.size()));
}
