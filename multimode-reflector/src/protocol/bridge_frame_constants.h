#pragma once

#include <cstddef>

namespace BridgeFrameConstants {

    constexpr const char* DSTAR_BRIDGE_MAGIC = "DSBB";
    constexpr const char* YSF_BRIDGE_MAGIC   = "YSFB";

    constexpr size_t MAGIC_OFFSET = 0;
    constexpr size_t STREAM_ID_OFFSET = 4;
    constexpr size_t SEQUENCE_OFFSET = 6;
    constexpr size_t EOT_OFFSET = 7;
    constexpr size_t TARGET_MAGIC_OFFSET = 8;
    constexpr size_t PAYLOAD_LENGTH_OFFSET = 12;
    constexpr size_t FRAME_TYPE_OFFSET = 14;
    constexpr size_t PAYLOAD_OFFSET = 15;

    constexpr size_t HEADER_SIZE = 15;
}
