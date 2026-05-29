#pragma once

#include <chrono>
#include <string>

struct AMBEDeviceStatus
{
    bool decodePresent = false;
    bool encodePresent = false;

    bool decodeOpen = false;
    bool encodeOpen = false;

    bool decodeResponsive = false;
    bool encodeResponsive = false;

    bool ready = false;

    std::chrono::steady_clock::time_point lastDecodeResponse {};
    std::chrono::steady_clock::time_point lastEncodeResponse {};
};

class AMBEDeviceStatusReporter
{
public:

    static void logStatus(
        const AMBEDeviceStatus& status);
};
