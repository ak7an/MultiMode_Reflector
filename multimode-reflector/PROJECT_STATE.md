# MultiMode Reflector — Project State

## Repository

GitHub:
https://github.com/ak7an/MultiMode_Reflector

Active Branch:
next-feature

Latest Confirmed Commit:
9328793 — Add XLXD peer listener lifecycle

---

# Current Status

The reflector is now transitioning from an internal transcoding framework into a real reflector-network engine.

The system currently contains:

## Core Media Engine

- MediaFrame abstraction
- Protocol-independent routing core
- Dynamic transcoding pipeline
- MediaOutputQueue
- MediaOutputWorker
- MediaPacer
- JitterBuffer
- Packet capture tooling
- Replay tooling
- Graceful shutdown handling

## Protocol Support

Implemented protocol ingest:

- D-Star
- YSF
- DMR

Current encoder support:

- D-Star synthetic bridge encoder
- YSF network encoder

Stubbed transcoding targets:

- D-Star
- YSF
- DMR
- NXDN
- P25
- M17

## Regression Testing

Working automated regression:

- DSTAR → YSF

Test tooling:

- scripts/test_dstar_to_ysf.sh
- tools/send_fake_dmr.py

Verified behavior:

- D-Star replay ingest
- DMR fake packet ingest
- Dynamic protocol detection
- MediaFrame creation
- Transcoding dispatch
- Media output queueing
- Encoder output generation
- Packet capture output

---

# XLXD Peering Architecture

The reflector now includes the first real XLXD peering subsystem foundation.

## XLXD Configuration

Configurable:

- xlxd_peering_enabled
- xlxd_peer_reflector
- xlxd_peer_host
- xlxd_peer_port
- xlxd_peer_module
- xlxd_peer_poll_interval_ms
- xlxd_peer_timeout_ms

## Implemented XLXD Components

### Peer Registry

ProtocolPeerRegistry now tracks:

- protocol
- host
- port
- sockaddr_in
- lastPollSent
- lastPollReceived
- connected state

The registry now returns editable peer pointers rather than copies.

### Peer Registrar

Manual configured peer registration implemented.

Important design decision:

XLXD peers are manually configured trust relationships between reflector sysops.

There is intentionally:

- no hostfile loader
- no automatic peer discovery
- no auto-peering behavior

The earlier xlxd_peer_loader subsystem was removed.

### Global Accessors

Implemented:

- GlobalProtocolRouter
- GlobalPeerRegistry

These expose live global subsystems for peer networking and future receive-side session updates.

### Protocol Network Router

Implemented outbound UDP routing to configured peers.

Verified:

MediaOutputWorker
→ ProtocolNetworkRouter
→ configured peer
→ UDP transmission

### XLXD Poll Monitor

Implemented:

- dedicated monitor thread
- configurable poll interval
- poll timer loop
- poll packet generation

Current placeholder poll packet:

XLXP

Verified:

- periodic poll transmission
- outbound routing
- UDP delivery to configured peer listener

### Dedicated XLXD Peer Listener

Implemented separate listener subsystem:

XLXDPeerListener

Important architectural separation:

Protocol media ingress listeners:

- DSTAR
- YSF
- DMR

are now separate from:

- XLXD peer control traffic

The XLXD listener currently:

- binds configured peer port
- receives UDP packets
- detects placeholder XLXP poll packets
- logs receive activity

Verified:

- listener bind
- poll reception
- outbound/inbound peer traffic path

---

# Current Verified Runtime Flow

Verified runtime chain:

DMR ingest
→ MediaFrame creation
→ dynamic transcoding
→ MediaOutputQueue
→ encoder
→ ProtocolNetworkRouter
→ configured XLXD peer
→ UDP transmission
→ XLXD peer listener

---

# Important Architectural Decisions

## Single-Module Design

The reflector intentionally remains:

- single-module
- simple
- not a 26-module XLX clone

## DVSI Transcoding

Target design remains:

- two DVSI AMBE-3000 USB dongles
- single transcoding engine
- multimode protocol bridging

## XLXD Compatibility

Goal:

- peer with existing XLXD reflectors
- preserve XLXD ecosystem compatibility
- preserve callinghome / hosts update compatibility

without adopting full XLX multi-module architecture.

---

# Next Major Development Steps

## Immediate Next Step

Implement real peer session state handling:

- update lastPollReceived
- connected/disconnected state
- timeout detection
- stale peer handling

using the dedicated XLXD peer listener.

## Near-Term Goals

- real XLXD poll packet format
- peer session handshake
- peer timeout management
- peer reconnection handling
- inbound XLXD frame parsing
- outbound XLXD frame formatting

## Medium-Term Goals

- true D-Star reflector compatibility
- DPlus support
- DExtra support
- DCS support
- real XLXD peer interoperability testing

## Long-Term Goals

- DVSI AMBE transcoding integration
- full multimode reflector operation
- M17 support
- optional repeater attachment support
- configurable talkgroup/room routing
- selective protocol enable/disable

---

# Known Technical Notes

## Logging

Some multithreaded logs occasionally interleave due to concurrent thread output. This is cosmetic only and not currently considered a functional issue.

## Current XLXD Poll Packet

Current poll packet is placeholder only:

XLXP

This will later be replaced with actual XLXD-compatible protocol structures.

## Peer Listener

The dedicated XLXD peer listener is now the correct architectural location for:

- peer control traffic
- poll handling
- session state updates
- future handshake processing

and should remain separate from media protocol ingest listeners.

---

# Current Critical Source Files

## Core Media Pipeline

- src/core/media_output_worker.cpp
- src/core/media_output_queue.cpp
- src/core/media_router.cpp
- src/core/media_pacer.cpp
- src/core/jitter_buffer.cpp

## Protocol Routing

- src/net/protocol_network_router.cpp
- src/net/global_protocol_router.cpp
- src/net/protocol_peer_registry.cpp
- src/net/global_peer_registry.cpp

## XLXD Peering

- src/net/xlxd_peer_monitor.cpp
- src/net/xlxd_peer_listener.cpp
- src/net/xlxd_peer_registrar.cpp

## Protocol Ingest

- src/net/epoll_server.cpp
- src/net/protocol_listener_registry.cpp
- src/net/protocol_ports.cpp

## Protocol Encoding

- src/protocol/dstar/dstar_encoder.cpp
- src/protocol/ysf/ysf_encoder.cpp

## Configuration

- src/core/xlxd_peer_config.cpp
- reflector.ini

---
