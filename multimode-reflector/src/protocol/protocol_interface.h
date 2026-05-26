#pragma once

#include "../core/media_destination.h"
#include "../core/media_frame.h"

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

    std::vector<MediaDestination> destinations;

    std::vector<MediaFrame> transcodedFrames;
};

class ProtocolInterface {
public:

    virtual ~ProtocolInterface() = default;

    virtual ProtocolResult handle(
        const uint8_t* data,
        size_t length,
        const std::string& peer) = 0;
};
