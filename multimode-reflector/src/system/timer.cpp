#include "timer.h"

#include <thread>
#include <chrono>

void Timer::start(std::function<void()> tick) {

    std::thread([=]() {

        while (true) {
            tick();

            std::this_thread::sleep_for(
                std::chrono::milliseconds(20)
            );
        }

    }).detach();
}
