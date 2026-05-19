#include "peer_manager.h"

#include "logger.h"

#include <sys/socket.h>
#include <arpa/inet.h>

std::unordered_map<
    std::string,
    Peer>
PeerManager::m_peers;

void PeerManager::registerPeer(
    const std::string& id,
    const sockaddr_in& addr)
{
    bool isNew =
        !m_peers.count(id);

    Peer& p =
        m_peers[id];

    p.id = id;
    p.addr = addr;
    p.lastActivity = time(nullptr);

    if (isNew) {

        Logger::log(INFO,
            "Peer registered: " + id);
    }
}

void PeerManager::updateActivity(
    const std::string& id)
{
    if (m_peers.count(id)) {

        m_peers[id].lastActivity =
            time(nullptr);
    }
}

void PeerManager::cleanup()
{
    time_t now = time(nullptr);

    for (auto it = m_peers.begin();
         it != m_peers.end();)
    {
        if ((now - it->second.lastActivity) > 60) {

            Logger::log(INFO,
                "Peer timeout: " +
                it->first);

            it = m_peers.erase(it);
        }
        else {
            ++it;
        }
    }
}

void PeerManager::broadcastFrame(
    int sockfd,
    const uint8_t* data,
    size_t length,
    const std::string& excludePeer)
{
    for (const auto& [id, peer] : m_peers) {

        if (id == excludePeer)
            continue;

        sendto(sockfd,
               data,
               length,
               0,
               reinterpret_cast<
                   const sockaddr*>(
                       &peer.addr),
               sizeof(peer.addr));

        Logger::log(INFO,
            "Forwarded frame to peer: " +
            id);
    }
}
