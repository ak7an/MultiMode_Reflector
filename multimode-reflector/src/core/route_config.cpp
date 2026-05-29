#include "route_config.h"

std::string RouteConfig::m_sourceReflector = "XLX999";
char RouteConfig::m_sourceModule = 'A';

bool RouteConfig::m_ysfEnabled = true;
bool RouteConfig::m_dmrEnabled = true;
bool RouteConfig::m_nxdnEnabled = false;
bool RouteConfig::m_p25Enabled = false;
bool RouteConfig::m_m17Enabled = false;

void RouteConfig::setSourceReflector(const std::string& v) { m_sourceReflector = v; }
void RouteConfig::setSourceModule(char v) { m_sourceModule = v; }

void RouteConfig::setYSFEnabled(bool v) { m_ysfEnabled = v; }
void RouteConfig::setDMREnabled(bool v) { m_dmrEnabled = v; }
void RouteConfig::setNXDNEnabled(bool v) { m_nxdnEnabled = v; }
void RouteConfig::setP25Enabled(bool v) { m_p25Enabled = v; }
void RouteConfig::setM17Enabled(bool v) { m_m17Enabled = v; }

const std::string& RouteConfig::sourceReflector() { return m_sourceReflector; }
char RouteConfig::sourceModule() { return m_sourceModule; }

bool RouteConfig::ysfEnabled() { return m_ysfEnabled; }
bool RouteConfig::dmrEnabled() { return m_dmrEnabled; }
bool RouteConfig::nxdnEnabled() { return m_nxdnEnabled; }
bool RouteConfig::p25Enabled() { return m_p25Enabled; }
bool RouteConfig::m17Enabled() { return m_m17Enabled; }
