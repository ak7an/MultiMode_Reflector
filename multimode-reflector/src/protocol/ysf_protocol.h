#pragma once

#include "protocol_interface.h"

#include <cstddef>
#include <cstdint>
#include <string>

class YSFProtocol : public ProtocolInterface {
public:

    ProtocolResult handle(
        const uint8_t* data,
        size_t length,
        const std::string& peer) override;

private:

    /*
     * Current YSF support accepts project-defined bridge/scaffold frames:
     *
     * YSFB = synthetic bridge packet
     * YSFD = scaffold network packet
     *
     * This is not yet real Yaesu YSF wire-format parsing.
     */
    static bool parseProjectBridgeFrame(
        const uint8_t* data,
        size_t length,
        const std::string& peer,
        MediaFrame& frame);
};
