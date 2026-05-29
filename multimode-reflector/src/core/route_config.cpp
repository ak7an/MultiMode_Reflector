#include "route_config.h"

std::vector<RouteRule> RouteConfig::m_routes;

void RouteConfig::clear()
{
    m_routes.clear();
}

void RouteConfig::addRoute(
    const RouteRule& rule)
{
    m_routes.push_back(rule);
}

std::vector<MediaProtocol>
RouteConfig::targetsForFrame(
    const MediaFrame& frame)
{
    std::vector<MediaProtocol> targets;

    if (frame.sourceReflector.empty())
    {
        return targets;
    }

    for (const auto& route : m_routes)
    {
        if (route.reflector != frame.sourceReflector)
        {
            continue;
        }

        if (route.module != frame.sourceModule)
        {
            continue;
        }

        if (route.ysfEnabled)
        {
            targets.push_back(MediaProtocol::YSF);
        }

        if (route.dmrEnabled)
        {
            targets.push_back(MediaProtocol::DMR);
        }

        if (route.nxdnEnabled)
        {
            targets.push_back(MediaProtocol::NXDN);
        }

        if (route.p25Enabled)
        {
            targets.push_back(MediaProtocol::P25);
        }

        if (route.m17Enabled)
        {
            targets.push_back(MediaProtocol::M17);
        }

        return targets;
    }

    return targets;
}
