#include "active_stream.h"

#include "logger.h"

bool ActiveStream::m_active = false;
MediaProtocol ActiveStream::m_protocol = MediaProtocol::UNKNOWN;
uint16_t ActiveStream::m_streamId = 0;

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
        m_lastSeen =
            std::chrono::steady_clock::time_point{};
    }
}

void ActiveStream::checkTimeout(
    int timeoutMs)
{
    if (!m_active) {
        return;
    }

    auto ageMs =
        std::chrono::duration_cast<
            std::chrono::milliseconds>(
                std::chrono::steady_clock::now() -
                m_lastSeen).count();

    if (ageMs > timeoutMs) {

        Logger::log(WARN,
            "ActiveStream timeout:"
            " STREAMID=" +
            std::to_string(m_streamId) +
            " AGE_MS=" +
            std::to_string(ageMs));

        m_active = false;
        m_protocol = MediaProtocol::UNKNOWN;
        m_streamId = 0;
        m_lastSeen =
            std::chrono::steady_clock::time_point{};
    }
}
