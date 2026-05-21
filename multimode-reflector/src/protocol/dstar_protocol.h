#pragma once

#include "protocol_interface.h"

#include <cstddef>
#include <cstdint>
#include <string>

class DStarProtocol :
    public ProtocolInterface
{
public:
    bool handle(
        const uint8_t* data,
        size_t length,
        const std::string& peer) override;
};
