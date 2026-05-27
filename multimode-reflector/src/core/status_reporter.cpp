#include "status_reporter.h"

#include "active_stream.h"
#include "logger.h"

#include <chrono>

static std::string protocolToString(
    MediaProtocol protocol)
{
    switch (protocol) {

    case MediaProtocol::DSTAR:
        return "DSTAR";

    case MediaProtocol::DMR:
        return "DMR";

    case MediaProtocol::YSF:
        return "YSF";

    case MediaProtocol::P25:
        return "P25";

    case MediaProtocol::NXDN:
        return "NXDN";

    case MediaProtocol::M17:
        return "M17";

    default:
        return "UNKNOWN";
    }
}

void StatusReporter::logActiveStream()
{
    static auto lastLog =
        std::chrono::steady_clock::now();

    auto now =
        std::chrono::steady_clock::now();

    if (std::chrono::duration_cast<
            std::chrono::seconds>(
                now - lastLog).count() < 5)
    {
        return;
    }

    lastLog = now;

    auto status =
        ActiveStream::status();

    if (!status.active &&
        !status.hangActive)
    {
        Logger::log(INFO,
            "Status: reflector idle");

        return;
    }

    Logger::log(INFO,
        "Status:"
        " ACTIVE=" +
        std::to_string(status.active) +
        " HANG=" +
        std::to_string(status.hangActive) +
        " CALLSIGN=" +
        status.callsign +
        " PROTO=" +
        protocolToString(status.protocol) +
        " STREAMID=" +
        std::to_string(status.streamId) +
        " TX_MS=" +
        std::to_string(status.txAgeMs) +
        " IDLE_MS=" +
        std::to_string(status.idleAgeMs) +
        " HANG_LEFT_MS=" +
        std::to_string(status.hangRemainingMs));
}
