#include "protocol_listener_registry.h"

#include "../core/protocol_config.h"
#include "../core/protocol_ports.h"

std::vector<ProtocolListener>
ProtocolListenerRegistry::listeners()
{
    std::vector<ProtocolListener> result;

    if (ProtocolConfig::dstarEnabled()) {
        result.push_back({
            ProtocolType::DSTAR,
            ProtocolPorts::dstarPort()
        });
    }

    if (ProtocolConfig::ysfEnabled()) {
        result.push_back({
            ProtocolType::YSF,
            ProtocolPorts::ysfPort()
        });
    }

    if (ProtocolConfig::dmrEnabled()) {
        result.push_back({
            ProtocolType::DMR,
            ProtocolPorts::dmrPort()
        });
    }

    return result;
}
