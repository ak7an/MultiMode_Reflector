#include "hex_dump.h"

#include <iomanip>
#include <sstream>

std::string HexDump::toHex(
    const std::vector<uint8_t>& data)
{
    std::ostringstream ss;

    for (size_t i = 0; i < data.size(); ++i)
    {
        if (i)
            ss << ' ';

        ss << std::hex
           << std::uppercase
           << std::setw(2)
           << std::setfill('0')
           << static_cast<int>(data[i]);
    }

    return ss.str();
}
