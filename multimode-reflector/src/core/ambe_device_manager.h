#pragma once

#include "codec_frame.h"

class AMBEDeviceManager
{
public:

    static bool initialize();

    static CodecFrame decodeAMBE(
        const CodecFrame& input);

    static CodecFrame encodeAMBE(
        const CodecFrame& input);

    static bool ready();

private:

    static bool m_ready;
};
