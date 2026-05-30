#pragma once

#include "protocol_interface.h"

class P25Protocol : public ProtocolInterface
{
public:
    ProtocolResult handle(
        const uint8_t* data,
        size_t length,
        const std::string& peer) override;
};
