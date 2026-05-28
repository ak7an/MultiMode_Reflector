#include "protocol_config.h"

bool ProtocolConfig::m_dstarEnabled = true;
bool ProtocolConfig::m_ysfEnabled = true;
bool ProtocolConfig::m_dmrEnabled = true;

void ProtocolConfig::setDStarEnabled(bool enabled)
{
    m_dstarEnabled = enabled;
}

void ProtocolConfig::setYSFEnabled(bool enabled)
{
    m_ysfEnabled = enabled;
}

void ProtocolConfig::setDMREnabled(bool enabled)
{
    m_dmrEnabled = enabled;
}

bool ProtocolConfig::dstarEnabled()
{
    return m_dstarEnabled;
}

bool ProtocolConfig::ysfEnabled()
{
    return m_ysfEnabled;
}

bool ProtocolConfig::dmrEnabled()
{
    return m_dmrEnabled;
}
