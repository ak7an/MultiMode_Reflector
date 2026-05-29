#include "audio_level_config.h"

static constexpr int MIN_GAIN_DB = -24;
static constexpr int MAX_GAIN_DB = 24;

int AudioLevelConfig::m_dstarGainInDb = 16;
int AudioLevelConfig::m_dstarGainOutDb = -10;

int AudioLevelConfig::m_ysfGainInDb = -3;
int AudioLevelConfig::m_ysfGainOutDb = 0;

int AudioLevelConfig::m_dmrGainInDb = -3;
int AudioLevelConfig::m_dmrGainOutDb = 0;

int AudioLevelConfig::m_nxdnGainInDb = 0;
int AudioLevelConfig::m_nxdnGainOutDb = 0;

int AudioLevelConfig::m_p25GainInDb = 0;
int AudioLevelConfig::m_p25GainOutDb = 0;

int AudioLevelConfig::m_m17GainInDb = 0;
int AudioLevelConfig::m_m17GainOutDb = 0;

int AudioLevelConfig::clampGain(
    int value)
{
    if (value < MIN_GAIN_DB)
    {
        return MIN_GAIN_DB;
    }

    if (value > MAX_GAIN_DB)
    {
        return MAX_GAIN_DB;
    }

    return value;
}

void AudioLevelConfig::setDStarGainInDb(int v) { m_dstarGainInDb = clampGain(v); }
void AudioLevelConfig::setDStarGainOutDb(int v) { m_dstarGainOutDb = clampGain(v); }

void AudioLevelConfig::setYSFGainInDb(int v) { m_ysfGainInDb = clampGain(v); }
void AudioLevelConfig::setYSFGainOutDb(int v) { m_ysfGainOutDb = clampGain(v); }

void AudioLevelConfig::setDMRGainInDb(int v) { m_dmrGainInDb = clampGain(v); }
void AudioLevelConfig::setDMRGainOutDb(int v) { m_dmrGainOutDb = clampGain(v); }

void AudioLevelConfig::setNXDNGainInDb(int v) { m_nxdnGainInDb = clampGain(v); }
void AudioLevelConfig::setNXDNGainOutDb(int v) { m_nxdnGainOutDb = clampGain(v); }

void AudioLevelConfig::setP25GainInDb(int v) { m_p25GainInDb = clampGain(v); }
void AudioLevelConfig::setP25GainOutDb(int v) { m_p25GainOutDb = clampGain(v); }

void AudioLevelConfig::setM17GainInDb(int v) { m_m17GainInDb = clampGain(v); }
void AudioLevelConfig::setM17GainOutDb(int v) { m_m17GainOutDb = clampGain(v); }

int AudioLevelConfig::gainInDb(
    MediaProtocol protocol)
{
    switch (protocol)
    {
    case MediaProtocol::DSTAR:
        return m_dstarGainInDb;

    case MediaProtocol::YSF:
        return m_ysfGainInDb;

    case MediaProtocol::DMR:
        return m_dmrGainInDb;

    case MediaProtocol::NXDN:
        return m_nxdnGainInDb;

    case MediaProtocol::P25:
        return m_p25GainInDb;

    case MediaProtocol::M17:
        return m_m17GainInDb;

    default:
        return 0;
    }
}

int AudioLevelConfig::gainOutDb(
    MediaProtocol protocol)
{
    switch (protocol)
    {
    case MediaProtocol::DSTAR:
        return m_dstarGainOutDb;

    case MediaProtocol::YSF:
        return m_ysfGainOutDb;

    case MediaProtocol::DMR:
        return m_dmrGainOutDb;

    case MediaProtocol::NXDN:
        return m_nxdnGainOutDb;

    case MediaProtocol::P25:
        return m_p25GainOutDb;

    case MediaProtocol::M17:
        return m_m17GainOutDb;

    default:
        return 0;
    }
}
