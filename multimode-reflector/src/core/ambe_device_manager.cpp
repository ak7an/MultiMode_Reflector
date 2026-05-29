#include "ambe_device_manager.h"

#include "logger.h"

bool AMBEDeviceManager::m_ready = false;

bool AMBEDeviceManager::initialize()
{
    /*
     * Placeholder for future DVSI AMBE-3000 USB initialization.
     *
     * Target design:
     * - AMBE device #1 handles decode path
     * - AMBE device #2 handles encode path
     */
    m_ready = false;

    Logger::log(INFO,
        "AMBEDeviceManager initialized in stub mode");

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
