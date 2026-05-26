#pragma once

#include "media_frame.h"

#include <cstdint>

class ActiveStream {
public:

    static bool accept(
        const MediaFrame& frame);

    static void end(
        MediaProtocol protocol,
        uint16_t streamId);

private:

    static bool m_active;
    static MediaProtocol m_protocol;
    static uint16_t m_streamId;
};
