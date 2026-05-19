#include <iostream>
#include <unistd.h>

#include "core/logger.h"
#include "net/epoll_server.h"
#include "system/timer.h"
#include "util/config.h"

int main() {

    Config cfg;
    cfg.load("config/reflector.ini");

    Logger::init("reflector.log");
    Logger::log(INFO, "Reflector starting...");

    EpollServer server;
    server.init(cfg.getInt("port"));

    Timer timer;
    timer.start([&]() {
        server.tick();
    });

    while (true) {
        sleep(1);
    }

    return 0;
}
