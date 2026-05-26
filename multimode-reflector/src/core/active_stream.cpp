#include "active_stream.h"

#include "logger.h"

bool ActiveStream::m_active = false;
MediaProtocol ActiveStream::m_protocol = MediaProtocol::UNKNOWN;
uint16_t ActiveStream::m_streamId = 0;

bool ActiveStream::accept(
    const MediaFrame& frame)
{
    if (!m_active) {

        m_active = true;
        m_protocol = frame.protocol;
        m_streamId = frame.streamId;

        Logger::log(INFO,
            "ActiveStream started:"
            " STREAMID=" +
            std::to_string(frame.streamId));

        return true;
    }

    if (m_protocol == frame.protocol &&
        m_streamId == frame.streamId)
    {
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
    }
}
