#include <iostream>
#include <unistd.h>
#include <memory>
#include <csignal>
#include <atomic>

#include "core/logger.h"
#include "core/media_output_worker.h"
#include "net/epoll_server.h"
#include "net/protocol_listener_registry.h"
#include "net/xlxd_peer_registrar.h"
#include "net/protocol_peer_registry.h"
#include "system/timer.h"
#include "util/config.h"
#include "protocol/dstar_session.h"
#include "protocol/protocol_manager.h"
#include "protocol/protocol_name.h"
#include "protocol/dstar_protocol.h"
#include "protocol/ysf_protocol.h"
#include "protocol/dmr_protocol.h"
#include "protocol/ysf_encoder.h"
#include "core/jitter_buffer.h"
#include "core/protocol_config.h"
#include "core/protocol_ports.h"
#include "core/xlxd_peer_config.h"

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
    Logger::log(INFO,
        "XLXD peer config: ENABLED=" +
        std::to_string(
            XLXDPeerConfig::enabled()) +
        " REFLECTOR=" +
        XLXDPeerConfig::reflector() +
        " HOST=" +
        XLXDPeerConfig::host() +
        " PORT=" +
        std::to_string(
            XLXDPeerConfig::port()) +
        " MODULE=" +
        std::string(
            1,
            XLXDPeerConfig::module()));


    YSFEncoder::setFrameMode(
        cfg.getString("ysf_frame_mode", "synthetic"));

    ProtocolConfig::setDStarEnabled(
        cfg.getInt("dstar_enabled", 1) != 0);

    ProtocolConfig::setYSFEnabled(
        cfg.getInt("ysf_enabled", 1) != 0);

    ProtocolConfig::setDMREnabled(
        cfg.getInt("dmr_enabled", 1) != 0);

    ProtocolPorts::setDStarPort(
        cfg.getInt("dstar_port", 9000));

    ProtocolPorts::setYSFPort(
        cfg.getInt("ysf_port", 42000));

    ProtocolPorts::setDMRPort(
        cfg.getInt("dmr_port", 62031));

    Logger::log(INFO,
        "Protocol ports:"
        " DSTAR=" +
        std::to_string(ProtocolPorts::dstarPort()) +
        " YSF=" +
        std::to_string(ProtocolPorts::ysfPort()) +
        " DMR=" +
        std::to_string(ProtocolPorts::dmrPort()));

    for (const auto& listener :
         ProtocolListenerRegistry::listeners())
    {
        Logger::log(INFO,
            "Configured protocol listener:"
            " PROTO=" +
            ProtocolName::toString(
                listener.protocol) +
            " PORT=" +
            std::to_string(
                listener.port));
    }

    ProtocolPeerRegistry peerRegistry;

    XLXDPeerRegistrar::registerConfiguredPeer(
        peerRegistry);

    MediaOutputWorker::start(
        cfg.getInt("idle_timeout_ms", 15000),
        cfg.getInt("max_tx_ms", 180000),
        cfg.getInt("debug_output_port", 9001));

    EpollServer server;
    server.init(cfg.getInt("port", 9000));

   if (ProtocolConfig::dstarEnabled()) {
       ProtocolManager::registerProtocol(
        ProtocolType::DSTAR,
        std::make_shared<
            DStarProtocol>());
   }

   if (ProtocolConfig::ysfEnabled()) {
       ProtocolManager::registerProtocol(
        ProtocolType::YSF,
        std::make_shared<
            YSFProtocol>());
   }

   if (ProtocolConfig::dmrEnabled()) {
       ProtocolManager::registerProtocol(
        ProtocolType::DMR,
        std::make_shared<
            DMRProtocol>());
   }

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
