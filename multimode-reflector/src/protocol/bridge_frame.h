#pragma once

/*
    BridgeFrame

    Internal protocol-independent bridge transport.

    Purpose:

    - internal media routing
    - transcoder transport
    - regression testing
    - protocol-independent frame movement

    This is NOT a real network protocol.

    Real network protocols remain:

    - DSVT (D-Star)
    - YSFD (YSF)
    - future DMR/NXDN/P25/M17 formats

    Current synthetic bridge formats:

    - DSBB
    - YSFB

    may eventually converge into a unified
    internal bridge transport.
*/

enum class BridgeFrameType {

    INTERNAL_MEDIA,
    INTERNAL_CONTROL
};
