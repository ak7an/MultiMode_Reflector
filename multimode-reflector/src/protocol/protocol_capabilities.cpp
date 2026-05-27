#include "protocol_capabilities.h"

ProtocolCapabilities getProtocolCapabilities(
    MediaProtocol protocol)
{
    ProtocolCapabilities caps{};

    switch (protocol) {

    case MediaProtocol::DSTAR:

        caps.ingest = true;
        caps.encode = true;
        caps.networkParse = true;
        caps.networkBuild = true;
        caps.transcodeSource = true;
        caps.transcodeTarget = true;
        break;

    case MediaProtocol::YSF:

        caps.ingest = true;
        caps.encode = true;
        caps.networkParse = true;
        caps.networkBuild = true;
        caps.transcodeSource = true;
        caps.transcodeTarget = true;
        break;

    case MediaProtocol::DMR:
    case MediaProtocol::NXDN:
    case MediaProtocol::P25:
    case MediaProtocol::M17:

        caps.transcodeSource = true;
        caps.transcodeTarget = true;
        break;

    default:
        break;
    }

    return caps;
}
