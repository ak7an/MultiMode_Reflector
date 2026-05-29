#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

class SerialPort
{
public:

    SerialPort();

    ~SerialPort();

    bool openPort(
        const std::string& device,
        int baudRate);

    void closePort();

    bool isOpen() const;

    bool writeBytes(
        const std::vector<uint8_t>& data);

    bool readBytes(
        std::vector<uint8_t>& data,
        size_t maxBytes,
        int timeoutMs);

private:

    int m_fd;
};
