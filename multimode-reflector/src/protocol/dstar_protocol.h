#pragma once

#include <cstdint>
#include <string>

class DStarProtocol {
public:
    static void handle(const uint8_t* data,
                       size_t length,
                       const std::string& peer);
};
