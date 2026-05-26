#include <arpa/inet.h>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <sys/socket.h>
#include <unistd.h>

static std::string timestamp()
{
    auto now =
        std::chrono::system_clock::now();

    auto t =
        std::chrono::system_clock::to_time_t(now);

    std::tm* tm =
        std::localtime(&t);

    std::ostringstream ss;

    ss << std::put_time(
        tm,
        "%Y-%m-%d %H:%M:%S");

    return ss.str();
}

int main()
{
    int sock =
        socket(AF_INET, SOCK_DGRAM, 0);

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9001);
    addr.sin_addr.s_addr = INADDR_ANY;

    bind(sock,
         reinterpret_cast<sockaddr*>(&addr),
         sizeof(addr));

    std::cout
        << "YSF listener on UDP 9001"
        << std::endl;

    uint8_t buffer[512];

    while (true) {

        ssize_t len =
            recv(sock,
                 buffer,
                 sizeof(buffer),
                 0);

        if (len <= 0)
            continue;

        std::ostringstream hex;

        for (ssize_t i = 0;
             i < len;
             ++i)
        {
            hex
                << std::hex
                << std::setw(2)
                << std::setfill('0')
                << static_cast<int>(buffer[i]);

            if (i + 1 < len)
                hex << " ";
        }

        std::cout
            << timestamp()
            << " | LEN="
            << len
            << " | HEX="
            << hex.str()
            << std::endl;
    }

    close(sock);
    return 0;
}
