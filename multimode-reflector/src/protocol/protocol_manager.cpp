#include "protocol_manager.h"

std::unordered_map<
    ProtocolType,
    std::shared_ptr<
        ProtocolInterface>>
ProtocolManager::m_protocols;

void ProtocolManager::registerProtocol(
    ProtocolType type,
    std::shared_ptr<
        ProtocolInterface> proto)
{
    m_protocols[type] = proto;
}

ProtocolInterface* ProtocolManager::get(
    ProtocolType type)
{
    auto it =
        m_protocols.find(type);

    if (it == m_protocols.end())
        return nullptr;

    return it->second.get();
}
