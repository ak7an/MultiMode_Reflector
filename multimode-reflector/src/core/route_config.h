#pragma once

#include "media_frame.h"

#include <string>
#include <vector>

class RouteConfig
{
public:

    static void setReflector(
        const std::string& reflector);

    static void setModule(
        char module);

    static void setYSFEnabled(
        bool enabled);

    static void setDMREnabled(
        bool enabled);

    static void setNXDNEnabled(
        bool enabled);

    static void setP25Enabled(
        bool enabled);

    static void setM17Enabled(
        bool enabled);

    static std::vector<MediaProtocol>
    targetsForFrame(
        const MediaFrame& frame);

    static const std::string&
    reflector();

    static char module();

private:

    static std::string m_reflector;
    static char m_module;

    static bool m_ysfEnabled;
    static bool m_dmrEnabled;
    static bool m_nxdnEnabled;
    static bool m_p25Enabled;
    static bool m_m17Enabled;
};
