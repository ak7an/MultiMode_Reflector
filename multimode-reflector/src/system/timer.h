#pragma once

#include <functional>

class Timer {
public:
    void start(std::function<void()> tick);
};
