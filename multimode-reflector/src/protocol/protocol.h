#pragma once

#include "../core/media_destination.h"

#include <cstdint>
#include <vector>

struct ProtocolFrame {

    std::vector<uint8_t> payload;
};

struct ProtocolResult {

    bool forwardCurrent;

    std::vector<ProtocolFrame> extraFrames;

    std::vector<MediaDestination> destinations;
};

class Protocol {
public:

    virtual ~Protocol() = default;

    virtual ProtocolResult handle(
        const uint8_t* data,
        size_t length,
        const std::string& peer) = 0;
};
