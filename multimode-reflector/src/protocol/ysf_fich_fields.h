#pragma once

#include <cstdint>

struct YSFFichFields {

    uint8_t frameInformation = 0x01;

    uint8_t communicationType = 0x00;

    uint8_t dataType = 0x02;

    uint8_t callMode = 0x00;
};
