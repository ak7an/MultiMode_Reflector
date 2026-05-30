#include "ysf_voice_frame.h"

#include "../core/logger.h"

#include <algorithm>
#include <cstring>
#include <sstream>

namespace
{
    constexpr uint8_t SLOT_UNUSED = 0x00;
    constexpr uint8_t SLOT_VOICE  = 0x01;
    constexpr uint8_t SLOT_EOT    = 0x02;

    size_t availableAmbeBytes(
        const MediaFrame& frame,
        size_t offset)
    {
        if (offset >= frame.payload.size())
            return 0;

        return std::min(
            YSFVoiceFrame::AMBE_BYTES_PER_SLOT,
            frame.payload.size() - offset);
    }

    std::string payloadSummary(
        size_t input,
        size_t copied,
        size_t slots)
    {
        std::ostringstream ss;

        ss << " INPUT_PAYLOAD=" << input
           << " AMBE_BYTES_COPIED=" << copied
           << " VOICE_SLOTS_USED=" << slots
           << " YSF_PAYLOAD=" << YSFVoiceFrame::YSF_PAYLOAD_SIZE;

        return ss.str();
    }
}

std::vector<uint8_t> YSFVoiceFrame::build(
    const MediaFrame& frame)
{
    std::vector<uint8_t> payload(
        YSF_PAYLOAD_SIZE,
        0);

    /*
     * Current YSF voice-frame construction layer.
     *
     * This replaces the earlier blind memcpy/pad placeholder.
     *
     * The frame is now organized into five deterministic voice slots.
     * Each slot carries:
     *
     *   byte 0      slot marker
     *   byte 1      stream sequence
     *   byte 2      slot index
     *   byte 3      flags
     *   byte 4-12   AMBE payload bytes
     *   byte 13-17  reserved for future YSF VCH/FICH integration
     *
     * This is still inside the project scaffold network frame.
     * Full Yaesu-compatible VCH/FICH bit packing comes next.
     */

    size_t inputOffset = 0;
    size_t totalCopied = 0;
    size_t slotsUsed = 0;

    for (size_t slot = 0;
         slot < YSF_SLOT_COUNT;
         ++slot)
    {
        const size_t slotOffset =
            slot * YSF_SLOT_SIZE;

        const size_t copyLength =
            availableAmbeBytes(
                frame,
                inputOffset);

        if (copyLength == 0)
        {
            payload[slotOffset + 0] = SLOT_UNUSED;
            payload[slotOffset + 1] = frame.sequence;
            payload[slotOffset + 2] = static_cast<uint8_t>(slot);
            payload[slotOffset + 3] = 0x00;
            continue;
        }

        payload[slotOffset + 0] =
            frame.endOfTransmission ? SLOT_EOT : SLOT_VOICE;

        payload[slotOffset + 1] =
            frame.sequence;

        payload[slotOffset + 2] =
            static_cast<uint8_t>(slot);

        payload[slotOffset + 3] =
            frame.endOfTransmission ? 0x01 : 0x00;

        std::memcpy(
            &payload[slotOffset + 4],
            frame.payload.data() + inputOffset,
            copyLength);

        inputOffset += copyLength;
        totalCopied += copyLength;
        ++slotsUsed;
    }

    Logger::log(INFO,
        "YSFVoiceFrame build:"
        " STREAMID=" +
        std::to_string(frame.streamId) +
        payloadSummary(
            frame.payload.size(),
            totalCopied,
            slotsUsed));

    return payload;
}

bool YSFVoiceFrame::parse(
    const uint8_t* data,
    size_t length,
    MediaFrame& frame)
{
    if (data == nullptr)
        return false;

    if (length < YSF_PAYLOAD_SIZE)
        return false;

    frame.payload.clear();

    size_t slotsUsed = 0;

    for (size_t slot = 0;
         slot < YSF_SLOT_COUNT;
         ++slot)
    {
        const size_t slotOffset =
            slot * YSF_SLOT_SIZE;

        const uint8_t marker =
            data[slotOffset + 0];

        if (marker == SLOT_UNUSED)
            continue;

        if (marker != SLOT_VOICE &&
            marker != SLOT_EOT)
            continue;

        frame.sequence =
            data[slotOffset + 1];

        if (marker == SLOT_EOT)
            frame.endOfTransmission = true;

        frame.payload.insert(
            frame.payload.end(),
            data + slotOffset + 4,
            data + slotOffset + 4 + AMBE_BYTES_PER_SLOT);

        ++slotsUsed;
    }

    Logger::log(INFO,
        "YSFVoiceFrame parse:"
        " STREAMID=" +
        std::to_string(frame.streamId) +
        " VOICE_SLOTS_USED=" +
        std::to_string(slotsUsed) +
        " AMBE_PAYLOAD=" +
        std::to_string(frame.payload.size()));

    return slotsUsed > 0;
}
