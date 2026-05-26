#include <iostream>
#include <unistd.h>
#include <memory>

#include "core/logger.h"
#include "core/media_output_worker.h"
#include "net/epoll_server.h"
#include "system/timer.h"
#include "util/config.h"
#include "protocol/dstar_session.h"
#include "protocol/protocol_manager.h"
#include "protocol/dstar_protocol.h"
#include "core/jitter_buffer.h"


int main() {

    Config cfg;
    cfg.load("reflector.ini");

    Logger::init("reflector.log");

    Logger::log(INFO, "Reflector starting...");

    MediaOutputWorker::start();

    EpollServer server;
    server.init(cfg.getInt("port", 9000));

   ProtocolManager::registerProtocol(
    ProtocolType::DSTAR,
    std::make_shared<
        DStarProtocol>());

    Timer timer;
    timer.start([&]() {
        server.tick();
        DStarSessionManager::cleanup();
        JitterBuffer::cleanup();
    });

    while (true) {
        sleep(1);
    }

    return 0;
}
