#pragma once

#include <string>

#include "protocol_definitions.h"

class ProtocolName
{
public:
    static std::string toString(
        ProtocolType protocol);
};
