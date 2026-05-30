#pragma once

#include <cstddef>
#include <cstdint>

class ProtocolCRC
{
public:
    /*
     * Simple CRC-8 scaffold.
     *
     * This is intentionally protocol-neutral.
     * It gives protocol builders a deterministic integrity byte
     * while full protocol-specific CRC/FEC implementations are developed.
     */
    static uint8_t crc8(
        const uint8_t* data,
        size_t length)
    {
        uint8_t crc = 0x00;

        for (size_t i = 0; i < length; ++i)
        {
            crc ^= data[i];

            for (int bit = 0; bit < 8; ++bit)
            {
                if (crc & 0x80)
                    crc = static_cast<uint8_t>((crc << 1) ^ 0x07);
                else
                    crc = static_cast<uint8_t>(crc << 1);
            }
        }

        return crc;
    }

    static uint16_t crc16Ccitt(
        const uint8_t* data,
        size_t length,
        uint16_t initial = 0xFFFF)
    {
        uint16_t crc = initial;

        for (size_t i = 0; i < length; ++i)
        {
            crc ^= static_cast<uint16_t>(data[i]) << 8;

            for (int bit = 0; bit < 8; ++bit)
            {
                if (crc & 0x8000)
                    crc = static_cast<uint16_t>((crc << 1) ^ 0x1021);
                else
                    crc = static_cast<uint16_t>(crc << 1);
            }
        }

        return crc;
    }
};
