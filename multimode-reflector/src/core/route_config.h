#pragma once

#include "media_frame.h"

#include <string>
#include <vector>

struct RouteRule
{
    std::string reflector;
    char module = 'A';

    bool ysfEnabled = false;
    bool dmrEnabled = false;
    bool nxdnEnabled = false;
    bool p25Enabled = false;
    bool m17Enabled = false;
};

class RouteConfig
{
public:

    static void clear();

    static void addRoute(
        const RouteRule& rule);

    static std::vector<MediaProtocol>
    targetsForFrame(
        const MediaFrame& frame);

private:

    static std::vector<RouteRule> m_routes;
};
