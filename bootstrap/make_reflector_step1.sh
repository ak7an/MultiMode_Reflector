#!/bin/bash

PROJECT="multimode-reflector"

echo "Creating $PROJECT..."

mkdir -p $PROJECT
cd $PROJECT

mkdir -p src/core src/net src/util src/system config systemd scripts include

#######################################
# CMake
#######################################
cat > CMakeLists.txt << 'EOF'
cmake_minimum_required(VERSION 3.10)
project(multimode_reflector)

set(CMAKE_CXX_STANDARD 17)

include_directories(src)

add_executable(reflector
    src/main.cpp
    src/core/logger.cpp
    src/core/session.cpp
    src/core/router.cpp
    src/net/epoll_server.cpp
    src/util/config.cpp
    src/system/timer.cpp
)

install(TARGETS reflector DESTINATION /usr/bin)
EOF

#######################################
# MAIN
#######################################
cat > src/main.cpp << 'EOF'
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
EOF

#######################################
# LOGGER
#######################################
cat > src/core/logger.h << 'EOF'
#pragma once
#include <string>

enum LogLevel { INFO, WARN, ERROR };

class Logger {
public:
    static void init(const std::string& file);
    static void log(LogLevel level, const std::string& msg);
};
EOF

cat > src/core/logger.cpp << 'EOF'
#include "logger.h"
#include <fstream>
#include <iostream>
#include <ctime>

static std::ofstream logFile;

void Logger::init(const std::string& file) {
    logFile.open(file, std::ios::app);
}

void Logger::log(LogLevel level, const std::string& msg) {

    const char* lvl =
        level == INFO ? "INFO" :
        level == WARN ? "WARN" : "ERROR";

    time_t now = time(nullptr);

    logFile << "[" << now << "][" << lvl << "] "
            << msg << std::endl;

    std::cout << "[" << lvl << "] "
              << msg << std::endl;
}
EOF

#######################################
# SESSION
#######################################
cat > src/core/session.h << 'EOF'
#pragma once
#include <cstdint>

struct Session {
    uint64_t id;
    bool active;
};
EOF

cat > src/core/session.cpp << 'EOF'
#include "session.h"
EOF

#######################################
# ROUTER
#######################################
cat > src/core/router.h << 'EOF'
#pragma once
#include "session.h"

class Router {
public:
    void route(Session& s);
};
EOF

cat > src/core/router.cpp << 'EOF'
#include "router.h"

void Router::route(Session& s) {}
EOF

#######################################
# EPOLL
#######################################
cat > src/net/epoll_server.h << 'EOF'
#pragma once

class EpollServer {
public:
    void init(int port);
    void tick();
};
EOF

cat > src/net/epoll_server.cpp << 'EOF'
#include "epoll_server.h"
#include "../core/logger.h"

void EpollServer::init(int port) {
    Logger::log(INFO, "Epoll initialized");
}

void EpollServer::tick() {}
EOF

#######################################
# CONFIG
#######################################
cat > src/util/config.h << 'EOF'
#pragma once

#include <string>

class Config {
public:
    void load(const std::string& file);
    int getInt(const std::string& key);
};
EOF

cat > src/util/config.cpp << 'EOF'
#include "config.h"

#include <fstream>
#include <sstream>
#include <unordered_map>

static std::unordered_map<std::string, int> ints;

void Config::load(const std::string& file) {

    std::ifstream in(file);
    std::string line;

    while (std::getline(in, line)) {

        if (line.empty() || line[0] == '[')
            continue;

        auto pos = line.find('=');

        if (pos == std::string::npos)
            continue;

        std::string key = line.substr(0, pos);
        int value = std::stoi(line.substr(pos + 1));

        ints[key] = value;
    }
}

int Config::getInt(const std::string& key) {

    if (ints.count(key))
        return ints[key];

    return 0;
}
EOF

#######################################
# TIMER
#######################################
cat > src/system/timer.h << 'EOF'
#pragma once

#include <functional>

class Timer {
public:
    void start(std::function<void()> tick);
};
EOF

cat > src/system/timer.cpp << 'EOF'
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
EOF

#######################################
# CONFIG FILE
#######################################
cat > config/reflector.ini << 'EOF'
[network]
port=9000
EOF

#######################################
# SYSTEMD
#######################################
cat > systemd/reflector.service << 'EOF'
[Unit]
Description=Multimode Reflector
After=network.target

[Service]
ExecStart=/usr/bin/reflector
Restart=always
RestartSec=2
Nice=-10

[Install]
WantedBy=multi-user.target
EOF

#######################################
# BUILD SCRIPT
#######################################
cat > scripts/build.sh << 'EOF'
#!/bin/bash

mkdir -p build
cd build

cmake ..
make -j$(nproc)
EOF

#######################################
# INSTALL SCRIPT
#######################################
cat > scripts/install.sh << 'EOF'
#!/bin/bash

cd build

sudo make install

sudo cp ../systemd/reflector.service \
    /etc/systemd/system/

sudo systemctl daemon-reload
sudo systemctl enable reflector
sudo systemctl start reflector

echo "Installed"
EOF

chmod +x scripts/build.sh
chmod +x scripts/install.sh

echo "DONE - multimode-reflector scaffold created"
