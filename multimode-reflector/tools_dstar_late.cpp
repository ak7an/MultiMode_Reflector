#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>

static void sendVoice(int sock, const sockaddr_in& addr, uint8_t seq)
{
    uint8_t frame[27];
    memset(frame, 0, sizeof(frame));
    memcpy(frame, "DSVT", 4);

    frame[12] = 0x12;
    frame[13] = 0x34;
    frame[14] = seq;

    for (int i = 15; i < 27; ++i)
        frame[i] = i;

    sendto(sock, frame, sizeof(frame), 0,
           reinterpret_cast<const sockaddr*>(&addr),
           sizeof(addr));

    std::cout << "Sent sequence " << static_cast<int>(seq) << "\n";
    usleep(100000);
}

int main()
{
    int sock = socket(AF_INET, SOCK_DGRAM, 0);

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9000);

    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

    sendVoice(sock, addr, 0);
    sendVoice(sock, addr, 1);
    sendVoice(sock, addr, 2);
    sendVoice(sock, addr, 4);
    sendVoice(sock, addr, 3);

    close(sock);
    return 0;
}
