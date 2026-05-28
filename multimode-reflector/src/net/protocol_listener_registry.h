#pragma once

#include <vector>

#include "protocol_listener.h"

class ProtocolListenerRegistry
{
public:
    static std::vector<ProtocolListener> listeners();
};
