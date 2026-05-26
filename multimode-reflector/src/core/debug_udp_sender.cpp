#include "debug_udp_sender.h"

#include "logger.h"

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

void DebugUdpSender::sendToLocal(
    const uint8_t* data,
    size_t length,
    uint16_t port)
{
    int sock =
        socket(AF_INET, SOCK_DGRAM, 0);

    if (sock < 0) {

        Logger::log(ERROR,
            "DebugUdpSender socket failed");

        return;
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

    inet_pton(
        AF_INET,
        "127.0.0.1",
        &addr.sin_addr);

    sendto(
        sock,
        data,
        length,
        0,
        reinterpret_cast<sockaddr*>(&addr),
        sizeof(addr));

    close(sock);

    Logger::log(INFO,
        "DebugUdpSender sent packet:"
        " PORT=" +
        std::to_string(port) +
        " LEN=" +
        std::to_string(length));
}
