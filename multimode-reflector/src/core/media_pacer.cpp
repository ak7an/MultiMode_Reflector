#include "media_pacer.h"

#include "logger.h"

#include <chrono>
#include <thread>

void MediaPacer::pace(
    const MediaFrame& frame,
    int targetDelayMs)
{
    auto ageMs =
        std::chrono::duration_cast<
            std::chrono::milliseconds>(
                std::chrono::steady_clock::now() -
                frame.createdAt).count();

    if (ageMs < targetDelayMs) {

        int sleepMs =
            targetDelayMs -
            static_cast<int>(ageMs);

        Logger::log(INFO,
            "MediaPacer delay:"
            " STREAMID=" +
            std::to_string(frame.streamId) +
            " SLEEP_MS=" +
            std::to_string(sleepMs));

        std::this_thread::sleep_for(
            std::chrono::milliseconds(
                sleepMs));
    }
}
