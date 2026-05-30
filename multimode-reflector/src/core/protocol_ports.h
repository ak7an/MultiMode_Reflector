#pragma once

class ProtocolPorts
{
public:
    static void setDStarPort(int port);
    static void setYSFPort(int port);
    static void setDMRPort(int port);
    static void setNXDNPort(int port);
    static void setP25Port(int port);

    static int dstarPort();
    static int ysfPort();
    static int dmrPort();
    static int nxdnPort();
    static int p25Port();

private:
    static int m_dstarPort;
    static int m_ysfPort;
    static int m_dmrPort;
    static int m_nxdnPort;
    static int m_p25Port;
};
