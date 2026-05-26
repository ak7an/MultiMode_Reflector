#include "media_output_worker.h"

#include "logger.h"
#include "media_output_queue.h"

#include <chrono>
#include <thread>

bool MediaOutputWorker::m_running = false;

void MediaOutputWorker::start()
{
    if (m_running) {
        return;
    }

    m_running = true;

    std::thread worker(
        MediaOutputWorker::run);

    worker.detach();

    Logger::log(INFO,
        "MediaOutputWorker started");
}

void MediaOutputWorker::stop()
{
    m_running = false;

    Logger::log(INFO,
        "MediaOutputWorker stopped");
}

void MediaOutputWorker::run()
{
    while (m_running) {

        MediaFrame frame{};

        if (MediaOutputQueue::pop(
                frame))
        {
            Logger::log(INFO,
                "MediaOutputWorker popped:"
                " STREAMID=" +
                std::to_string(frame.streamId) +
                " SEQ=" +
                std::to_string(frame.sequence));
        }
        else {

            std::this_thread::sleep_for(
                std::chrono::milliseconds(5));
        }
    }
}
