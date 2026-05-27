#pragma once

#include "media_frame.h"

class TranscodingMatrix
{
public:
    static bool canTranscode(
        MediaProtocol source,
        MediaProtocol target);
};
