#include <arpa/inet.h>
#include <chrono>
#include <cstring>
#include <iostream>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>

static void sendPacket(
    int sock,
    const sockaddr_in& addr,
    const uint8_t* data,
    size_t len)
{
    sendto(
        sock,
        data,
        len,
        0,
        reinterpret_cast<const sockaddr*>(&addr),
        sizeof(addr));
}

int main()
{
    int sock =
        socket(AF_INET, SOCK_DGRAM, 0);

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9000);

    inet_pton(
        AF_INET,
        "127.0.0.1",
        &addr.sin_addr);

    uint8_t header[56]{};

    header[0] = 'D';
    header[1] = 'S';
    header[2] = 'V';
    header[3] = 'T';

    header[12] = 0x12;
    header[13] = 0x34;

    const char* mycall =
        "KD0CVD  ";

    std::memcpy(
        &header[18],
        "CQCQCQ  ",
        8);

    std::memcpy(
        &header[26],
        "AK7AN B ",
        8);

    std::memcpy(
        &header[34],
        "AK7AN G ",
        8);

    std::memcpy(
        &header[42],
        mycall,
        8);

    std::cout << "Sending D-Star header\n";

    sendPacket(
        sock,
        addr,
        header,
        sizeof(header));

    std::this_thread::sleep_for(
        std::chrono::milliseconds(20));

    for (int i = 0;
         i < 20;
         ++i)
    {
        uint8_t voice[27]{};

        voice[0] = 'D';
        voice[1] = 'S';
        voice[2] = 'V';
        voice[3] = 'T';

        voice[12] = 0x12;
        voice[13] = 0x34;

        voice[14] =
            static_cast<uint8_t>(
                i & 0x1F);

        if (i == 19) {

            voice[14] |= 0x40;
        }

        std::cout
            << "Burst voice seq "
            << (i & 0x1F)
            << (i == 19 ? " EOT" : "")
            << "\n";

        sendPacket(
            sock,
            addr,
            voice,
            sizeof(voice));

        // intentionally too fast
        std::this_thread::sleep_for(
            std::chrono::milliseconds(2));
    }

    close(sock);
    return 0;
}
