#include "packet_capture.h"

#include <fstream>
#include <iomanip>
#include <mutex>
#include <sstream>
#include <chrono>

static std::mutex g_captureMutex;

void PacketCapture::record(
    const std::string& proto,
    const std::vector<uint8_t>& packet)
{
    std::lock_guard<std::mutex> lock(
        g_captureMutex);

    static std::ofstream out(
        "packet_capture.log",
        std::ios::app);

    auto now =
        std::chrono::system_clock::now();

    auto ms =
        std::chrono::duration_cast<
            std::chrono::milliseconds>(
                now.time_since_epoch()).count();

    out << ms
        << " "
        << proto
        << " ";

    for (uint8_t b : packet) {

        out << std::hex
            << std::setw(2)
            << std::setfill('0')
            << static_cast<int>(b)
            << " ";
    }

    out << "\n";

    out.flush();
}
