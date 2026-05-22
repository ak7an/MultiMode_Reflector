#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

struct ProtocolFrame {

    std::vector<uint8_t> payload;
};

struct ProtocolResult {

    bool forwardCurrent;

    std::vector<ProtocolFrame> extraFrames;
};

class ProtocolInterface {
public:
    virtual ~ProtocolInterface() = default;

    virtual ProtocolResult handle(
        const uint8_t* data,
        size_t length,
        const std::string& peer) = 0;
};
