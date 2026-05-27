#include "media_output_worker.h"

#include "active_stream.h"
#include "debug_udp_sender.h"
#include "logger.h"
#include "media_output_queue.h"
#include "media_pacer.h"
#include "media_timing.h"
#include "status_reporter.h"
#include "packet_capture.h"


static std::string frameTypeToString(
    MediaFrameType type)
{
    switch (type) {

    case MediaFrameType::HEADER:
        return "HEADER";

    case MediaFrameType::VOICE:
        return "VOICE";

    case MediaFrameType::VOICE_EOT:
        return "VOICE_EOT";

    case MediaFrameType::CONTROL:
        return "CONTROL";

    default:
        return "UNKNOWN";
    }
}


#include "../protocol/protocol_encoder.h"

#include <chrono>
#include <thread>

bool MediaOutputWorker::m_running = false;
int MediaOutputWorker::m_idleTimeoutMs = 15000;
int MediaOutputWorker::m_maxTxMs = 180000;
int MediaOutputWorker::m_outputPort = 9001;

void MediaOutputWorker::start(
    int idleTimeoutMs,
    int maxTxMs,
    int outputPort)
{
    if (m_running) {
        return;
    }

    m_idleTimeoutMs = idleTimeoutMs;
    m_maxTxMs = maxTxMs;
    m_outputPort = outputPort;
    m_outputPort = outputPort;

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
            m_idleTimeoutMs,
            m_maxTxMs);

        static auto lastStatus =
            std::chrono::steady_clock::now();

        auto now =
            std::chrono::steady_clock::now();

        if (std::chrono::duration_cast<
                std::chrono::seconds>(
                    now - lastStatus).count() >= 5)
        {
            StatusReporter::logActiveStream();

            lastStatus = now;
        }


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
            " TYPE=" +
            frameTypeToString(frame.frameType) +
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

        PacketCapture::record(
            "YSF",
            packet);

        DebugUdpSender::sendToLocal(
            packet.data(),
            packet.size(),
            m_outputPort);


        if (frame.endOfTransmission) {

            MediaPacer::reset(
                frame.protocol,
                frame.streamId);
        }
    }
}
