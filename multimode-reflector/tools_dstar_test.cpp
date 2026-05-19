#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>

static void writeField(
    uint8_t* buf,
    size_t offset,
    const std::string& text)
{
    memset(buf + offset, ' ', 8);

    memcpy(buf + offset,
           text.c_str(),
           std::min((size_t)8, text.size()));
}

int main() {

    int sock =
        socket(AF_INET, SOCK_DGRAM, 0);

    if (sock < 0) {
        std::cerr << "socket failed\n";
        return 1;
    }

    sockaddr_in addr{};

    addr.sin_family = AF_INET;
    addr.sin_port = htons(9000);

    inet_pton(AF_INET,
              "127.0.0.1",
              &addr.sin_addr);

    uint8_t frame[56];

    memset(frame, 0, sizeof(frame));

    /*
     * DSVT signature
     */
    memcpy(frame, "DSVT", 4);

    /*
     * Fake stream ID
     */
    frame[12] = 0x12;
    frame[13] = 0x34;

    /*
     * Callsigns
     */
    writeField(frame, 18, "AK7AN G");
    writeField(frame, 26, "AK7AN B");
    writeField(frame, 34, "CQCQCQ");
    writeField(frame, 42, "KD0CVD");

    ssize_t sent =
        sendto(sock,
               frame,
               sizeof(frame),
               0,
               reinterpret_cast<sockaddr*>(&addr),
               sizeof(addr));

    std::cout << "Sent "
              << sent
              << " byte D-Star frame\n";

    close(sock);

    return 0;
}
