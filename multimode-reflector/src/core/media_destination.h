#pragma once

#include <string>

enum class MediaDestinationType {

    ALL,
    PEER,
    PROTOCOL,
    MODULE,
    BRIDGE
};

struct MediaDestination {

    MediaDestinationType type;

    std::string value;
};
