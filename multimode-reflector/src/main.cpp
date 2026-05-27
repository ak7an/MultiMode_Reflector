#include <iostream>
#include <unistd.h>
#include <memory>
#include <csignal>
#include <atomic>

#include "core/logger.h"
#include "core/media_output_worker.h"
#include "net/epoll_server.h"
#include "system/timer.h"
#include "util/config.h"
#include "protocol/dstar_session.h"
#include "protocol/protocol_manager.h"
#include "protocol/dstar_protocol.h"
#include "protocol/ysf_protocol.h"
#include "protocol/dmr_protocol.h"
#include "protocol/ysf_encoder.h"
#include "core/jitter_buffer.h"

static std::atomic<bool> running(true);

static void handleSignal(int) {
    running = false;
}


int main() {

    std::signal(SIGINT, handleSignal);
    std::signal(SIGTERM, handleSignal);

    Config cfg;
    cfg.load("reflector.ini");

    Logger::init("reflector.log");

    Logger::log(INFO, "Reflector starting...");

    YSFEncoder::setFrameMode(
        cfg.getString("ysf_frame_mode", "synthetic"));

    MediaOutputWorker::start(
        cfg.getInt("idle_timeout_ms", 15000),
        cfg.getInt("max_tx_ms", 180000),
        cfg.getInt("debug_output_port", 9001));

    EpollServer server;
    server.init(cfg.getInt("port", 9000));

   ProtocolManager::registerProtocol(
    ProtocolType::DSTAR,
    std::make_shared<
        DStarProtocol>());

   ProtocolManager::registerProtocol(
    ProtocolType::YSF,
    std::make_shared<
        YSFProtocol>());

   ProtocolManager::registerProtocol(
    ProtocolType::DMR,
    std::make_shared<
        DMRProtocol>());

    Timer timer;
    timer.start([&]() {
        server.tick();
        DStarSessionManager::cleanup();
        JitterBuffer::cleanup();
    });

    while (running) {
        sleep(1);
    }

    Logger::log(INFO, "Reflector shutting down...");
    timer.stop();
    MediaOutputWorker::stop();
    sleep(1);

    return 0;
}
