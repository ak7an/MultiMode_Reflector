#include "dstar_header.h"

#include <cstring>

static std::string extractString(
    const uint8_t* data,
    size_t offset,
    size_t len)
{
    char buf[32];

    memset(buf, 0, sizeof(buf));

    memcpy(buf, data + offset, len);

    return std::string(buf);
}

DStarHeader DStarHeaderParser::parse(
    const uint8_t* data,
    size_t length)
{
    DStarHeader hdr{};

    hdr.valid = false;

    /*
     * Very simplified DSVT header layout
     * for initial framework development
     */

    if (length < 56)
        return hdr;

    /*
     * Stream ID
     */
    hdr.streamId =
        (data[12] << 8) |
         data[13];

    /*
     * Callsigns
     */
    hdr.rpt2 =
        extractString(data, 18, 8);

    hdr.rpt1 =
        extractString(data, 26, 8);

    hdr.urcall =
        extractString(data, 34, 8);

    hdr.mycall =
        extractString(data, 42, 8);

    hdr.valid = true;

    return hdr;
}
