#include "serial_port.h"

#include "logger.h"

#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <sys/select.h>

static speed_t baudToSpeed(
    int baudRate)
{
    switch (baudRate)
    {
    case 9600:
        return B9600;

    case 19200:
        return B19200;

    case 38400:
        return B38400;

    case 57600:
        return B57600;

    case 115200:
        return B115200;

    case 230400:
        return B230400;

    case 460800:
        return B460800;

    default:
        return B115200;
    }
}

SerialPort::SerialPort() :
    m_fd(-1)
{
}

SerialPort::~SerialPort()
{
    closePort();
}

bool SerialPort::openPort(
    const std::string& device,
    int baudRate)
{
    closePort();

    m_fd =
        ::open(
            device.c_str(),
            O_RDWR | O_NOCTTY | O_NONBLOCK);

    if (m_fd < 0)
    {
        Logger::log(WARN,
            "SerialPort open failed: " +
            device);

        return false;
    }

    termios tty {};

    if (tcgetattr(
            m_fd,
            &tty) != 0)
    {
        Logger::log(WARN,
            "SerialPort tcgetattr failed: " +
            device);

        closePort();
        return false;
    }

    cfmakeraw(
        &tty);

    speed_t speed =
        baudToSpeed(
            baudRate);

    cfsetispeed(
        &tty,
        speed);

    cfsetospeed(
        &tty,
        speed);

    tty.c_cflag |=
        CLOCAL | CREAD;

    tty.c_cflag &=
        ~CRTSCTS;

    tty.c_cc[VMIN] =
        0;

    tty.c_cc[VTIME] =
        0;

    if (tcsetattr(
            m_fd,
            TCSANOW,
            &tty) != 0)
    {
        Logger::log(WARN,
            "SerialPort tcsetattr failed: " +
            device);

        closePort();
        return false;
    }

    Logger::log(INFO,
        "SerialPort opened: " +
        device +
        " BAUD=" +
        std::to_string(baudRate));

    return true;
}

void SerialPort::closePort()
{
    if (m_fd >= 0)
    {
        ::close(
            m_fd);

        m_fd = -1;
    }
}

bool SerialPort::isOpen() const
{
    return m_fd >= 0;
}

bool SerialPort::writeBytes(
    const std::vector<uint8_t>& data)
{
    if (m_fd < 0)
    {
        return false;
    }

    ssize_t written =
        ::write(
            m_fd,
            data.data(),
            data.size());

    if (written !=
        static_cast<ssize_t>(
            data.size()))
    {
        return false;
    }

    tcdrain(
        m_fd);

    return true;
}

bool SerialPort::readBytes(
    std::vector<uint8_t>& data,
    size_t maxBytes,
    int timeoutMs)
{
    data.clear();

    if (m_fd < 0)
    {
        return false;
    }

    fd_set readSet;
    FD_ZERO(
        &readSet);
    FD_SET(
        m_fd,
        &readSet);

    timeval timeout {};
    timeout.tv_sec =
        timeoutMs / 1000;
    timeout.tv_usec =
        (timeoutMs % 1000) * 1000;

    int ready =
        select(
            m_fd + 1,
            &readSet,
            nullptr,
            nullptr,
            &timeout);

    if (ready <= 0)
    {
        return false;
    }

    std::vector<uint8_t> buffer(
        maxBytes);

    ssize_t bytesRead =
        ::read(
            m_fd,
            buffer.data(),
            buffer.size());

    if (bytesRead <= 0)
    {
        return false;
    }

    data.assign(
        buffer.begin(),
        buffer.begin() + bytesRead);

    return true;
}
