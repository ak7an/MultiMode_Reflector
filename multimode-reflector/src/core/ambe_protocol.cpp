#include "ambe_protocol.h"

#include "logger.h"
#include "hex_dump.h"

namespace
{
    constexpr uint8_t AMBE_PACKET_START = 0x61;

    constexpr uint8_t CMD_PROBE  = 0x01;
    constexpr uint8_t CMD_DECODE = 0x02;
    constexpr uint8_t CMD_ENCODE = 0x03;
}

static std::vector<uint8_t> buildPacket(
    uint8_t command,
    const std::vector<uint8_t>& payload = {})
{
    std::vector<uint8_t> packet;

    packet.push_back(
        AMBE_PACKET_START);

    uint16_t length =
        static_cast<uint16_t>(
            payload.size() + 1);

    packet.push_back(
        (length >> 8) & 0xff);

    packet.push_back(
        length & 0xff);

    packet.push_back(
        command);

    packet.insert(
        packet.end(),
        payload.begin(),
        payload.end());

    return packet;
}

bool AMBEProtocol::probe(
    SerialPort& port)
{
    if (!port.isOpen())
    {
        Logger::log(INFO,
            "AMBEProtocol probe skipped: port not open");

        return false;
    }

    auto command =
        buildPacket(
            CMD_PROBE);

    Logger::log(INFO,
        "AMBEProtocol probe:"
        " TX_LEN=" +
        std::to_string(command.size()));

    sendCommand(
        port,
        command);

    std::vector<uint8_t> response;

    if (readResponse(
            port,
            response))
    {
        Logger::log(INFO,
            "AMBEProtocol probe response:"
            " RX_LEN=" +
            std::to_string(response.size()));

        return true;
    }

    Logger::log(INFO,
        "AMBEProtocol probe no response");

    return false;
}

CodecFrame AMBEProtocol::decode(
    SerialPort& port,
    const CodecFrame& input)
{
    if (!port.isOpen())
    {
        Logger::log(INFO,
            "AMBEProtocol decode stub: port not open");

        return input;
    }

    auto command =
        buildPacket(
            CMD_DECODE);

    Logger::log(INFO,
        "AMBEProtocol decode:"
        " STREAMID=" +
        std::to_string(input.streamId) +
        " TX_LEN=" +
        std::to_string(command.size()));

    sendCommand(
        port,
        command);

    std::vector<uint8_t> response;

    readResponse(
        port,
        response);

    return input;
}

CodecFrame AMBEProtocol::encode(
    SerialPort& port,
    const CodecFrame& input)
{
    if (!port.isOpen())
    {
        Logger::log(INFO,
            "AMBEProtocol encode stub: port not open");

        return input;
    }

    auto command =
        buildPacket(
            CMD_ENCODE);

    Logger::log(INFO,
        "AMBEProtocol encode:"
        " STREAMID=" +
        std::to_string(input.streamId) +
        " TX_LEN=" +
        std::to_string(command.size()));

    sendCommand(
        port,
        command);

    std::vector<uint8_t> response;

    readResponse(
        port,
        response);

    return input;
}

bool AMBEProtocol::sendCommand(
    SerialPort& port,
    const std::vector<uint8_t>& command)
{
    Logger::log(INFO,
        "AMBE TX: " +
        HexDump::toHex(command));

    return port.writeBytes(
        command);
}

bool AMBEProtocol::readResponse(
    SerialPort& port,
    std::vector<uint8_t>& response)
{
    bool result =
        port.readBytes(
            response,
            512,
            100);

    if (result)
    {
        Logger::log(INFO,
            "AMBE RX: " +
            HexDump::toHex(response));
    }

    return result;
}
