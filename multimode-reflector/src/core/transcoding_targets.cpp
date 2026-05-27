#include "transcoding_targets.h"

#include "transcoding_matrix.h"

std::vector<MediaProtocol>
TranscodingTargets::getTargets(
    MediaProtocol source)
{
    std::vector<MediaProtocol> targets;

    const MediaProtocol protocols[] = {

        MediaProtocol::DSTAR,
        MediaProtocol::YSF,
        MediaProtocol::DMR,
        MediaProtocol::NXDN,
        MediaProtocol::P25,
        MediaProtocol::M17
    };

    for (auto target : protocols) {

        if (TranscodingMatrix::canTranscode(
                source,
                target))
        {
            targets.push_back(
                target);
        }
    }

    return targets;
}
