#include "device_discovery.h"

#include "logger.h"

#include <filesystem>

namespace fs = std::filesystem;

static std::string resolvePath(
    const fs::path& path)
{
    try
    {
        return fs::canonical(path).string();
    }
    catch (...)
    {
        return "";
    }
}

std::vector<USBSerialDevice>
DeviceDiscovery::findUSBSerialDevices()
{
    std::vector<USBSerialDevice> devices;

    fs::path byIdPath(
        "/dev/serial/by-id");

    if (!fs::exists(byIdPath))
    {
        Logger::log(INFO,
            "DeviceDiscovery: /dev/serial/by-id not found");

        return devices;
    }

    for (const auto& entry :
         fs::directory_iterator(byIdPath))
    {
        USBSerialDevice device;

        device.stablePath =
            entry.path().string();

        device.devicePath =
            resolvePath(
                entry.path());

        device.idName =
            entry.path().filename().string();

        if (!device.devicePath.empty())
        {
            devices.push_back(
                device);

            Logger::log(INFO,
                "DeviceDiscovery serial device:"
                " STABLE=" +
                device.stablePath +
                " DEVICE=" +
                device.devicePath +
                " ID=" +
                device.idName);
        }
    }

    return devices;
}

std::vector<USBSerialDevice>
DeviceDiscovery::findFTDIDevices()
{
    std::vector<USBSerialDevice> result;

    auto devices =
        findUSBSerialDevices();

    for (const auto& device :
         devices)
    {
        if (device.idName.find("FTDI") !=
                std::string::npos ||
            device.idName.find("ftdi") !=
                std::string::npos)
        {
            result.push_back(
                device);
        }
    }

    Logger::log(INFO,
        "DeviceDiscovery FTDI count: " +
        std::to_string(result.size()));

    return result;
}
