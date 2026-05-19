#include "epoll_server.h"
#include "../core/logger.h"

void EpollServer::init(int port) {
    Logger::log(INFO, "Epoll initialized");
}

void EpollServer::tick() {}
