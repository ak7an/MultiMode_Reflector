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

static bool canTranscodeTo(
    MediaProtocol source,
    MediaProtocol target)
{
    auto targets =
        TranscodingTargets::getTargets(
            source);

    for (auto allowed : targets)
    {
        if (allowed == target)
        {
            return true;
        }
    }

    return false;
}

std::vector<MediaProtocol>
TranscodingPolicy::getTargets(
    const MediaFrame& frame)
{
    std::vector<MediaProtocol> enabledTargets;

    auto routeTargets =
        RouteConfig::targetsForFrame(
            frame);

    if (frame.sourceReflector.empty())
    {
        routeTargets =
            TranscodingTargets::getTargets(
                frame.protocol);
    }

    if (routeTargets.empty())
    {
        Logger::log(INFO,
            "TranscodingPolicy no route targets:"
            " REFLECTOR=" +
            frame.sourceReflector +
            " MODULE=" +
            std::string(1, frame.sourceModule));

        return enabledTargets;
    }

    for (auto target : routeTargets)
    {
        if (protocolEnabled(target) &&
            canTranscodeTo(
                frame.protocol,
                target))
        {
            enabledTargets.push_back(target);
        }
    }

    return enabledTargets;
}
