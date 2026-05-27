#include "transcoding_policy.h"

#include "transcoding_targets.h"

std::vector<MediaProtocol>
TranscodingPolicy::getTargets(
    const MediaFrame& frame)
{
    return TranscodingTargets::getTargets(
        frame.protocol);
}
