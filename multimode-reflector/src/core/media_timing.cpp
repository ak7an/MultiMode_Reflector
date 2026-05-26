#include "media_timing.h"

int MediaTiming::targetDelayMs(
    MediaProtocol protocol)
{
    switch (protocol) {

    case MediaProtocol::DSTAR:
        return 20;

    case MediaProtocol::YSF:
        return 20;

    case MediaProtocol::DMR:
        return 60;

    case MediaProtocol::P25:
        return 20;

    case MediaProtocol::NXDN:
        return 20;

    case MediaProtocol::M17:
        return 20;

    default:
        return 20;
    }
}
