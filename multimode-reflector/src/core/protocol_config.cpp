#include "protocol_config.h"

bool ProtocolConfig::m_dstarEnabled = true;
bool ProtocolConfig::m_ysfEnabled = true;
bool ProtocolConfig::m_dmrEnabled = true;
bool ProtocolConfig::m_nxdnEnabled = false;
bool ProtocolConfig::m_p25Enabled = false;
bool ProtocolConfig::m_m17Enabled = false;

void ProtocolConfig::setDStarEnabled(bool enabled) { m_dstarEnabled = enabled; }
void ProtocolConfig::setYSFEnabled(bool enabled) { m_ysfEnabled = enabled; }
void ProtocolConfig::setDMREnabled(bool enabled) { m_dmrEnabled = enabled; }
void ProtocolConfig::setNXDNEnabled(bool enabled) { m_nxdnEnabled = enabled; }
void ProtocolConfig::setP25Enabled(bool enabled) { m_p25Enabled = enabled; }
void ProtocolConfig::setM17Enabled(bool enabled) { m_m17Enabled = enabled; }

bool ProtocolConfig::dstarEnabled() { return m_dstarEnabled; }
bool ProtocolConfig::ysfEnabled() { return m_ysfEnabled; }
bool ProtocolConfig::dmrEnabled() { return m_dmrEnabled; }
bool ProtocolConfig::nxdnEnabled() { return m_nxdnEnabled; }
bool ProtocolConfig::p25Enabled() { return m_p25Enabled; }
bool ProtocolConfig::m17Enabled() { return m_m17Enabled; }
