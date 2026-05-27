#pragma once

#include <functional>
#include <atomic>

class Timer {
public:
    void start(std::function<void()> tick);
    void stop();

private:
    std::atomic<bool> running{false};
};
