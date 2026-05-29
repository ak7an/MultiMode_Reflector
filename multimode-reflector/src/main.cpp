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
#include "net/xlxd_peer_monitor.h"
#include "net/xlxd_peer_listener.h"
#include "net/protocol_peer_registry.h"
#include "net/global_protocol_router.h"
#include "net/global_peer_registry.h"
#include "net/protocol_network_router.h"
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
#include "core/route_config.h"
#include "core/protocol_ports.h"
#include "core/xlxd_peer_config.h"
#include "core/ambe_device_manager.h"

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

    AMBEDeviceManager::initialize(
        cfg.getString(
            "ambe_decode_device",
            "/dev/ttyUSB0"),
        cfg.getString(
            "ambe_encode_device",
            "/dev/ttyUSB1"),
        cfg.getInt(
            "ambe_baud",
            460800));

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

    ProtocolConfig::setNXDNEnabled(
        cfg.getInt("nxdn_enabled", 0) != 0);

    ProtocolConfig::setP25Enabled(
        cfg.getInt("p25_enabled", 0) != 0);

    ProtocolConfig::setM17Enabled(
        cfg.getInt("m17_enabled", 0) != 0);




    RouteConfig::setReflector(
        cfg.getString(
            "route_source_reflector",
            cfg.getString(
                "xlxd_peer_reflector",
                "XLX999")));

    RouteConfig::setModule(
        cfg.getString(
            "route_source_module",
            cfg.getString(
                "xlxd_peer_module",
                "A"))[0]);

    RouteConfig::setYSFEnabled(
        cfg.getInt(
            "route_enable_ysf",
            1) != 0);

    RouteConfig::setDMREnabled(
        cfg.getInt(
            "route_enable_dmr",
            1) != 0);

    RouteConfig::setNXDNEnabled(
        cfg.getInt(
            "route_enable_nxdn",
            0) != 0);

    RouteConfig::setP25Enabled(
        cfg.getInt(
            "route_enable_p25",
            0) != 0);

    RouteConfig::setM17Enabled(
        cfg.getInt(
            "route_enable_m17",
            0) != 0);

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

    Logger::log(INFO,
        "Protocol enabled:"
        " DSTAR=" +
        std::to_string(ProtocolConfig::dstarEnabled()) +
        " YSF=" +
        std::to_string(ProtocolConfig::ysfEnabled()) +
        " DMR=" +
        std::to_string(ProtocolConfig::dmrEnabled()) +
        " NXDN=" +
        std::to_string(ProtocolConfig::nxdnEnabled()) +
        " P25=" +
        std::to_string(ProtocolConfig::p25Enabled()) +
        " M17=" +
        std::to_string(ProtocolConfig::m17Enabled()));

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

    GlobalPeerRegistry::setRegistry(
        &peerRegistry);

    XLXDPeerRegistrar::registerConfiguredPeer(
        peerRegistry);

    ProtocolNetworkRouter
        protocolRouter(
            peerRegistry);

    GlobalProtocolRouter::setRouter(
        &protocolRouter);

    if (XLXDPeerConfig::enabled())
    {
        XLXDPeerListener::start();
        XLXDPeerMonitor::start();
    }

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

    if (XLXDPeerConfig::enabled())
    {
        XLXDPeerMonitor::stop();
        XLXDPeerListener::stop();
    }

    MediaOutputWorker::stop();

    sleep(1);

    return 0;
}
