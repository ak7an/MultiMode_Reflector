#include "jitter_buffer.h"

#include "logger.h"

void JitterBuffer::observe(
    const std::string& protocol,
    uint16_t streamId,
    uint8_t sequence)
{
    Logger::log(INFO,
        "JitterBuffer observe:"
        " PROTO=" + protocol +
        " STREAMID=" + std::to_string(streamId) +
        " SEQ=" + std::to_string(sequence & 0x1F));
}
