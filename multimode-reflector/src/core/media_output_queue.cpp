#include "media_output_queue.h"

#include "logger.h"

std::queue<MediaFrame> MediaOutputQueue::m_queue;
std::mutex MediaOutputQueue::m_mutex;

static constexpr size_t MAX_QUEUE_DEPTH =
    100;

void MediaOutputQueue::push(
    const MediaFrame& frame)
{
    std::lock_guard<std::mutex> lock(
        m_mutex);

    if (m_queue.size() >= MAX_QUEUE_DEPTH) {

        Logger::log(WARN,
            "MediaOutputQueue overload drop:"
            " STREAMID=" +
            std::to_string(frame.streamId) +
            " SEQ=" +
            std::to_string(frame.sequence) +
            " SIZE=" +
            std::to_string(m_queue.size()));

        return;
    }

    m_queue.push(
        frame);

    Logger::log(INFO,
        "MediaOutputQueue push:"
        " STREAMID=" +
        std::to_string(frame.streamId) +
        " SEQ=" +
        std::to_string(frame.sequence) +
        " SIZE=" +
        std::to_string(m_queue.size()));
}

bool MediaOutputQueue::pop(
    MediaFrame& frame)
{
    std::lock_guard<std::mutex> lock(
        m_mutex);

    if (m_queue.empty()) {
        return false;
    }

    frame =
        m_queue.front();

    m_queue.pop();

    return true;
}

size_t MediaOutputQueue::size()
{
    std::lock_guard<std::mutex> lock(
        m_mutex);

    return m_queue.size();
}
