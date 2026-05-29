#pragma once

class ProtocolConfig
{
public:
    static void setDStarEnabled(bool enabled);
    static void setYSFEnabled(bool enabled);
    static void setDMREnabled(bool enabled);
    static void setNXDNEnabled(bool enabled);
    static void setP25Enabled(bool enabled);
    static void setM17Enabled(bool enabled);

    static bool dstarEnabled();
    static bool ysfEnabled();
    static bool dmrEnabled();
    static bool nxdnEnabled();
    static bool p25Enabled();
    static bool m17Enabled();

private:
    static bool m_dstarEnabled;
    static bool m_ysfEnabled;
    static bool m_dmrEnabled;
    static bool m_nxdnEnabled;
    static bool m_p25Enabled;
    static bool m_m17Enabled;
};
