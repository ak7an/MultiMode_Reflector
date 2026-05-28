#include "xlxd_peer_config.h"

#include "../util/config.h"

bool XLXDPeerConfig::enabled()
{
    Config config;
    config.load("reflector.ini");

    return config.getInt(
        "xlxd_peering_enabled",
        0) != 0;
}

std::string XLXDPeerConfig::reflector()
{
    Config config;
    config.load("reflector.ini");

    return config.getString(
        "xlxd_peer_reflector",
        "XLX000");
}

std::string XLXDPeerConfig::host()
{
    Config config;
    config.load("reflector.ini");

    return config.getString(
        "xlxd_peer_host",
        "127.0.0.1");
}

int XLXDPeerConfig::port()
{
    Config config;
    config.load("reflector.ini");

    return config.getInt(
        "xlxd_peer_port",
        10001);
}

char XLXDPeerConfig::module()
{
    Config config;
    config.load("reflector.ini");

    std::string value =
        config.getString(
            "xlxd_peer_module",
            "A");

    if (value.empty()) {
        return 'A';
    }

    return value[0];
}
