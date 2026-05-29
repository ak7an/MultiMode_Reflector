#pragma once

#include <string>

class RouteConfig
{
public:

    static void setSourceReflector(
        const std::string& reflector);

    static void setSourceModule(
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

    static const std::string&
    sourceReflector();

    static char sourceModule();

    static bool ysfEnabled();
    static bool dmrEnabled();
    static bool nxdnEnabled();
    static bool p25Enabled();
    static bool m17Enabled();

private:

    static std::string m_sourceReflector;
    static char m_sourceModule;

    static bool m_ysfEnabled;
    static bool m_dmrEnabled;
    static bool m_nxdnEnabled;
    static bool m_p25Enabled;
    static bool m_m17Enabled;
};
