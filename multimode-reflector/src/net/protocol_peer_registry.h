#pragma once

#include <vector>
#include <string>
#include <mutex>
#include <cstdint>

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

    bool markPeerReceivedValidated(
        ProtocolType proto,
        const std::string& host,
        int port,
        const std::string& reflector,
        char module);

    bool establishSession(
        ProtocolType proto,
        const std::string& host,
        const std::string& reflector,
        char module,
        uint32_t protocolVersion);

    bool isSessionEstablished(
        ProtocolType proto,
        const std::string& host,
        const std::string& reflector,
        char module);

    void updatePeerTimeouts(
        ProtocolType proto,
        int timeoutMs);

private:

    std::vector<ProtocolPeer> m_peers;
    std::mutex m_mutex;
};
