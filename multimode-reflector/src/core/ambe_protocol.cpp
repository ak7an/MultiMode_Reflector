#include "ambe_protocol.h"

#include "logger.h"
#include "hex_dump.h"

namespace
{
    constexpr uint8_t AMBE_PACKET_START = 0x61;

    constexpr uint8_t PKT_CONTROL = 0x00;

    constexpr uint8_t CMD_PROBE      = 0x31;
    constexpr uint8_t CMD_SOFT_RESET = 0x34;
    constexpr uint8_t CMD_DECODE = 0x02;
    constexpr uint8_t CMD_ENCODE = 0x03;
}

static std::string commandName(
    uint8_t command)
{
    switch (command)
    {
    case CMD_PROBE:
        return "PROBE";

    case CMD_SOFT_RESET:
        return "SOFT_RESET";

    case CMD_DECODE:
        return "DECODE";

    case CMD_ENCODE:
        return "ENCODE";

    default:
        return "UNKNOWN";
    }
}

static std::string payloadToAscii(
    const std::vector<uint8_t>& payload)
{
    std::string text;

    for (uint8_t byte : payload)
    {
        if (byte == 0)
        {
            break;
        }

        if (byte >= 32 && byte <= 126)
        {
            text.push_back(
                static_cast<char>(
                    byte));
        }
    }

    return text;
}

static std::vector<uint8_t> buildPacket(
    uint8_t packetType,
    const std::vector<uint8_t>& payload = {})
{
    std::vector<uint8_t> packet;

    packet.push_back(
        AMBE_PACKET_START);

    uint16_t length =
        static_cast<uint16_t>(
            payload.size());

    packet.push_back(
        (length >> 8) & 0xff);

    packet.push_back(
        length & 0xff);

    packet.push_back(
        packetType);

    packet.insert(
        packet.end(),
        payload.begin(),
        payload.end());

    return packet;
}


bool AMBEProtocol::validatePacket(
    const std::vector<uint8_t>& packet)
{
    if (packet.size() < 4)
    {
        Logger::log(WARN,
            "AMBEProtocol invalid packet: too short LEN=" +
            std::to_string(packet.size()));

        return false;
    }

    if (packet[0] != AMBE_PACKET_START)
    {
        Logger::log(WARN,
            "AMBEProtocol invalid packet: bad start BYTE=" +
            std::to_string(packet[0]));

        return false;
    }

    uint16_t length =
        static_cast<uint16_t>(
            (packet[1] << 8) |
            packet[2]);

    size_t expectedSize =
        static_cast<size_t>(
            length) + 4;

    if (packet.size() != expectedSize)
    {
        Logger::log(WARN,
            "AMBEProtocol invalid packet: length mismatch"
            " DECLARED=" +
            std::to_string(length) +
            " ACTUAL=" +
            std::to_string(packet.size()));

        return false;
    }

    Logger::log(INFO,
        "AMBEProtocol packet valid:"
        " CMD=" +
        std::to_string(packet[3]) +
        " LEN=" +
        std::to_string(length));

    return true;
}


AMBEResponse AMBEProtocol::parseResponse(
    const std::vector<uint8_t>& packet)
{
    AMBEResponse response;

    if (!validatePacket(
            packet))
    {
        return response;
    }

    response.valid =
        true;

    response.command =
        packet.size() > 4 ? packet[4] : packet[3];

    if (packet.size() > 4)
    {
        response.payload.assign(
            packet.begin() + 4,
            packet.end());
    }

    Logger::log(INFO,
        "AMBEProtocol response parsed:"
        " CMD=" +
        std::to_string(response.command) +
        " NAME=" +
        commandName(response.command) +
        " PAYLOAD_LEN=" +
        std::to_string(response.payload.size()));

    if (response.command == CMD_PROBE)
    {
        std::string version =
            payloadToAscii(
                response.payload);

        if (!version.empty())
        {
            Logger::log(INFO,
                "AMBE Device Version: " +
                version);
        }
    }

    return response;
}

bool AMBEProtocol::softReset(
    SerialPort& port)
{
    if (!port.isOpen())
    {
        Logger::log(INFO,
            "AMBEProtocol soft reset skipped: port not open");

        return false;
    }

    auto command =
        buildPacket(
            PKT_CONTROL,
            { CMD_SOFT_RESET, 0x05, 0x00, 0x00, 0x0F, 0x00, 0x00 });

    Logger::log(INFO,
        "AMBEProtocol soft reset:"
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
            "AMBEProtocol soft reset response:"
            " RX_LEN=" +
            std::to_string(response.size()));

        return true;
    }

    Logger::log(WARN,
        "AMBEProtocol soft reset no response");

    return false;
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
            PKT_CONTROL,
            { CMD_PROBE, 0x2F, 0x1D });

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

        AMBEResponse parsed =
            parseResponse(
                response);

        return parsed.valid;
    }

    return false;
}
