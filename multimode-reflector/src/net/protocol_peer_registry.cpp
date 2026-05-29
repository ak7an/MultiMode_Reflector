#include "protocol_peer_registry.h"

#include <chrono>

#include "../core/logger.h"

void ProtocolPeerRegistry::addPeer(
    const ProtocolPeer& peer)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    m_peers.push_back(peer);
}

std::vector<ProtocolPeer*>
ProtocolPeerRegistry::peersForProtocol(
    ProtocolType proto)
{
    std::lock_guard<std::mutex> lock(m_mutex);

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
    markPeerReceivedValidated(
        proto,
        host,
        port,
        "",
        '\0');
}

bool ProtocolPeerRegistry::markPeerReceivedValidated(
    ProtocolType proto,
    const std::string& host,
    int port,
    const std::string& reflector,
    char module)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    for (auto& peer : m_peers)
    {
        if (peer.protocol != proto ||
            peer.host != host)
        {
            continue;
        }

        bool identityCheckRequested =
            !reflector.empty() &&
            module != '\0';

        if (identityCheckRequested &&
            (peer.reflector != reflector ||
             peer.module != module))
        {
            Logger::log(WARN,
                "XLXD peer identity mismatch: EXPECTED=" +
                peer.reflector + "/" +
                std::string(1, peer.module) +
                " RECEIVED=" +
                reflector + "/" +
                std::string(1, module));

            return false;
        }

        peer.lastPollReceived =
            std::chrono::steady_clock::now();

        if (!peer.connected)
        {
            Logger::log(INFO,
                "XLXD peer connected: " +
                peer.host + ":" +
                std::to_string(peer.port));
        }

        if (peer.port != port)
        {
            Logger::log(INFO,
                "XLXD peer poll source port differs: CONFIGURED=" +
                std::to_string(peer.port) +
                " RECEIVED=" +
                std::to_string(port));
        }

        peer.connected = true;

        if (identityCheckRequested)
        {
            Logger::log(INFO,
                "XLXD peer identity validated: " +
                reflector + "/" +
                std::string(1, module));
        }

        return true;
    }

    Logger::log(WARN,
        "XLXD poll received from unknown peer: " +
        host + ":" +
        std::to_string(port));

    return false;
}

void ProtocolPeerRegistry::updatePeerTimeouts(
    ProtocolType proto,
    int timeoutMs)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    auto now =
        std::chrono::steady_clock::now();

    for (auto& peer : m_peers)
    {
        if (peer.protocol != proto)
        {
            continue;
        }

        auto elapsedMs =
            std::chrono::duration_cast<std::chrono::milliseconds>(
                now - peer.lastPollReceived).count();

        if (peer.connected &&
            elapsedMs > timeoutMs)
        {
            peer.connected = false;

            Logger::log(WARN,
                "XLXD peer timed out: " +
                peer.host + ":" +
                std::to_string(peer.port));
        }
    }
}
