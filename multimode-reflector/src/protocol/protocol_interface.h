#pragma once

#include <cstddef>
#include <cstdint>
#include <string>

class ProtocolInterface {
public:

    virtual ~ProtocolInterface() = default;

    virtual void handle(
        const uint8_t* data,
        size_t length,
        const std::string& peer) = 0;
};
