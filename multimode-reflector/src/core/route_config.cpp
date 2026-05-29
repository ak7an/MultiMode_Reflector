#include "route_config.h"

std::string RouteConfig::m_reflector = "XLX999";
char RouteConfig::m_module = 'A';

bool RouteConfig::m_ysfEnabled = true;
bool RouteConfig::m_dmrEnabled = true;
bool RouteConfig::m_nxdnEnabled = false;
bool RouteConfig::m_p25Enabled = false;
bool RouteConfig::m_m17Enabled = false;

void RouteConfig::setReflector(
    const std::string& reflector)
{
    m_reflector = reflector;
}

void RouteConfig::setModule(
    char module)
{
    m_module = module;
}

void RouteConfig::setYSFEnabled(
    bool enabled)
{
    m_ysfEnabled = enabled;
}

void RouteConfig::setDMREnabled(
    bool enabled)
{
    m_dmrEnabled = enabled;
}

void RouteConfig::setNXDNEnabled(
    bool enabled)
{
    m_nxdnEnabled = enabled;
}

void RouteConfig::setP25Enabled(
    bool enabled)
{
    m_p25Enabled = enabled;
}

void RouteConfig::setM17Enabled(
    bool enabled)
{
    m_m17Enabled = enabled;
}

const std::string&
RouteConfig::reflector()
{
    return m_reflector;
}

char RouteConfig::module()
{
    return m_module;
}

std::vector<MediaProtocol>
RouteConfig::targetsForFrame(
    const MediaFrame& frame)
{
    std::vector<MediaProtocol> targets;

    if (!frame.sourceReflector.empty() &&
        frame.sourceReflector != m_reflector)
    {
        return targets;
    }

    if (!frame.sourceReflector.empty() &&
        frame.sourceModule != m_module)
    {
        return targets;
    }

    if (m_ysfEnabled)
    {
        targets.push_back(MediaProtocol::YSF);
    }

    if (m_dmrEnabled)
    {
        targets.push_back(MediaProtocol::DMR);
    }

    if (m_nxdnEnabled)
    {
        targets.push_back(MediaProtocol::NXDN);
    }

    if (m_p25Enabled)
    {
        targets.push_back(MediaProtocol::P25);
    }

    if (m_m17Enabled)
    {
        targets.push_back(MediaProtocol::M17);
    }

    return targets;
}
