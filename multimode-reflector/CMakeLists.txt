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
    src/core/config_manager.cpp
    src/core/loop_guard.cpp 
    src/protocol/protocol_manager.cpp
    src/core/jitter_buffer.cpp
    src/core/transcoder.cpp
    src/protocol/ysf_encoder.cpp
    src/protocol/ysf_network_frame.cpp
    src/protocol/ysf_fich.cpp
    src/protocol/ysf_frame_mapper.cpp
    src/protocol/protocol_encoder.cpp
    src/core/debug_udp_sender.cpp
    src/core/media_pacer.cpp
    src/core/media_timing.cpp
    src/core/media_lifecycle.cpp
    src/core/media_output_queue.cpp
    src/core/media_output_worker.cpp
    src/core/active_stream.cpp
    src/core/status_reporter.cpp
    src/core/packet_capture.cpp
    src/protocol/ysf_protocol.cpp
    src/protocol/dstar_encoder.cpp
    src/util/config.cpp
    src/system/timer.cpp
    src/core/media_router.cpp
)

install(TARGETS reflector DESTINATION /usr/bin)
