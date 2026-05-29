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
#include "../core/media_output_queue.h"
#include "../core/media_frame_type.h"
#include "global_peer_registry.h"
#include "xlxd_poll_packet.h"
#include "xlxd_handshake_packet.h"
#include "xlxd_frame_packet.h"

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

            XLXDPollData pollData;

            if (XLXDPollPacket::parse(
                    buffer,
                    static_cast<size_t>(received),
                    pollData))
            {
                Logger::log(INFO,
                    "XLXD peer poll received: REFLECTOR=" +
                    pollData.reflector +
                    " MODULE=" +
                    std::string(1, pollData.module));

                auto* registry =
                    GlobalPeerRegistry::registry();

                if (registry != nullptr)
                {
                    registry->markPeerReceivedValidated(
                        ProtocolType::DSTAR,
                        host,
                        port,
                        pollData.reflector,
                        pollData.module);
                }
            }
            else
            {
                XLXDHandshakeData handshakeData;

                if (XLXDHandshakePacket::parse(
                        buffer,
                        static_cast<size_t>(received),
                        handshakeData))
                {
                    Logger::log(INFO,
                        "XLXD peer handshake received: REFLECTOR=" +
                        handshakeData.reflector +
                        " MODULE=" +
                        std::string(1, handshakeData.module) +
                        " VERSION=" +
                        std::to_string(
                            handshakeData.protocolVersion));

                    auto* registry =
                        GlobalPeerRegistry::registry();

                    if (registry != nullptr)
                    {
                        if (registry->markPeerReceivedValidated(
                                ProtocolType::DSTAR,
                                host,
                                port,
                                handshakeData.reflector,
                                handshakeData.module))
                        {
                            registry->establishSession(
                                ProtocolType::DSTAR,
                                host,
                                handshakeData.reflector,
                                handshakeData.module,
                                handshakeData.protocolVersion);

                            Logger::log(INFO,
                                "XLXD peer session established: " +
                                handshakeData.reflector +
                                "/" +
                                std::string(1, handshakeData.module));
                        }
                    }
                }
                else
                {
                    XLXDFrameData frameData;

                    if (XLXDFramePacket::parse(
                            buffer,
                            static_cast<size_t>(received),
                            frameData))
                    {
                        auto* registry =
                            GlobalPeerRegistry::registry();

                        bool accepted = false;

                        if (registry != nullptr)
                        {
                            accepted =
                                registry->isSessionEstablished(
                                    ProtocolType::DSTAR,
                                    host,
                                    frameData.reflector,
                                    frameData.module);
                        }

                        if (accepted)
                        {
                            Logger::log(INFO,
                                "XLXD frame accepted: REFLECTOR=" +
                                frameData.reflector +
                                " MODULE=" +
                                std::string(1, frameData.module) +
                                " PAYLOAD_LEN=" +
                                std::to_string(
                                    frameData.payload.size()));

                            MediaFrame mediaFrame;
                            mediaFrame.protocol =
                                MediaProtocol::DSTAR;
                            mediaFrame.frameType =
                                MediaFrameType::VOICE;
                            mediaFrame.streamId =
                                0;
                            mediaFrame.sourceCallsign =
                                frameData.reflector;
                            mediaFrame.sequence =
                                0;
                            mediaFrame.endOfTransmission =
                                false;
                            mediaFrame.sourcePeer =
                                host + ":" +
                                std::to_string(port);
                            mediaFrame.payload =
                                frameData.payload;
                            mediaFrame.createdAt =
                                std::chrono::steady_clock::now();

                            MediaOutputQueue::push(
                                mediaFrame);

                            Logger::log(INFO,
                                "XLXD frame queued to MediaOutputQueue: REFLECTOR=" +
                                frameData.reflector +
                                " MODULE=" +
                                std::string(1, frameData.module) +
                                " PAYLOAD_LEN=" +
                                std::to_string(
                                    frameData.payload.size()));
                        }
                        else
                        {
                            Logger::log(WARN,
                                "XLXD frame rejected: REFLECTOR=" +
                                frameData.reflector +
                                " MODULE=" +
                                std::string(1, frameData.module) +
                                " REASON=session not established");
                        }
                    }
                    else
                    {
                        Logger::log(INFO,
                            "XLXD packet ignored: FROM=" +
                            host + ":" +
                            std::to_string(port) +
                            " LEN=" +
                            std::to_string(received));
                    }
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
