#include "xlxd_peer_monitor.h"

#include <thread>
#include <atomic>
#include <chrono>

#include "../core/logger.h"
#include "../core/xlxd_peer_config.h"
#include "xlxd_poll_packet.h"
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

        auto packet =
            XLXDPollPacket::build();

        auto* router =
            GlobalProtocolRouter::router();

        if (router != nullptr)
        {
            router->routePacket(
                ProtocolType::DSTAR,
                packet);
        }

        std::this_thread::sleep_for(
            std::chrono::milliseconds(
                XLXDPeerConfig::pollIntervalMs()));
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
