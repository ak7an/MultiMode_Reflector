#include <arpa/inet.h>
#include <iomanip>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>

int main()
{
    int sock = socket(AF_INET, SOCK_DGRAM, 0);

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9001);
    addr.sin_addr.s_addr = INADDR_ANY;

    bind(sock,
         reinterpret_cast<sockaddr*>(&addr),
         sizeof(addr));

    std::cout << "YSF listener on UDP 9001" << std::endl;

    uint8_t buffer[512];

    while (true) {

        ssize_t len =
            recv(sock,
                 buffer,
                 sizeof(buffer),
                 0);

        if (len <= 0)
            continue;

        std::cout << "Received "
                  << len
                  << " bytes: ";

        for (ssize_t i = 0;
             i < len;
             ++i)
        {
            std::cout
                << std::hex
                << std::setw(2)
                << std::setfill('0')
                << static_cast<int>(buffer[i])
                << " ";
        }

        std::cout << std::dec << std::endl;
    }

    close(sock);
    return 0;
}
