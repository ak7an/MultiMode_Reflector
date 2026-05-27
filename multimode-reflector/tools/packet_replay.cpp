#include <arpa/inet.h>
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>
#include <sys/socket.h>
#include <unistd.h>

static int hexValue(char c)
{
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return -1;
}

static bool parseHexByte(const std::string& token, uint8_t& value)
{
    if (token.size() != 2) return false;

    int hi = hexValue(token[0]);
    int lo = hexValue(token[1]);

    if (hi < 0 || lo < 0) return false;

    value = static_cast<uint8_t>((hi << 4) | lo);
    return true;
}

int main(int argc, char** argv)
{
    if (argc < 4) {
        std::cerr << "Usage: " << argv[0]
                  << " <capture.log> <host> <port> [speed]\n";
        return 1;
    }

    std::ifstream in(argv[1]);
    if (!in) {
        std::cerr << "Unable to open " << argv[1] << "\n";
        return 1;
    }

    int port = std::atoi(argv[3]);
    double speed = argc >= 5 ? std::atof(argv[4]) : 1.0;
    if (speed <= 0.0) speed = 1.0;

    int sock = socket(AF_INET, SOCK_DGRAM, 0);

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, argv[2], &addr.sin_addr);

    std::string line;
    long long previousTs = -1;
    size_t sent = 0;

    while (std::getline(in, line)) {
        std::stringstream ss(line);

        std::string tsToken;
        std::string proto;
        ss >> tsToken >> proto;

        long long ts = std::stoll(tsToken, nullptr, 16);

        if (previousTs >= 0) {
            long long delay = static_cast<long long>((ts - previousTs) / speed);

            if (delay > 0 && delay < 5000) {
                std::this_thread::sleep_for(
                    std::chrono::milliseconds(delay));
            }
        }

        previousTs = ts;

        std::vector<uint8_t> packet;
        std::string token;

        while (ss >> token) {
            uint8_t value{};
            if (parseHexByte(token, value)) {
                packet.push_back(value);
            }
        }

        if (packet.empty()) continue;

        sendto(sock, packet.data(), packet.size(), 0,
               reinterpret_cast<sockaddr*>(&addr), sizeof(addr));

        ++sent;
        std::cout << "Replayed " << proto
                  << " packet len=" << packet.size() << "\n";
    }

    close(sock);

    std::cout << "Replay complete. Packets sent=" << sent << "\n";
    return 0;
}
