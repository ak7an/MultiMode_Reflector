#pragma once

#include <unordered_map>
#include <string>
#include <netinet/in.h>

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

    std::unordered_map<std::string, ClientSession> m_sessions;

    std::string addrToString(const sockaddr_in& addr);
    void handlePacket();
};
