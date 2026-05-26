#pragma once

#include <cstdint>
#include <cstddef>

class DebugUdpSender {
public:

    static void sendToLocal(
        const uint8_t* data,
        size_t length,
        uint16_t port);
};
