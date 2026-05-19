#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>

int main() {

    int sock =
        socket(AF_INET, SOCK_DGRAM, 0);

    sockaddr_in addr{};

    addr.sin_family = AF_INET;
    addr.sin_port = htons(9000);

    inet_pton(AF_INET,
              "127.0.0.1",
              &addr.sin_addr);

    uint8_t frame[27];

    memset(frame, 0, sizeof(frame));

    memcpy(frame, "DSVT", 4);

    /*
     * Same stream ID
     */
    frame[12] = 0x12;
    frame[13] = 0x34;

    /*
     * Fake voice payload
     */
    for (int i = 14; i < 27; ++i)
        frame[i] = i;

    sendto(sock,
           frame,
           sizeof(frame),
           0,
           reinterpret_cast<sockaddr*>(&addr),
           sizeof(addr));

    std::cout << "Voice frame sent\n";

    close(sock);

    return 0;
}
