#include "protocol_network_router.h"

#include <unistd.h>
#include <sys/socket.h>

#include "../core/logger.h"
#include "../protocol/protocol_name.h"

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
        ProtocolName::toString(proto) +
        " PEERS=" +
        std::to_string(peers.size()) +
        " LEN=" +
        std::to_string(packet.size()));

    for (const auto& peer : peers)
    {
        int sockfd =
            socket(AF_INET, SOCK_DGRAM, 0);

        if (sockfd < 0)
        {
            Logger::log(ERROR,
                "ProtocolNetworkRouter socket create failed");

            continue;
        }

        ssize_t sent =
            sendto(
                sockfd,
                packet.data(),
                packet.size(),
                0,
                reinterpret_cast<const sockaddr*>(
                    &peer.address),
                sizeof(peer.address));

        if (sent < 0)
        {
            Logger::log(ERROR,
                "ProtocolNetworkRouter send failed: PROTO=" +
                ProtocolName::toString(proto) +
                " HOST=" +
                peer.host +
                " PORT=" +
                std::to_string(peer.port));
        }
        else
        {
            Logger::log(INFO,
                "ProtocolNetworkRouter sent: PROTO=" +
                ProtocolName::toString(proto) +
                " HOST=" +
                peer.host +
                " PORT=" +
                std::to_string(peer.port) +
                " LEN=" +
                std::to_string(sent));
        }

        close(sockfd);
    }
}
