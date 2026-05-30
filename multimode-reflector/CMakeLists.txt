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
    src/net/protocol_peer_registry.cpp
    src/net/protocol_network_router.cpp
    src/net/global_protocol_router.cpp
    src/net/global_peer_registry.cpp
    src/net/xlxd_peer_registrar.cpp
    src/net/xlxd_peer_monitor.cpp
    src/net/xlxd_peer_listener.cpp
    src/net/xlxd_poll_packet.cpp
    src/net/xlxd_handshake_packet.cpp
    src/net/xlxd_frame_packet.cpp
    src/net/protocol_listener_registry.cpp
    src/net/epoll_server.cpp
    src/net/protocol_peer_registry.cpp
    src/net/protocol_network_router.cpp
    src/net/global_protocol_router.cpp
    src/net/global_peer_registry.cpp
    src/net/xlxd_peer_registrar.cpp
    src/net/xlxd_peer_monitor.cpp
    src/net/xlxd_peer_listener.cpp
    src/net/xlxd_poll_packet.cpp
    src/protocol/protocol_detector.cpp
    src/protocol/protocol_name.cpp
    src/protocol/dstar_protocol.cpp
    src/protocol/dstar_header.cpp
    src/protocol/dstar_session.cpp
    src/core/lastheard_manager.cpp 
    src/core/config_manager.cpp
    src/core/loop_guard.cpp 
    src/protocol/protocol_manager.cpp
    src/core/jitter_buffer.cpp
    src/core/transcoder.cpp
    src/core/codec_router.cpp
    src/core/codec_manager.cpp
    src/core/ambe_device_manager.cpp
    src/core/ambe_device_status.cpp
    src/core/ambe_protocol.cpp
    src/core/serial_port.cpp
    src/core/device_discovery.cpp
    src/core/hex_dump.cpp
    src/core/audio_level_config.cpp
    src/core/audio_level_manager.cpp
    src/core/protocol_codec_mapper.cpp
    src/protocol/ysf_encoder.cpp
    src/protocol/ysf_scaffold_frame.cpp
    src/protocol/ysf_voice_frame.cpp
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
    src/protocol/dstar_network_frame.cpp
    src/protocol/protocol_capabilities.cpp
    src/core/transcoding_matrix.cpp
    src/core/transcoding_targets.cpp
    src/core/transcoding_policy.cpp
    src/protocol/dmr_protocol.cpp
    src/protocol/dmr_network_frame.cpp
    src/protocol/nxdn_protocol.cpp
    src/protocol/nxdn_scaffold_frame.cpp
    src/util/config.cpp
    src/system/timer.cpp
    src/core/media_router.cpp
    src/core/protocol_config.cpp
    src/core/route_config.cpp
    src/core/protocol_ports.cpp
    src/core/xlxd_peer_config.cpp
    src/protocol/protocol_encoder_capabilities.cpp
    src/protocol/dmr_network_builder.cpp
    src/protocol/dmr_voice_frame.cpp



)

install(TARGETS reflector DESTINATION /usr/bin)
