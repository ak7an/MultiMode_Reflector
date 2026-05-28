#include "protocol_name.h"

std::string ProtocolName::toString(
    ProtocolType protocol)
{
    switch (protocol)
    {
    case ProtocolType::DSTAR:
        return "DSTAR";

    case ProtocolType::DMR:
        return "DMR";

    case ProtocolType::YSF:
        return "YSF";

    case ProtocolType::NXDN:
        return "NXDN";

    case ProtocolType::P25:
        return "P25";

    case ProtocolType::M17:
        return "M17";

    default:
        return "UNKNOWN";
    }
}
