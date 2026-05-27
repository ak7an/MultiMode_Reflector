#include "transcoding_matrix.h"

#include "../protocol/protocol_capabilities.h"

bool TranscodingMatrix::canTranscode(
    MediaProtocol source,
    MediaProtocol target)
{
    if (source == target) {
        return false;
    }

    auto sourceCaps =
        getProtocolCapabilities(
            source);

    auto targetCaps =
        getProtocolCapabilities(
            target);

    return sourceCaps.transcodeSource &&
           targetCaps.transcodeTarget;
}
