cmake_minimum_required(VERSION 3.10)
project(multimode_reflector)

set(CMAKE_CXX_STANDARD 17)

include_directories(src)

add_executable(reflector
    src/main.cpp
    src/core/logger.cpp
    src/core/session.cpp
    src/core/router.cpp
    src/core/peer_manager.cpp
    src/net/epoll_server.cpp
    src/net/epoll_server.cpp 
    src/protocol/protocol_detector.cpp
    src/protocol/dstar_protocol.cpp
    src/protocol/dstar_header.cpp
    src/protocol/dstar_session.cpp
    src/core/lastheard_manager.cpp 
    src/util/config.cpp
    src/system/timer.cpp
)

install(TARGETS reflector DESTINATION /usr/bin)
