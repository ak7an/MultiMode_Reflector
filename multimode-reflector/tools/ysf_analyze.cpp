#include <cstdint>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>



static std::string frameTypeName(uint8_t code)
{
    switch (code) {

    case 1:
        return "HEADER";

    case 2:
        return "VOICE";

    case 3:
        return "VOICE_EOT";

    case 4:
        return "CONTROL";

    default:
        return "UNKNOWN";
    }
}

static int hexValue(char c)
{
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return -1;
}

int main(int argc, char** argv)
{
    if (argc < 2) {

        std::cerr << "Usage: "
                  << argv[0]
                  << " \"59 53 46 42 ...\"\n";

        return 1;
    }

    std::string input =
        argv[1];

    std::stringstream ss(input);
    std::string token;
    std::vector<uint8_t> data;

    while (ss >> token) {

        if (token.size() != 2)
            continue;

        int hi = hexValue(token[0]);
        int lo = hexValue(token[1]);

        if (hi < 0 || lo < 0)
            continue;

        data.push_back(
            static_cast<uint8_t>(
                (hi << 4) | lo));
    }

    if (data.size() < 14) {

        std::cerr << "Packet too short\n";
        return 1;
    }

    std::string magic;
    magic.push_back(data[0]);
    magic.push_back(data[1]);
    magic.push_back(data[2]);
    magic.push_back(data[3]);

    uint16_t streamId =
        (data[4] << 8) |
         data[5];

    uint8_t sequence =
        data[6];

    bool eot =
        data[7] != 0;

    std::string source;
    source.push_back(data[8]);
    source.push_back(data[9]);
    source.push_back(data[10]);
    source.push_back(data[11]);

    uint16_t payloadLength =
        (data[12] << 8) |
         data[13];

    std::cout << "Magic: " << magic << "\n";
    std::cout << "Stream ID: " << streamId << "\n";
    std::cout << "Sequence: " << static_cast<int>(sequence) << "\n";
    std::cout << "EOT: " << (eot ? "yes" : "no") << "\n";
    std::cout << "Source marker: " << source << "\n";
    uint8_t frameType =
        data.size() > 14 ? data[14] : 0;

    std::cout << "Payload length: " << payloadLength << "\n";
    std::cout << "Frame type: " << frameTypeName(frameType)
              << " (" << static_cast<int>(frameType) << ")\n";
    std::cout << "Packet bytes: " << data.size() << "\n";

    return 0;
}
