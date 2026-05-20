#pragma once

#include <memory>
#include <unordered_map>

#include "protocol_interface.h"
#include "protocol_definitions.h"

class ProtocolManager {
public:

    static void registerProtocol(
        ProtocolType type,
        std::shared_ptr<
            ProtocolInterface> proto);

    static ProtocolInterface* get(
        ProtocolType type);

private:

    static std::unordered_map<
        ProtocolType,
        std::shared_ptr<
            ProtocolInterface>> m_protocols;
};
