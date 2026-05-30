#include "protocol_ports.h"

int ProtocolPorts::m_dstarPort = 9000;
int ProtocolPorts::m_ysfPort = 42000;
int ProtocolPorts::m_dmrPort = 62031;
int ProtocolPorts::m_nxdnPort = 41400;

void ProtocolPorts::setDStarPort(int port)
{
    m_dstarPort = port;
}

void ProtocolPorts::setYSFPort(int port)
{
    m_ysfPort = port;
}

void ProtocolPorts::setDMRPort(int port)
{
    m_dmrPort = port;
}

int ProtocolPorts::dstarPort()
{
    return m_dstarPort;
}

int ProtocolPorts::ysfPort()
{
    return m_ysfPort;
}

int ProtocolPorts::dmrPort()
{
    return m_dmrPort;
}

void ProtocolPorts::setNXDNPort(int port)
{
    m_nxdnPort = port;
}

int ProtocolPorts::nxdnPort()
{
    return m_nxdnPort;
}
