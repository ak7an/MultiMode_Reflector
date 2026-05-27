#include "protocol_detector.h"

#include <cstring>

ProtocolType ProtocolDetector::detect(
    const uint8_t* data,
    size_t length)
{
    if (length < 4)
        return ProtocolType::UNKNOWN;

    /*
     * D-Star examples:
     * "DSVT"
     */
    if (memcmp(data, "DSVT", 4) == 0)
        return ProtocolType::DSTAR;

    /*
     * Synthetic D-Star bridge/test packets:
     * "DSBB"
     */
    if (memcmp(data, "DSBB", 4) == 0)
        return ProtocolType::DSTAR;

    /*
     * YSF examples:
     * "YSFD"
     */
    if (memcmp(data, "YSFD", 4) == 0)
        return ProtocolType::YSF;

    /*
     * Synthetic YSF bridge/test packets:
     * "YSFB"
     */
    if (memcmp(data, "YSFB", 4) == 0)
        return ProtocolType::YSF;

    /*
     * M17 examples:
     * "M17 "
     */
    if (memcmp(data, "M17 ", 4) == 0)
        return ProtocolType::M17;

    /*
     * XLX examples:
     * "XLX"
     */
    if (memcmp(data, "XLX", 3) == 0)
        return ProtocolType::XLX;

    /*
     * Placeholder detection for:
     * DMR
     * NXDN
     * P25
     */

    return ProtocolType::UNKNOWN;
}
