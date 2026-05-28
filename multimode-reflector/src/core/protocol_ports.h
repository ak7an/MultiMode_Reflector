#pragma once

class ProtocolPorts
{
public:
    static void setDStarPort(int port);
    static void setYSFPort(int port);
    static void setDMRPort(int port);

    static int dstarPort();
    static int ysfPort();
    static int dmrPort();

private:
    static int m_dstarPort;
    static int m_ysfPort;
    static int m_dmrPort;
};
