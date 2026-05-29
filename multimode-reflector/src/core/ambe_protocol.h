#pragma once

#include "codec_frame.h"
#include "serial_port.h"

#include <cstdint>
#include <vector>

struct AMBEResponse
{
    bool valid = false;

    uint8_t command = 0;

    std::vector<uint8_t> payload;
};

class AMBEProtocol
{
public:

    static bool softReset(
        SerialPort& port);

    static bool probe(
        SerialPort& port);

    static CodecFrame decode(
        SerialPort& port,
        const CodecFrame& input);

    static CodecFrame encode(
        SerialPort& port,
        const CodecFrame& input);

private:

    static bool sendCommand(
        SerialPort& port,
        const std::vector<uint8_t>& command);

    static bool readResponse(
        SerialPort& port,
        std::vector<uint8_t>& response);

    static bool validatePacket(
        const std::vector<uint8_t>& packet);

    static AMBEResponse parseResponse(
        const std::vector<uint8_t>& packet);
};
