#include "ambe_device_manager.h"

#include "logger.h"
#include "device_discovery.h"
#include "ambe_protocol.h"

bool AMBEDeviceManager::m_ready = false;

SerialPort AMBEDeviceManager::m_decodePort;
SerialPort AMBEDeviceManager::m_encodePort;

AMBEDeviceStatus AMBEDeviceManager::m_status;

bool AMBEDeviceManager::initialize(
    const std::string& decodeDevice,
    const std::string& encodeDevice,
    int baudRate)
{
    DeviceDiscovery::findFTDIDevices();

    bool decodeOk =
        m_decodePort.openPort(
            decodeDevice,
            baudRate);

    bool encodeOk =
        m_encodePort.openPort(
            encodeDevice,
            baudRate);

    m_status.decodePresent =
        decodeOk;
    m_status.encodePresent =
        encodeOk;

    m_status.decodeOpen =
        decodeOk;
    m_status.encodeOpen =
        encodeOk;

    m_status.decodeResponsive =
        false;
    m_status.encodeResponsive =
        false;

    if (decodeOk)
    {
        bool resetOk =
            AMBEProtocol::softReset(
                m_decodePort);

        m_status.decodeResponsive =
            resetOk &&
            AMBEProtocol::probe(
                m_decodePort);
    }

    if (encodeOk)
    {
        bool resetOk =
            AMBEProtocol::softReset(
                m_encodePort);

        m_status.encodeResponsive =
            resetOk &&
            AMBEProtocol::probe(
                m_encodePort);
    }

    m_ready =
        decodeOk &&
        encodeOk &&
        m_status.decodeResponsive &&
        m_status.encodeResponsive;

    m_status.ready =
        m_ready;

    AMBEDeviceStatusReporter::logStatus(
        m_status);

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
        "AMBEDeviceManager decode:"
        " STREAMID=" +
        std::to_string(input.streamId));

    return AMBEProtocol::decode(
        m_decodePort,
        input);
}

CodecFrame AMBEDeviceManager::encodeAMBE(
    const CodecFrame& input)
{
    Logger::log(INFO,
        "AMBEDeviceManager encode:"
        " STREAMID=" +
        std::to_string(input.streamId));

    return AMBEProtocol::encode(
        m_encodePort,
        input);
}


AMBEDeviceStatus AMBEDeviceManager::status()
{
    return m_status;
}
