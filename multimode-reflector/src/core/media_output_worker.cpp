#include "media_output_worker.h"

#include "active_stream.h"
#include "debug_udp_sender.h"
#include "../net/global_protocol_router.h"
#include "../net/xlxd_frame_packet.h"
#include "../protocol/protocol_definitions.h"
#include "../protocol/dstar_network_frame.h"
#include "logger.h"
#include "media_output_queue.h"
#include "media_pacer.h"
#include "media_timing.h"
#include "status_reporter.h"
#include "packet_capture.h"
#include "xlxd_peer_config.h"


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
            std::to_string(frame.sequence) +
            " REFLECTOR=" +
            frame.sourceReflector +
            " MODULE=" +
            std::string(1, frame.sourceModule));

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

        std::string captureProtocol =
            "UNKNOWN";

        switch (frame.protocol) {

        case MediaProtocol::DSTAR:
            captureProtocol = "DSTAR";
            break;

        case MediaProtocol::YSF:
            captureProtocol = "YSF";
            break;

        case MediaProtocol::DMR:
            captureProtocol = "DMR";
            break;

        case MediaProtocol::NXDN:
            captureProtocol = "NXDN";
            break;

        case MediaProtocol::P25:
            captureProtocol = "P25";
            break;

        case MediaProtocol::M17:
            captureProtocol = "M17";
            break;

        default:
            break;
        }

        PacketCapture::record(
            captureProtocol,
            packet);

        DebugUdpSender::sendToLocal(
            packet.data(),
            packet.size(),
            m_outputPort);

        ProtocolNetworkRouter* router =
            GlobalProtocolRouter::router();

        if (router != nullptr) {

            ProtocolType targetProto =
                ProtocolType::UNKNOWN;

            switch (frame.protocol) {

            case MediaProtocol::DSTAR:
                targetProto = ProtocolType::DSTAR;
                break;

            case MediaProtocol::YSF:
                targetProto = ProtocolType::YSF;
                break;

            case MediaProtocol::DMR:
                targetProto = ProtocolType::DMR;
                break;

            case MediaProtocol::NXDN:
                targetProto = ProtocolType::NXDN;
                break;

            default:
                break;
            }

            std::vector<uint8_t> routedPacket =
                packet;

            if (frame.protocol == MediaProtocol::DSTAR &&
                XLXDPeerConfig::enabled())
            {
                std::string xlxdPeerPrefix =
                    XLXDPeerConfig::host() + ":";

                bool frameCameFromXLXDPeer =
                    frame.sourcePeer.rfind(
                        xlxdPeerPrefix,
                        0) == 0;

                if (frameCameFromXLXDPeer)
                {
                    Logger::log(INFO,
                        "XLXD outbound route suppressed to avoid peer loop:"
                        " STREAMID=" +
                        std::to_string(frame.streamId) +
                        " SEQ=" +
                        std::to_string(frame.sequence));

                    routedPacket.clear();
                }
                else
                {
                    XLXDFrameData xlxdFrame;
                    xlxdFrame.reflector =
                        XLXDPeerConfig::reflector();
                    xlxdFrame.module =
                        XLXDPeerConfig::module();
                    xlxdFrame.payload =
                        DStarNetworkFrame::build(
                            frame);

                    routedPacket =
                        XLXDFramePacket::build(
                            xlxdFrame);

                    Logger::log(INFO,
                        "D-Star packet wrapped for XLXD route:"
                        " STREAMID=" +
                        std::to_string(frame.streamId) +
                        " SEQ=" +
                        std::to_string(frame.sequence) +
                        " XLXD_LEN=" +
                        std::to_string(
                            routedPacket.size()));
                }
            }

            if (!routedPacket.empty())
            {
                router->routePacket(
                    targetProto,
                    routedPacket);
            }
        }


        if (frame.endOfTransmission) {

            MediaPacer::reset(
                frame.protocol,
                frame.streamId);
        }
    }
}
