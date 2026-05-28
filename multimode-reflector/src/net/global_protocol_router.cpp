#include "global_protocol_router.h"

ProtocolNetworkRouter*
GlobalProtocolRouter::m_router =
    nullptr;

void GlobalProtocolRouter::setRouter(
    ProtocolNetworkRouter* router)
{
    m_router = router;
}

ProtocolNetworkRouter*
GlobalProtocolRouter::router()
{
    return m_router;
}
