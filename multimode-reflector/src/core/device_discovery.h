#pragma once

#include <string>
#include <vector>

struct USBSerialDevice
{
    std::string devicePath;
    std::string stablePath;
    std::string idName;
};

class DeviceDiscovery
{
public:

    static std::vector<USBSerialDevice>
    findUSBSerialDevices();

    static std::vector<USBSerialDevice>
    findFTDIDevices();
};
