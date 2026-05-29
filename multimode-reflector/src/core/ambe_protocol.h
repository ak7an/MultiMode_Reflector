#pragma once

#include "codec_frame.h"
#include "serial_port.h"

class AMBEProtocol
{
public:

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
};
