#pragma once

#include "codec_frame.h"

class CodecManager
{
public:

    static CodecFrame decode(
        const CodecFrame& input);

    static CodecFrame encode(
        const CodecFrame& input,
        CodecType targetCodec);
};
