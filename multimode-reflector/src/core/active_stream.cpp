#include "active_stream.h"

#include "logger.h"

bool ActiveStream::m_active = false;
MediaProtocol ActiveStream::m_protocol = MediaProtocol::UNKNOWN;
uint16_t ActiveStream::m_streamId = 0;

std::chrono::steady_clock::time_point
ActiveStream::m_startedAt;

std::chrono::steady_clock::time_point
ActiveStream::m_lastSeen;

bool ActiveStream::accept(
    const MediaFrame& frame)
{
    auto now =
        std::chrono::steady_clock::now();

    if (!m_active) {

        m_active = true;
        m_protocol = frame.protocol;
        m_streamId = frame.streamId;
        m_startedAt = now;
        m_lastSeen = now;

        Logger::log(INFO,
            "ActiveStream started:"
            " STREAMID=" +
            std::to_string(frame.streamId));

        return true;
    }

    if (m_protocol == frame.protocol &&
        m_streamId == frame.streamId)
    {
        m_lastSeen = now;
        return true;
    }

    Logger::log(WARN,
        "ActiveStream rejected competing stream:"
        " ACTIVE=" +
        std::to_string(m_streamId) +
        " REJECTED=" +
        std::to_string(frame.streamId));

    return false;
}

void ActiveStream::end(
    MediaProtocol protocol,
    uint16_t streamId)
{
    if (m_active &&
        m_protocol == protocol &&
        m_streamId == streamId)
    {
        Logger::log(INFO,
            "ActiveStream ended:"
            " STREAMID=" +
            std::to_string(streamId));

        m_active = false;
        m_protocol = MediaProtocol::UNKNOWN;
        m_streamId = 0;
        m_startedAt =
            std::chrono::steady_clock::time_point{};
        m_lastSeen =
            std::chrono::steady_clock::time_point{};
    }
}

void ActiveStream::checkTimeout(
    int idleTimeoutMs,
    int maxTxMs)
{
    if (!m_active) {
        return;
    }

    auto now =
        std::chrono::steady_clock::now();

    auto idleAgeMs =
        std::chrono::duration_cast<
            std::chrono::milliseconds>(
                now - m_lastSeen).count();

    auto txAgeMs =
        std::chrono::duration_cast<
            std::chrono::milliseconds>(
                now - m_startedAt).count();

    if (idleAgeMs > idleTimeoutMs) {

        Logger::log(WARN,
            "ActiveStream idle timeout:"
            " STREAMID=" +
            std::to_string(m_streamId) +
            " IDLE_MS=" +
            std::to_string(idleAgeMs));

        m_active = false;
        m_protocol = MediaProtocol::UNKNOWN;
        m_streamId = 0;
        m_startedAt =
            std::chrono::steady_clock::time_point{};
        m_lastSeen =
            std::chrono::steady_clock::time_point{};

        return;
    }

    if (txAgeMs > maxTxMs) {

        Logger::log(WARN,
            "ActiveStream TX timeout:"
            " STREAMID=" +
            std::to_string(m_streamId) +
            " TX_MS=" +
            std::to_string(txAgeMs));

        m_active = false;
        m_protocol = MediaProtocol::UNKNOWN;
        m_streamId = 0;
        m_startedAt =
            std::chrono::steady_clock::time_point{};
        m_lastSeen =
            std::chrono::steady_clock::time_point{};
    }
}
