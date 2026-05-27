#pragma once

#include "../core/media_frame.h"

struct ProtocolCapabilities {

    bool ingest = false;

    bool encode = false;

    bool networkParse = false;

    bool networkBuild = false;

    bool transcodeSource = false;

    bool transcodeTarget = false;
};

ProtocolCapabilities getProtocolCapabilities(
    MediaProtocol protocol);
