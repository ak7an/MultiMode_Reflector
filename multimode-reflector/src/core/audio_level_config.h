#pragma once

#include "../core/media_frame.h"

class AudioLevelConfig
{
public:

    static void setDStarGainInDb(int value);
    static void setDStarGainOutDb(int value);

    static void setYSFGainInDb(int value);
    static void setYSFGainOutDb(int value);

    static void setDMRGainInDb(int value);
    static void setDMRGainOutDb(int value);

    static void setNXDNGainInDb(int value);
    static void setNXDNGainOutDb(int value);

    static void setP25GainInDb(int value);
    static void setP25GainOutDb(int value);

    static void setM17GainInDb(int value);
    static void setM17GainOutDb(int value);

    static int gainInDb(
        MediaProtocol protocol);

    static int gainOutDb(
        MediaProtocol protocol);

private:

    static int clampGain(
        int value);

    static int m_dstarGainInDb;
    static int m_dstarGainOutDb;

    static int m_ysfGainInDb;
    static int m_ysfGainOutDb;

    static int m_dmrGainInDb;
    static int m_dmrGainOutDb;

    static int m_nxdnGainInDb;
    static int m_nxdnGainOutDb;

    static int m_p25GainInDb;
    static int m_p25GainOutDb;

    static int m_m17GainInDb;
    static int m_m17GainOutDb;
};
