#include "xlxd_peer_registrar.h"

#include <arpa/inet.h>

#include "../core/xlxd_peer_config.h"
#include "../core/logger.h"

void XLXDPeerRegistrar::registerConfiguredPeer(
    ProtocolPeerRegistry& registry)
{
    if (!XLXDPeerConfig::enabled())
    {
        Logger::log(INFO,
            "XLXD peering disabled");

        return;
    }

    ProtocolPeer peer;

    peer.protocol =
        ProtocolType::DSTAR;

    peer.host =
        XLXDPeerConfig::host();

    peer.port =
        XLXDPeerConfig::port();

    peer.reflector =
        XLXDPeerConfig::reflector();

    peer.module =
        XLXDPeerConfig::module();

    peer.address.sin_family =
        AF_INET;

    peer.address.sin_port =
        htons(peer.port);

    inet_pton(
        AF_INET,
        peer.host.c_str(),
        &peer.address.sin_addr);

    registry.addPeer(peer);

    Logger::log(INFO,
        "Configured XLXD peer registered: REFLECTOR=" +
        XLXDPeerConfig::reflector() +
        " HOST=" +
        peer.host +
        " PORT=" +
        std::to_string(peer.port) +
        " MODULE=" +
        std::string(1, XLXDPeerConfig::module()));
}
