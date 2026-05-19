#pragma once

#include <string>
#include <unordered_map>
#include <netinet/in.h>
#include <ctime>
#include <cstdint>

struct Peer {

    std::string id;

    sockaddr_in addr;

    time_t lastActivity;
};

class PeerManager {
public:

    static void registerPeer(
        const std::string& id,
        const sockaddr_in& addr);

    static void updateActivity(
        const std::string& id);

    static void cleanup();

    static void broadcastFrame(
        int sockfd,
        const uint8_t* data,
        size_t length,
        const std::string& excludePeer);

private:

    static std::unordered_map<
        std::string,
        Peer> m_peers;
};
