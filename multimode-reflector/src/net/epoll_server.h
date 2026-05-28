#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include <netinet/in.h>
#include "listener_socket.h"

struct ClientSession {
    sockaddr_in addr;
    uint64_t packets;
};

class EpollServer {
public:
    EpollServer();
    ~EpollServer();

    bool init(int port);
    void tick();

private:
    int m_socket;
    int m_epoll;

    std::vector<ListenerSocket> m_listeners;

    std::unordered_map<std::string, ClientSession> m_sessions;

    std::string addrToString(const sockaddr_in& addr);
    void handlePacket(int socketFd);
};
