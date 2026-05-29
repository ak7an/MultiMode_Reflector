#include "xlxd_peer_listener.h"

#include <atomic>
#include <thread>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>

#include "../core/logger.h"
#include "../core/xlxd_peer_config.h"
#include "global_peer_registry.h"

static std::atomic<bool> g_peerListenerRunning(false);
static std::thread g_peerListenerThread;

static void listenerThread()
{
    Logger::log(INFO,
        "XLXD peer listener starting");

    int sockfd =
        socket(AF_INET, SOCK_DGRAM, 0);

    if (sockfd < 0)
    {
        Logger::log(ERROR,
            "XLXD peer listener socket failed");
        return;
    }

    sockaddr_in addr {};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port =
        htons(XLXDPeerConfig::port());

    if (bind(sockfd,
             reinterpret_cast<sockaddr*>(&addr),
             sizeof(addr)) < 0)
    {
        Logger::log(ERROR,
            "XLXD peer listener bind failed");

        close(sockfd);
        return;
    }

    Logger::log(INFO,
        "XLXD peer listener active: PORT=" +
        std::to_string(XLXDPeerConfig::port()));

    while (g_peerListenerRunning)
    {
        uint8_t buffer[2048];
        sockaddr_in from {};
        socklen_t fromLen = sizeof(from);

        ssize_t received =
            recvfrom(
                sockfd,
                buffer,
                sizeof(buffer),
                MSG_DONTWAIT,
                reinterpret_cast<sockaddr*>(&from),
                &fromLen);

        if (received > 0)
        {
            char hostBuffer[INET_ADDRSTRLEN] {};
            inet_ntop(
                AF_INET,
                &(from.sin_addr),
                hostBuffer,
                sizeof(hostBuffer));

            std::string host(hostBuffer);
            int port =
                ntohs(from.sin_port);

            Logger::log(INFO,
                "XLXD peer packet received: FROM=" +
                host + ":" +
                std::to_string(port) +
                " LEN=" +
                std::to_string(received));

            if (received >= 4 &&
                std::memcmp(buffer, "XLXP", 4) == 0)
            {
                Logger::log(INFO,
                    "XLXD peer poll received");

                auto* registry =
                    GlobalPeerRegistry::registry();

                if (registry != nullptr)
                {
                    registry->markPeerReceived(
                        ProtocolType::DSTAR,
                        host,
                        port);
                }
            }
        }

        usleep(10000);
    }

    close(sockfd);

    Logger::log(INFO,
        "XLXD peer listener stopped");
}

void XLXDPeerListener::start()
{
    if (g_peerListenerRunning)
    {
        return;
    }

    g_peerListenerRunning = true;

    g_peerListenerThread =
        std::thread(listenerThread);
}

void XLXDPeerListener::stop()
{
    if (!g_peerListenerRunning)
    {
        return;
    }

    g_peerListenerRunning = false;

    if (g_peerListenerThread.joinable())
    {
        g_peerListenerThread.join();
    }
}
