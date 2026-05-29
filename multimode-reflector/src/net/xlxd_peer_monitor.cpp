#include "xlxd_peer_monitor.h"

#include <thread>
#include <atomic>
#include <chrono>

#include "../core/logger.h"
#include "../core/xlxd_peer_config.h"
#include "xlxd_poll_packet.h"
#include "xlxd_handshake_packet.h"
#include "global_protocol_router.h"
#include "global_peer_registry.h"

static std::atomic<bool>
    g_running(false);

static std::thread
    g_thread;

static void monitorThread()
{
    Logger::log(INFO,
        "XLXD peer monitor started");

    while (g_running)
    {
        Logger::log(INFO,
            "XLXD peer poll tick");

        auto* registry =
            GlobalPeerRegistry::registry();

        if (registry != nullptr)
        {
            registry->updatePeerTimeouts(
                ProtocolType::DSTAR,
                XLXDPeerConfig::timeoutMs());
        }

        XLXDPollData pollData;
        pollData.reflector =
            XLXDPeerConfig::reflector();
        pollData.module =
            XLXDPeerConfig::module();

        auto packet =
            XLXDPollPacket::build(
                pollData);

        XLXDHandshakeData handshakeData;
        handshakeData.reflector =
            XLXDPeerConfig::reflector();
        handshakeData.module =
            XLXDPeerConfig::module();
        handshakeData.protocolVersion =
            1;

        auto handshakePacket =
            XLXDHandshakePacket::build(
                handshakeData);

        auto* router =
            GlobalProtocolRouter::router();

        if (router != nullptr)
        {
            router->routePacket(
                ProtocolType::DSTAR,
                packet);

            router->routePacket(
                ProtocolType::DSTAR,
                handshakePacket);
        }

        int sleptMs = 0;
        int intervalMs =
            XLXDPeerConfig::pollIntervalMs();

        while (g_running &&
               sleptMs < intervalMs)
        {
            std::this_thread::sleep_for(
                std::chrono::milliseconds(100));

            sleptMs += 100;
        }
    }

    Logger::log(INFO,
        "XLXD peer monitor stopped");
}

void XLXDPeerMonitor::start()
{
    if (g_running)
    {
        return;
    }

    g_running = true;

    g_thread =
        std::thread(
            monitorThread);
}

void XLXDPeerMonitor::stop()
{
    if (!g_running)
    {
        return;
    }

    g_running = false;

    if (g_thread.joinable())
    {
        g_thread.join();
    }
}
