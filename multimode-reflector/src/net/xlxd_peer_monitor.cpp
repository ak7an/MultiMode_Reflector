#include "xlxd_peer_monitor.h"

#include <thread>
#include <atomic>
#include <chrono>

#include "../core/logger.h"
#include "../core/xlxd_peer_config.h"

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
