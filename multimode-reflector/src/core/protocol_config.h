#pragma once

class ProtocolConfig
{
public:
    static void setDStarEnabled(bool enabled);
    static void setYSFEnabled(bool enabled);
    static void setDMREnabled(bool enabled);

    static bool dstarEnabled();
    static bool ysfEnabled();
    static bool dmrEnabled();

private:
    static bool m_dstarEnabled;
    static bool m_ysfEnabled;
    static bool m_dmrEnabled;
};
