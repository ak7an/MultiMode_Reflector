#pragma once

#include "codec_frame.h"
#include "serial_port.h"
#include "ambe_device_status.h"

#include <string>

class AMBEDeviceManager
{
public:

    static bool initialize(
        const std::string& decodeDevice,
        const std::string& encodeDevice,
        int baudRate);

    static CodecFrame decodeAMBE(
        const CodecFrame& input);

    static CodecFrame encodeAMBE(
        const CodecFrame& input);

    static bool ready();

    static AMBEDeviceStatus status();

private:

    static bool m_ready;

    static SerialPort m_decodePort;
    static SerialPort m_encodePort;

    static AMBEDeviceStatus m_status;
};
