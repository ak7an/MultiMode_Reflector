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
