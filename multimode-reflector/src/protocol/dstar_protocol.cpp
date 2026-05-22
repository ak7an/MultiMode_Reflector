#include "dstar_protocol.h"

#include "dstar_header.h"
#include "dstar_session.h"

#include "../core/logger.h"
#include "../core/lastheard_manager.h"
#include "../core/loop_guard.h"
#include "../core/jitter_buffer.h"
#include "../core/media_frame.h"
#include "../core/media_router.h"

#include <sstream>
#include <iomanip>

ProtocolResult DStarProtocol::handle(
    const uint8_t* data,
    size_t length,
    const std::string& peer)
{
    ProtocolResult result{};
    result.forwardCurrent = false;

    std::stringstream ss;

    ss << "D-Star frame from "
       << peer
       << " len="
       << length
       << " data=";

    size_t dumpLen =
        (length > 16)
            ? 16
            : length;

    for (size_t i = 0;
         i < dumpLen;
         ++i)
    {
        ss << std::hex
           << std::setw(2)
           << std::setfill('0')
           << static_cast<int>(data[i])
           << " ";
    }

    Logger::log(INFO, ss.str());

    DStarHeader hdr =
        DStarHeaderParser::parse(
            data,
            length);

    if (hdr.valid) {

        DStarSessionManager::createOrUpdate(
            hdr.streamId,
            peer,
            hdr.mycall,
            hdr.urcall,
            hdr.rpt1,
            hdr.rpt2);

        LastHeardManager::addEntry(
            hdr.mycall,
            peer,
            hdr.streamId);

        Logger::log(INFO,
            "D-Star Header:"
            " MYCALL=" + hdr.mycall +
            " URCALL=" + hdr.urcall +
            " RPT1=" + hdr.rpt1 +
            " RPT2=" + hdr.rpt2 +
            " STREAMID=" +
            std::to_string(hdr.streamId));

        LastHeardManager::dump();

        result.forwardCurrent = true;
        return result;
    }

    if (length >= 15) {

        uint16_t streamId =
            (data[12] << 8) |
             data[13];

        uint8_t sequence =
            data[14];

        bool endOfTransmission =
            (sequence & 0x40);

        MediaFrame media{};

        media.protocol =
            MediaProtocol::DSTAR;

        media.streamId =
            streamId;

        media.sequence =
            sequence & 0x1F;

        media.endOfTransmission =
            endOfTransmission;

        media.sourcePeer =
            peer;

        media.payload.assign(
            data,
            data + length);

        Logger::log(INFO,
            "MediaFrame created:"
            " PROTO=DSTAR"
            " STREAMID=" +
            std::to_string(media.streamId) +
            " SEQ=" +
            std::to_string(media.sequence) +
            " EOT=" +
            std::to_string(media.endOfTransmission));

        MediaRouteResult routeResult =
            MediaRouter::route(
                media);

         if (routeResult.action ==
    RouteAction::DROP)
{

       return result;
}


        if (!DStarSessionManager::hasStream(
                streamId))
        {
            Logger::log(INFO,
                "Ignoring voice frame for unknown stream: " +
                std::to_string(streamId));

            return result;
        }

        if (LoopGuard::seenRecently(
                streamId,
                sequence,
                peer))
        {
            Logger::log(INFO,
                "Loop suppressed for stream " +
                std::to_string(streamId));

            return result;
        }

        if (!DStarSessionManager::acceptSequence(
                streamId,
                sequence))
        {
            return result;
        }

        JitterResult jitterResult =
            JitterBuffer::observe(
                media);

        JitterBuffer::dump();

        for (const auto& frame :
             jitterResult.releasedFrames)
        {
            ProtocolFrame pf{};

            pf.payload =
                frame.payload;

            result.extraFrames.push_back(
                pf);

            Logger::log(INFO,
                "D-Star buffered frame ready:"
                " STREAMID=" +
                std::to_string(streamId) +
                " SEQ=" +
                std::to_string(frame.sequence) +
                " LEN=" +
                std::to_string(
                    frame.payload.size()));
        }

        if (!jitterResult.releaseCurrent) {

            Logger::log(INFO,
                "D-Star frame held by jitter buffer:"
                " STREAMID=" +
                std::to_string(streamId) +
                " SEQ=" +
                std::to_string(sequence & 0x1F));

            return result;
        }

        DStarSessionManager::touchStream(
            streamId);

        Logger::log(INFO,
            "D-Star Voice Frame:"
            " STREAMID=" +
            std::to_string(streamId) +
            " SEQ=" +
            std::to_string(sequence) +
            " LEN=" +
            std::to_string(length));

        if (endOfTransmission) {

            Logger::log(INFO,
                "D-Star EOT detected:"
                " STREAMID=" +
                std::to_string(streamId));

            DStarSessionManager::endStream(
                streamId);
        }

        result.forwardCurrent = true;
        return result;
    }

    return result;
}
