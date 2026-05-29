#include "ambe_device_manager.h"

#include "logger.h"

bool AMBEDeviceManager::m_ready = false;

SerialPort AMBEDeviceManager::m_decodePort;
SerialPort AMBEDeviceManager::m_encodePort;

bool AMBEDeviceManager::initialize(
    const std::string& decodeDevice,
    const std::string& encodeDevice,
    int baudRate)
{
    bool decodeOk =
        m_decodePort.openPort(
            decodeDevice,
            baudRate);

    bool encodeOk =
        m_encodePort.openPort(
            encodeDevice,
            baudRate);

    m_ready =
        decodeOk &&
        encodeOk;

    Logger::log(INFO,
        "AMBEDeviceManager init:"
        " DECODE=" +
        decodeDevice +
        " ENCODE=" +
        encodeDevice +
        " BAUD=" +
        std::to_string(baudRate) +
        " READY=" +
        std::to_string(m_ready));

    return m_ready;
}

bool AMBEDeviceManager::ready()
{
    return m_ready;
}

CodecFrame AMBEDeviceManager::decodeAMBE(
    const CodecFrame& input)
{
    Logger::log(INFO,
        "AMBEDeviceManager decode stub:"
        " STREAMID=" +
        std::to_string(input.streamId));

    return input;
}

CodecFrame AMBEDeviceManager::encodeAMBE(
    const CodecFrame& input)
{
    Logger::log(INFO,
        "AMBEDeviceManager encode stub:"
        " STREAMID=" +
        std::to_string(input.streamId));

    return input;
}
