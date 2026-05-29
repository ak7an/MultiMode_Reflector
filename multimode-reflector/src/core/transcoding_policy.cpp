#include "transcoding_policy.h"

#include "transcoding_targets.h"
#include "protocol_config.h"
#include "route_config.h"
#include "logger.h"

static bool protocolEnabled(
    MediaProtocol protocol)
{
    switch (protocol)
    {
    case MediaProtocol::DSTAR:
        return ProtocolConfig::dstarEnabled();

    case MediaProtocol::YSF:
        return ProtocolConfig::ysfEnabled();

    case MediaProtocol::DMR:
        return ProtocolConfig::dmrEnabled();

    case MediaProtocol::NXDN:
        return ProtocolConfig::nxdnEnabled();

    case MediaProtocol::P25:
        return ProtocolConfig::p25Enabled();

    case MediaProtocol::M17:
        return ProtocolConfig::m17Enabled();

    default:
        return false;
    }
}

static bool routeEnabled(
    MediaProtocol protocol)
{
    switch (protocol)
    {
    case MediaProtocol::YSF:
        return RouteConfig::ysfEnabled();

    case MediaProtocol::DMR:
        return RouteConfig::dmrEnabled();

    case MediaProtocol::NXDN:
        return RouteConfig::nxdnEnabled();

    case MediaProtocol::P25:
        return RouteConfig::p25Enabled();

    case MediaProtocol::M17:
        return RouteConfig::m17Enabled();

    default:
        return false;
    }
}

static bool routeMatches(
    const MediaFrame& frame)
{
    if (frame.sourceReflector.empty())
    {
        return true;
    }

    return frame.sourceReflector ==
               RouteConfig::sourceReflector() &&
           frame.sourceModule ==
               RouteConfig::sourceModule();
}

std::vector<MediaProtocol>
TranscodingPolicy::getTargets(
    const MediaFrame& frame)
{
    std::vector<MediaProtocol> enabledTargets;

    if (!routeMatches(frame))
    {
        Logger::log(INFO,
            "TranscodingPolicy route mismatch:"
            " REFLECTOR=" +
            frame.sourceReflector +
            " MODULE=" +
            std::string(1, frame.sourceModule));

        return enabledTargets;
    }

    auto targets =
        TranscodingTargets::getTargets(
            frame.protocol);

    for (auto target : targets)
    {
        if (protocolEnabled(target) &&
            routeEnabled(target))
        {
            enabledTargets.push_back(target);
        }
    }

    return enabledTargets;
}
