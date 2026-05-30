#pragma once

#include <cstdint>

struct YSFFichFields
{
    /*
     * Structured YSF FICH field model.
     *
     * These fields are intentionally explicit now so later work can add:
     *
     * - bit packing
     * - CRC
     * - convolutional encoding
     * - interleaving
     *
     * Current build still stores these in the existing 6-byte scaffold.
     */

    uint8_t frameInformation = 0;   // FI
    uint8_t communicationType = 0;  // CS / communication service
    uint8_t callMode = 0;           // CM
    uint8_t blockNumber = 0;        // BN
    uint8_t blockTotal = 0;         // BT
    uint8_t frameNumber = 0;        // FN
    uint8_t frameTotal = 0;         // FT
    uint8_t dataType = 0;           // DT
    uint8_t mr = 0;                 // MR
    uint8_t voip = 0;               // VOIP
    uint8_t dev = 0;                // DEV
    uint8_t sql = 0;                // SQL
    uint8_t sq = 0;                 // SQ
};
