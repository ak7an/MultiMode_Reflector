#include "ambe_protocol.h"

#include "logger.h"
#include "hex_dump.h"

namespace
{
    constexpr uint8_t AMBE_PACKET_START = 0x61;

    constexpr uint8_t PKT_CONTROL = 0x00;
    constexpr uint8_t PKT_CHANNEL = 0x01;
    constexpr uint8_t PKT_SPEECH  = 0x02;

    constexpr uint8_t PKT_SPEECHD = 0x00;
    constexpr uint8_t PKT_CHAND   = 0x01;

    constexpr uint8_t AMBE_NUM_BITS = 72;
    constexpr uint8_t PCM_NUM_SAMPLES = 160;

    constexpr uint8_t CMD_PROBE      = 0x31;
    constexpr uint8_t CMD_SOFT_RESET = 0x34;
    constexpr uint8_t CMD_RESET_ACK  = 0x39;
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

    case CMD_RESET_ACK:
        return "RESET_ACK";

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


static std::vector<uint8_t> buildAMBEToPCMPacket(
    const std::vector<uint8_t>& ambe)
{
    std::vector<uint8_t> payload;

    payload.push_back(
        PKT_CHAND);

    payload.push_back(
        AMBE_NUM_BITS);

    payload.insert(
        payload.end(),
        ambe.begin(),
        ambe.end());

    return buildPacket(
        PKT_CHANNEL,
        payload);
}

static std::vector<uint8_t> buildPCMToAMBEPacket(
    const std::vector<uint8_t>& pcm)
{
    std::vector<uint8_t> payload;

    payload.push_back(
        PKT_SPEECHD);

    payload.push_back(
        PCM_NUM_SAMPLES);

    payload.insert(
        payload.end(),
        pcm.begin(),
        pcm.end());

    return buildPacket(
        PKT_SPEECH,
        payload);
}

static bool extractPCMResponse(
    const std::vector<uint8_t>& packet,
    std::vector<uint8_t>& pcm)
{
    pcm.clear();

    if (packet.size() != 326)
    {
        Logger::log(WARN,
            "AMBEProtocol PCM response invalid size:"
            " LEN=" +
            std::to_string(packet.size()));

        return false;
    }

    if (packet[3] != PKT_SPEECH ||
        packet[4] != PKT_SPEECHD ||
        packet[5] != PCM_NUM_SAMPLES)
    {
        Logger::log(WARN,
            "AMBEProtocol PCM response invalid header");

        return false;
    }

    pcm.assign(
        packet.begin() + 6,
        packet.end());

    return true;
}

static bool extractAMBEResponse(
    const std::vector<uint8_t>& packet,
    std::vector<uint8_t>& ambe)
{
    ambe.clear();

    if (packet.size() != 15)
    {
        Logger::log(WARN,
            "AMBEProtocol AMBE response invalid size:"
            " LEN=" +
            std::to_string(packet.size()));

        return false;
    }

    if (packet[3] != PKT_CHANNEL ||
        packet[4] != PKT_CHAND ||
        packet[5] != AMBE_NUM_BITS)
    {
        Logger::log(WARN,
            "AMBEProtocol AMBE response invalid header");

        return false;
    }

    ambe.assign(
        packet.begin() + 6,
        packet.end());

    return true;
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

    if (input.payload.size() != 9)
    {
        Logger::log(WARN,
            "AMBEProtocol decode skipped:"
            " STREAMID=" +
            std::to_string(input.streamId) +
            " REASON=invalid D-Star AMBE payload size"
            " PAYLOAD_LEN=" +
            std::to_string(input.payload.size()));

        return input;
    }

    auto command =
        buildAMBEToPCMPacket(
            input.payload);

    Logger::log(INFO,
        "AMBEProtocol decode:"
        " STREAMID=" +
        std::to_string(input.streamId) +
        " PAYLOAD_LEN=" +
        std::to_string(input.payload.size()) +
        " TX_LEN=" +
        std::to_string(command.size()));

    sendCommand(
        port,
        command);

    std::vector<uint8_t> response;

    CodecFrame output =
        input;

    if (readResponse(
            port,
            response))
    {
        std::vector<uint8_t> pcm;

        if (extractPCMResponse(
                response,
                pcm))
        {
            output.codec =
                CodecType::PCM;

            output.payload =
                pcm;

            Logger::log(INFO,
                "AMBEProtocol decode PCM ready:"
                " STREAMID=" +
                std::to_string(output.streamId) +
                " PCM_BYTES=" +
                std::to_string(output.payload.size()));
        }
    }

    return output;
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

    if (input.payload.size() != 320)
    {
        Logger::log(WARN,
            "AMBEProtocol encode skipped:"
            " STREAMID=" +
            std::to_string(input.streamId) +
            " REASON=invalid PCM payload size"
            " PAYLOAD_LEN=" +
            std::to_string(input.payload.size()));

        return input;
    }

    auto command =
        buildPCMToAMBEPacket(
            input.payload);

    Logger::log(INFO,
        "AMBEProtocol encode:"
        " STREAMID=" +
        std::to_string(input.streamId) +
        " PAYLOAD_LEN=" +
        std::to_string(input.payload.size()) +
        " TX_LEN=" +
        std::to_string(command.size()));

    sendCommand(
        port,
        command);

    std::vector<uint8_t> response;

    CodecFrame output =
        input;

    if (readResponse(
            port,
            response))
    {
        std::vector<uint8_t> ambe;

        if (extractAMBEResponse(
                response,
                ambe))
        {
            output.codec =
                CodecType::AMBE;

            output.payload =
                ambe;

            Logger::log(INFO,
                "AMBEProtocol encode AMBE ready:"
                " STREAMID=" +
                std::to_string(output.streamId) +
                " AMBE_BYTES=" +
                std::to_string(output.payload.size()));
        }
    }

    return output;
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
    response.clear();

    std::vector<uint8_t> chunk;

    for (int attempt = 0;
         attempt < 10;
         ++attempt)
    {
        if (!port.readBytes(
                chunk,
                512,
                50))
        {
            continue;
        }

        response.insert(
            response.end(),
            chunk.begin(),
            chunk.end());

        if (response.size() < 4)
        {
            continue;
        }

        uint16_t length =
            (static_cast<uint16_t>(response[1]) << 8) |
             static_cast<uint16_t>(response[2]);

        size_t expectedSize =
            static_cast<size_t>(length) + 4;

        if (response.size() < expectedSize)
        {
            continue;
        }

        Logger::log(INFO,
            "AMBE RX: " +
            HexDump::toHex(response));

        AMBEResponse parsed =
            parseResponse(
                response);

        return parsed.valid;
    }

    if (!response.empty())
    {
        Logger::log(WARN,
            "AMBE partial packet: LEN=" +
            std::to_string(response.size()));
    }

    return false;
}
