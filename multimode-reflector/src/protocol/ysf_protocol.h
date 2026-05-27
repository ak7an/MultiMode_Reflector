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

    static bool parseSyntheticBridge(
        const uint8_t* data,
        size_t length,
        const std::string& peer,
        MediaFrame& frame);
};
