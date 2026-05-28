#include "epoll_server.h"
#include "../protocol/protocol_detector.h"
#include "../protocol/dstar_protocol.h"
#include "../protocol/ysf_protocol.h"
#include "../core/peer_manager.h"
#include "../core/logger.h"
#include "../core/config_manager.h"
#include "../protocol/protocol_manager.h"
#include "../core/media_output_queue.h"
#include "../protocol/protocol_interface.h"
#include "../protocol/protocol_encoder_capabilities.h"

#include <unistd.h>
#include <chrono>
#include <fcntl.h>
#include <cstring>
#include <iostream>

#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>

EpollServer::EpollServer() :
    m_socket(-1),
    m_epoll(-1)
{
}

EpollServer::~EpollServer() {

    if (m_socket >= 0)
        close(m_socket);

    if (m_epoll >= 0)
        close(m_epoll);
}

bool EpollServer::init(int port) {

    m_socket = socket(AF_INET, SOCK_DGRAM, 0);

    if (m_socket < 0) {
        Logger::log(ERROR, "Failed to create UDP socket");
        return false;
    }

    int flags = fcntl(m_socket, F_GETFL, 0);
    fcntl(m_socket, F_SETFL, flags | O_NONBLOCK);

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    if (bind(m_socket,
             reinterpret_cast<sockaddr*>(&addr),
             sizeof(addr)) < 0)
    {
        Logger::log(ERROR, "Failed to bind UDP socket");
        return false;
    }

    m_epoll = epoll_create1(0);

    if (m_epoll < 0) {
        Logger::log(ERROR, "Failed to create epoll instance");
        return false;
    }

    epoll_event ev{};
    ev.events = EPOLLIN;
    ev.data.fd = m_socket;

    if (epoll_ctl(m_epoll,
                  EPOLL_CTL_ADD,
                  m_socket,
                  &ev) < 0)
    {
        Logger::log(ERROR, "Failed to add socket to epoll");
        return false;
    }

    Logger::log(INFO,
        "UDP listener active on port " +
        std::to_string(port));

    return true;
}

void EpollServer::tick() {

    epoll_event events[16];

    int count = epoll_wait(m_epoll,
                           events,
                           16,
                           0);

    for (int i = 0; i < count; ++i) {

        if (events[i].data.fd == m_socket) {
            handlePacket(
                events[i].data.fd);
        }
    }

    PeerManager::cleanupInactivePeers();
}

void EpollServer::handlePacket(int socketFd) {

    char buffer[2048];

    sockaddr_in client{};
    socklen_t len = sizeof(client);

    ssize_t received = recvfrom(
        socketFd,
        buffer,
        sizeof(buffer),
        0,
        reinterpret_cast<sockaddr*>(&client),
        &len
    );

    if (received <= 0)
        return;

    std::string key = addrToString(client);

    if (!m_sessions.count(key)) {

        ClientSession session{};
        session.addr = client;
        session.packets = 0;

        m_sessions[key] = session;

        Logger::log(INFO,
            "New client session: " + key);
    }

PeerManager::registerPeer(
    key,
    client);

PeerManager::updateActivity(
    key);

    m_sessions[key].packets++;

ProtocolType proto =
    ProtocolDetector::detect(
        reinterpret_cast<uint8_t*>(buffer),
        received);

std::string protoName = "UNKNOWN";

switch (proto) {

    case ProtocolType::DSTAR:
        protoName = "DSTAR";
        break;

    case ProtocolType::DMR:
        protoName = "DMR";
        break;

    case ProtocolType::YSF:
        protoName = "YSF";
        break;

    case ProtocolType::NXDN:
        protoName = "NXDN";
        break;

    case ProtocolType::P25:
        protoName = "P25";
        break;

    case ProtocolType::M17:
        protoName = "M17";
        break;

    case ProtocolType::XLX:
        protoName = "XLX";
        break;

    default:
        break;
}

Logger::log(INFO,
    "Packet received from " +
    key +
    " proto=" +
    protoName +
    " size=" +
    std::to_string(received));

ProtocolInterface* handler =
    ProtocolManager::get(proto);

if (handler) {

ProtocolResult result =
    handler->handle(
        reinterpret_cast<uint8_t*>(buffer),
        received,
        key);

if (result.forwardCurrent) {

    PeerManager::broadcastFrame(
        socketFd,
        reinterpret_cast<uint8_t*>(buffer),
        received,
        key);
}

for (const auto& frame :
     result.extraFrames)
{
    PeerManager::broadcastFrame(
        socketFd,
        frame.payload.data(),
        frame.payload.size(),
        key);
}

for (const auto& media :
     result.transcodedFrames)
{
    Logger::log(INFO,
        "Transcoded media output observed:"
        " STREAMID=" +
        std::to_string(media.streamId) +
        " SEQ=" +
        std::to_string(media.sequence));

    if (ProtocolEncoderCapabilities::canEncode(
            media.protocol))
    {
        MediaOutputQueue::push(
            media);
    }
    else {
        Logger::log(INFO,
            "Skipping media output without encoder:"
            " STREAMID=" +
            std::to_string(media.streamId));
    }
}


}

}

std::string EpollServer::addrToString(const sockaddr_in& addr) {

    char ip[INET_ADDRSTRLEN];

    inet_ntop(AF_INET,
              &addr.sin_addr,
              ip,
              sizeof(ip));

    return std::string(ip) +
           ":" +
           std::to_string(ntohs(addr.sin_port));
}
