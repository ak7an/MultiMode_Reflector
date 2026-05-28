#pragma once

#include <string>

class XLXDPeerConfig
{
public:

    static bool enabled();

    static std::string reflector();

    static std::string host();

    static int port();

    static char module();
};
