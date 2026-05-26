#include "media_output_worker.h"

#include "active_stream.h"
#include "debug_udp_sender.h"
#include "logger.h"
#include "media_output_queue.h"
#include "media_pacer.h"
#include "media_timing.h"

#include "../protocol/protocol_encoder.h"

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

        ActiveStream::checkTimeout(
            15000);

        MediaFrame frame{};

        if (!MediaOutputQueue::pop(
                frame))
        {
            std::this_thread::sleep_for(
                std::chrono::milliseconds(5));

            continue;
        }

        Logger::log(INFO,
            "MediaOutputWorker popped:"
            " STREAMID=" +
            std::to_string(frame.streamId) +
            " SEQ=" +
            std::to_string(frame.sequence));

        auto ageMs =
            std::chrono::duration_cast<
                std::chrono::milliseconds>(
                    std::chrono::steady_clock::now() -
                    frame.createdAt).count();

        if (ageMs > 200) {

            Logger::log(WARN,
                "MediaOutputWorker stale drop:"
                " STREAMID=" +
                std::to_string(frame.streamId) +
                " SEQ=" +
                std::to_string(frame.sequence) +
                " AGE_MS=" +
                std::to_string(ageMs));

            continue;
        }

        MediaPacer::pace(
            frame,
            MediaTiming::targetDelayMs(
                frame.protocol));

        auto packet =
            ProtocolEncoder::encode(
                frame);

        if (packet.empty()) {

            Logger::log(INFO,
                "MediaOutputWorker empty encoded packet");

            continue;
        }

        Logger::log(INFO,
            "MediaOutputWorker encoded packet:"
            " LEN=" +
            std::to_string(packet.size()));

        DebugUdpSender::sendToLocal(
            packet.data(),
            packet.size(),
            9001);


        if (frame.endOfTransmission) {

            MediaPacer::reset(
                frame.protocol,
                frame.streamId);
        }
    }
}
