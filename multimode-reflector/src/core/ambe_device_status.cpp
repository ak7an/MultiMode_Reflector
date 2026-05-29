#include "ambe_device_status.h"

#include "logger.h"

void AMBEDeviceStatusReporter::logStatus(
    const AMBEDeviceStatus& status)
{
    Logger::log(INFO,
        "AMBE STATUS:"
        " DECODE_PRESENT=" +
        std::to_string(status.decodePresent) +
        " ENCODE_PRESENT=" +
        std::to_string(status.encodePresent) +
        " DECODE_OPEN=" +
        std::to_string(status.decodeOpen) +
        " ENCODE_OPEN=" +
        std::to_string(status.encodeOpen) +
        " DECODE_RESPONSIVE=" +
        std::to_string(status.decodeResponsive) +
        " ENCODE_RESPONSIVE=" +
        std::to_string(status.encodeResponsive) +
        " READY=" +
        std::to_string(status.ready));
}
