#pragma once

#include "protocol_network_router.h"

class GlobalProtocolRouter
{
public:

    static void setRouter(
        ProtocolNetworkRouter* router);

    static ProtocolNetworkRouter*
    router();

private:

    static ProtocolNetworkRouter*
        m_router;
};
