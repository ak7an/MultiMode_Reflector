#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

class ProtocolBitBuffer
{
public:
    void pushBit(
        bool bit)
    {
        const size_t byteIndex =
            m_bitCount / 8;

        const size_t bitIndex =
            7 - (m_bitCount % 8);

        if (byteIndex >= m_bytes.size())
            m_bytes.push_back(0);

        if (bit)
            m_bytes[byteIndex] |= static_cast<uint8_t>(1U << bitIndex);

        ++m_bitCount;
    }

    void pushBits(
        uint32_t value,
        size_t count)
    {
        for (size_t i = 0; i < count; ++i)
        {
            const size_t shift =
                count - 1 - i;

            pushBit(
                ((value >> shift) & 0x01U) != 0);
        }
    }

    const std::vector<uint8_t>& bytes() const
    {
        return m_bytes;
    }

    size_t bitCount() const
    {
        return m_bitCount;
    }

    void clear()
    {
        m_bytes.clear();
        m_bitCount = 0;
    }

private:
    std::vector<uint8_t> m_bytes;
    size_t m_bitCount = 0;
};
