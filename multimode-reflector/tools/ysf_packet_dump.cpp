#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <string>
#include <cstdint>

static int hexValue(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return -1;
}

static bool parseHexByte(const std::string& token, uint8_t& value) {
    if (token.size() != 2) return false;
    int hi = hexValue(token[0]);
    int lo = hexValue(token[1]);
    if (hi < 0 || lo < 0) return false;
    value = static_cast<uint8_t>((hi << 4) | lo);
    return true;
}

static const char* fiName(uint8_t v) {
    switch (v) {
    case 0x00: return "HEADER";
    case 0x01: return "VOICE";
    case 0x02: return "VOICE_EOT";
    default: return "UNKNOWN";
    }
}

static const char* dtName(uint8_t v) {
    switch (v) {
    case 0x02: return "VOICE_FULL_RATE";
    default: return "UNKNOWN";
    }
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: ysf_packet_dump <packet_capture.log>\n";
        return 1;
    }

    std::ifstream in(argv[1]);
    if (!in) {
        std::cerr << "Unable to open file\n";
        return 1;
    }

    std::string line;
    int count = 0;

    while (std::getline(in, line)) {
        std::stringstream ss(line);

        std::string ts;
        std::string proto;
        ss >> ts >> proto;

        std::vector<uint8_t> pkt;
        std::string token;

        while (ss >> token) {
            uint8_t value{};
            if (parseHexByte(token, value)) {
                pkt.push_back(value);
            }
        }

        if (pkt.size() < 35) continue;
        if (!(pkt[0] == 'Y' && pkt[1] == 'S' && pkt[2] == 'F' && pkt[3] == 'D')) continue;

        count++;

        uint16_t streamId = (pkt[4] << 8) | pkt[5];

        std::string src(pkt.begin() + 9, pkt.begin() + 19);
        std::string dst(pkt.begin() + 19, pkt.begin() + 29);

        std::cout << "YSFD packet #" << count << "\n";
        std::cout << "  len       : " << pkt.size() << "\n";
        std::cout << "  streamId  : " << streamId << "\n";
        std::cout << "  sequence  : " << static_cast<int>(pkt[6]) << "\n";
        std::cout << "  eot       : " << static_cast<int>(pkt[7]) << "\n";
        std::cout << "  frameType : " << static_cast<int>(pkt[8]) << "\n";
        std::cout << "  src       : [" << src << "]\n";
        std::cout << "  dst       : [" << dst << "]\n";

        std::cout << "  src hex   : ";
        for (int i = 9; i < 19; ++i) {
            std::cout << std::hex << std::setw(2)
                      << std::setfill('0')
                      << static_cast<int>(pkt[i]) << " ";
        }
        std::cout << std::dec << "\n";

        std::cout << "  dst hex   : ";
        for (int i = 19; i < 29; ++i) {
            std::cout << std::hex << std::setw(2)
                      << std::setfill('0')
                      << static_cast<int>(pkt[i]) << " ";
        }
        std::cout << std::dec << "\n";

        std::cout << "  fich hex  : ";
        for (int i = 29; i < 35; ++i) {
            std::cout << std::hex << std::setw(2)
                      << std::setfill('0')
                      << static_cast<int>(pkt[i]) << " ";
        }
        std::cout << std::dec << "\n";

        std::cout << "  fich FI   : " << fiName(pkt[29]) << "\n";
        std::cout << "  fich DT   : " << dtName(pkt[31]) << "\n";
        std::cout << "  fich CM   : " << static_cast<int>(pkt[32]) << "\n";
        std::cout << "\n";
    }

    std::cout << "Total YSFD packets: " << count << "\n";
    return 0;
}
