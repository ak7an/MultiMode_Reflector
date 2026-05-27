#include "timer.h"

#include <thread>
#include <chrono>

void Timer::start(std::function<void()> tick) {

    running = true;

    std::thread([this, tick]() {

        while (running) {
            tick();

            std::this_thread::sleep_for(
                std::chrono::milliseconds(20));
        }

    }).detach();
}

void Timer::stop() {
    running = false;
}
