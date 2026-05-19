#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>

int main() {

    int sock =
        socket(AF_INET, SOCK_DGRAM, 0);

    if (sock < 0) {

        std::cerr << "socket failed\n";
        return 1;
    }

    sockaddr_in addr{};

    addr.sin_family = AF_INET;
    addr.sin_port = htons(40000);

    inet_pton(
        AF_INET,
        "0.0.0.0",
        &addr.sin_addr);

    if (bind(
            sock,
            reinterpret_cast<sockaddr*>(&addr),
            sizeof(addr)) < 0)
    {
        std::cerr << "bind failed\n";
        return 1;
    }

    std::cout
        << "Listening on UDP 40000...\n";
    /*
     * Register with reflector
     */

    sockaddr_in reflector{};

    reflector.sin_family = AF_INET;
    reflector.sin_port = htons(9000);

    inet_pton(
        AF_INET,
        "127.0.0.1",
        &reflector.sin_addr);

    uint8_t reg[4] = {
        'D','S','V','T'
    };

    sendto(
        sock,
        reg,
        sizeof(reg),
        0,
        reinterpret_cast<
            sockaddr*>(&reflector),
        sizeof(reflector));

    while (true) {

        uint8_t buffer[2048];

        sockaddr_in client{};
        socklen_t len =
            sizeof(client);

        ssize_t received =
            recvfrom(
                sock,
                buffer,
                sizeof(buffer),
                0,
                reinterpret_cast<
                    sockaddr*>(&client),
                &len);

        if (received <= 0)
            continue;

        char ip[INET_ADDRSTRLEN];

        inet_ntop(
            AF_INET,
            &client.sin_addr,
            ip,
            sizeof(ip));

        std::cout
            << "Received forwarded frame from "
            << ip
            << ":"
            << ntohs(client.sin_port)
            << " size="
            << received
            << "\n";
    }

    close(sock);

    return 0;
}
