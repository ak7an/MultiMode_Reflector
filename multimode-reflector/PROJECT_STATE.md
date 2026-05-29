# MultiMode Reflector — Project State

## Repository

GitHub:
https://github.com/ak7an/MultiMode_Reflector

Active Branch:
next-feature

Latest Confirmed Commit:
9328793 — Add XLXD peer listener lifecycle

---
Latest Confirmed Commit:
3c377c0 — Document development workflow and protocol configuration


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

---

# Development Workflow

## Current Development Method

Early project development used very small incremental changes:

- implement one class
- build
- fix compile errors
- test
- commit

This was necessary while establishing the core architecture and resolving build-system issues.

## Current Project Phase

The architecture is now sufficiently stable to use larger feature batches.

Preferred workflow:

1. Define a complete subsystem feature.
2. Implement the entire subsystem.
3. Build once.
4. Resolve compile errors.
5. Perform a focused runtime test.
6. Commit the complete feature.

Example feature batches:

- Protocol enable/disable expansion
- XLXD peer state management
- XLXD timeout handling
- DPlus support
- DExtra support
- DCS support

Avoid returning to micro-commits unless architectural uncertainty or debugging requires it.

## Architectural Priority Order

Before adding new protocol families:

1. Stabilize D-Star
2. Stabilize YSF
3. Stabilize DMR
4. Complete XLXD peering
5. Implement reflector interoperability
6. Integrate DVSI transcoding

Only after the above are stable should development expand into:

- NXDN
- P25
- M17

## Design Philosophy

The project prioritizes:

- Stability over feature count
- Low latency over architectural complexity
- Clean layering over shortcuts
- Deterministic behavior over maximum flexibility

The goal is not to become the largest multimode reflector.

The goal is to become a reliable, maintainable, low-latency multimode reflector that remains operationally simple.

---

# Recent Progress

Latest Confirmed Commit:

cc2dd60 — Document critical source files in project state

Latest Development Work:

- Added future protocol enable/disable framework
- Added NXDN enable flag
- Added P25 enable flag
- Added M17 enable flag
- Added runtime protocol enable status logging
- Confirmed protocol listener creation remains driven by protocol enable state

Current Runtime Verification:

Protocol enabled:

DSTAR=1
YSF=1
DMR=1
NXDN=0
P25=0
M17=0

This establishes the future ability to run:

- DMR-only reflectors
- P25-only reflectors
- DMR/P25 reflectors
- M17-only reflectors
- Any supported protocol combination

without code changes.


-------------------------------------------------------------------------------
2026-05-28 PROGRESS UPDATE
-------------------------------------------------------------------------------

Major Milestone:
XLXD peer traffic is now fully integrated into the reflector media pipeline.

Completed:

XLXD Session Layer
------------------
- Implemented XLXD poll packet build/parse support
- Implemented XLXD handshake packet build/parse support
- Added peer identity validation
- Added reflector/module validation
- Added protocol version tracking
- Added session establishment tracking
- Added session-required frame acceptance

Verified:
- Correct reflector accepted
- Incorrect reflector rejected
- Incorrect module rejected
- Frames rejected without established session
- Frames accepted after successful handshake

XLXD Frame Transport
--------------------
- Implemented XLXD frame packet build/parse support
- Added reflector identification
- Added module identification
- Added payload extraction

Verified:
- Valid frame acceptance
- Invalid session rejection
- Invalid reflector rejection
- Invalid module rejection

D-Star Network Integration
--------------------------
- XLXD payloads now validated using DStarNetworkFrame parser
- Extracted stream ID
- Extracted sequence number
- Extracted voice payload

MediaFrame now preserves:
- Stream ID
- Sequence Number
- Source Reflector
- Source Module
- Source Peer
- Payload

Media Pipeline Integration
--------------------------
XLXD traffic now follows the same path as native protocol traffic:

XLXD Frame
  -> DStarNetworkFrame
  -> MediaFrame
  -> MediaRouter
  -> TranscodingPolicy
  -> Transcoder
  -> MediaOutputQueue
  -> MediaOutputWorker

This replaces the earlier direct queue insertion model.

Routing Metadata
----------------
Added MediaFrame routing context:

- sourceReflector
- sourceModule

Verified propagation through:
- XLXD Peer Listener
- MediaOutputQueue
- MediaOutputWorker
- MediaRouter

This metadata will support future routing policy decisions.

Transcoding Policy Improvements
-------------------------------
Added protocol enable-state filtering.

Before:
D-Star traffic generated outputs for:
- YSF
- DMR
- NXDN
- P25
- M17

After:
Only enabled protocols are generated.

Verified Configuration:
Enabled:
- DSTAR
- YSF
- DMR

Disabled:
- NXDN
- P25
- M17

Observed Output:
D-Star -> YSF
D-Star -> DMR

No disabled protocol outputs generated.

Loop Prevention
---------------
Implemented XLXD outbound route suppression.

Purpose:
Prevent internally generated traffic from being re-ingested
as new XLXD peer traffic and creating routing loops.

Verified by runtime testing.

Shutdown Stability
------------------
Fixed:
- XLXD listener shutdown
- XLXD monitor shutdown
- Thread cleanup sequencing

Reflector now exits cleanly without aborts.

Verified End-to-End Flow
------------------------

XLXD Poll
  -> Handshake
  -> Session Establishment
  -> D-Star Network Frame Validation
  -> MediaRouter
  -> TranscodingPolicy
  -> YSF Output
  -> DMR Output
  -> MediaOutputWorker

Current Project Status
----------------------

Working:
- XLXD Polling
- XLXD Handshake
- XLXD Session Management
- XLXD Frame Transport
- D-Star Frame Validation
- Media Routing
- Protocol-Aware Transcoding
- YSF Output Generation
- DMR Output Generation
- Clean Shutdown

Next Major Development Area
---------------------------

Routing Policy Engine

Goal:

Use sourceReflector and sourceModule metadata to perform
configurable routing between:

- D-Star Reflector Modules
- YSF Rooms
- DMR Talkgroups
- NXDN Rooms
- P25 Talkgroups
- M17 Rooms

This becomes the foundation for true multimode reflector operation.

Estimated Overall Completion:
~50%


-------------------------------------------------------------------------------
2026-05-29 PROGRESS UPDATE
-------------------------------------------------------------------------------

Major Milestone:
The reflector now has a complete codec and AMBE hardware integration architecture.

This session moved the project beyond protocol routing and into real transcoder readiness.

Single-Module Routing Correction
--------------------------------
Corrected the routing design back to the intended project architecture:

- Single-module reflector
- No 26-module XLX-style routing model
- One active reflector/module context
- Multiple protocol targets from that single module

Current model:

Single XLXD Module
  -> RouteConfig
  -> YSF
  -> DMR
  -> Future NXDN / P25 / M17

This preserves the intentionally simple reflector design.

Routing Policy Engine
---------------------
Implemented a configurable single-module routing policy.

Current routing configuration supports:

- route_source_reflector
- route_source_module
- route_enable_ysf
- route_enable_dmr
- route_enable_nxdn
- route_enable_p25
- route_enable_m17

Verified behavior:

XLX999/A
  -> YSF
  -> DMR

Disabled protocols are not emitted.

Codec Architecture
------------------
Added a codec abstraction layer.

New components:

- CodecType
- CodecFrame
- CodecRouter
- CodecManager

Current transcoding flow:

MediaFrame
  -> Transcoder
  -> CodecRouter
  -> CodecManager
  -> CodecRouter
  -> MediaFrame

This establishes the insertion point for real codec translation.

AMBE Hardware Integration Path
------------------------------
Added an AMBE device integration layer.

New components:

- AMBEDeviceManager
- AMBEProtocol
- SerialPort
- DeviceDiscovery
- AMBEDeviceStatus

Current AMBE path:

CodecManager
  -> AMBEDeviceManager
  -> AMBEProtocol
  -> SerialPort
  -> Future DVSI AMBE-3000 USB dongles

Serial Transport
----------------
Added reusable serial transport support.

SerialPort currently supports:

- openPort()
- closePort()
- isOpen()
- writeBytes()
- readBytes()

Implementation uses Linux termios and supports common baud rates including:

- 9600
- 19200
- 38400
- 57600
- 115200
- 230400
- 460800

AMBE Configuration
------------------
Added AMBE device configuration to reflector.ini:

- ambe_decode_device=/dev/ttyUSB0
- ambe_encode_device=/dev/ttyUSB1
- ambe_baud=460800

Without dongles attached, the reflector behaves correctly:

- Attempts device discovery
- Attempts serial open
- Marks AMBE ready state as false
- Continues running normally

Verified no-dongle startup:

DeviceDiscovery FTDI count: 0
SerialPort open failed
AMBE STATUS READY=0
Reflector continues running

USB / FTDI Discovery
--------------------
Added USB serial device discovery.

DeviceDiscovery currently scans:

/dev/serial/by-id

and identifies FTDI-style serial devices.

This prepares the project for stable dongle assignment instead of relying only on /dev/ttyUSB0 and /dev/ttyUSB1.

AMBE Health Status
------------------
Added AMBE health/status reporting.

Tracked status fields:

- decodePresent
- encodePresent
- decodeOpen
- encodeOpen
- decodeResponsive
- encodeResponsive
- ready

Verified status output:

AMBE STATUS:
DECODE_PRESENT=0
ENCODE_PRESENT=0
DECODE_OPEN=0
ENCODE_OPEN=0
DECODE_RESPONSIVE=0
ENCODE_RESPONSIVE=0
READY=0

End-to-End Codec Path Verified
------------------------------
The live runtime path now includes:

XLXD Frame
  -> D-Star Network Parser
  -> MediaFrame
  -> MediaRouter
  -> RouteConfig
  -> Transcoder
  -> CodecRouter
  -> CodecManager
  -> AMBEDeviceManager
  -> AMBEProtocol
  -> CodecManager
  -> CodecRouter
  -> MediaOutputQueue
  -> MediaOutputWorker
  -> YSF Encoder
  -> DMR Encoder
  -> ProtocolNetworkRouter

Verified output includes:

- CodecRouter decode stub
- CodecManager decode
- AMBEDeviceManager decode
- AMBEProtocol decode stub
- CodecManager encode
- AMBEDeviceManager encode
- AMBEProtocol encode stub
- CodecRouter encode stub
- YSF packet generation
- DMR packet generation

Current Working Features
------------------------

Working:
- Single-module reflector routing model
- XLXD polling
- XLXD handshake
- XLXD session management
- XLXD frame parsing
- D-Star network frame parsing
- MediaRouter integration
- RouteConfig policy
- YSF output generation
- DMR output generation
- CodecRouter
- CodecManager
- AMBEDeviceManager
- AMBEProtocol skeleton
- SerialPort skeleton
- USB/FTDI device discovery
- AMBE health reporting
- Safe no-hardware startup
- Clean shutdown

Important Recent Commits
------------------------

- 3e9f620 Restore single-module routing policy model
- 36edc5e Add codec abstraction layer skeleton
- b808eb8 Route transcoding through codec abstraction layer
- 27dff7d Add codec manager layer behind codec router
- 6b4e199 Add AMBE device manager integration path
- 99739e3 Add serial port transport skeleton
- 4f371eb Initialize AMBE device manager from configuration
- c74f854 Add USB serial device discovery for AMBE hardware
- be4a46e Add AMBE device health status reporting

Next Major Development Area
---------------------------

DVSI AMBE-3000 Protocol Implementation

Next steps:

1. Add real AMBEProtocol packet framing.
2. Implement probe/response validation.
3. Add FTDI stable device assignment by serial number.
4. Detect decode and encode dongle roles.
5. Implement AMBE frame decode path.
6. Implement AMBE frame encode path.
7. Replace stub codec passthrough with real DVSI transcoding.
8. Validate real D-Star -> YSF and D-Star -> DMR voice output.

Current Overall Status
----------------------

Project Phase:
Hardware Integration Ready

Estimated Overall Completion:
~60%


-------------------------------------------------------------------------------
2026-05-29 AMBE HARDWARE VERIFICATION MILESTONE
-------------------------------------------------------------------------------

Major Milestone:
Verified live communication with real ThumbDV AMBE hardware.

Latest Confirmed Commit:
e1471d6 — Log ThumbDV AMBE firmware version

Hardware Connected
------------------

Two NW Digital Radio ThumbDV devices were attached and detected.

Detected USB devices:

- ThumbDV serial D30G37BA
- ThumbDV serial D30G37AJ

Stable Linux device paths:

- /dev/serial/by-id/usb-NW_Digital_Radio_09_25_ThumbDV_D30G37BA-if00-port0
- /dev/serial/by-id/usb-NW_Digital_Radio_09_25_ThumbDV_D30G37AJ-if00-port0

Driver Status
-------------

Confirmed Linux kernel FTDI drivers are active:

- ftdi_sio
- usbserial

Confirmed user is in dialout group.

No external FTDI D2XX driver is required.

AMBE Device Configuration
-------------------------

reflector.ini now uses stable by-id device paths:

- ambe_decode_device=/dev/serial/by-id/usb-NW_Digital_Radio_09_25_ThumbDV_D30G37BA-if00-port0
- ambe_encode_device=/dev/serial/by-id/usb-NW_Digital_Radio_09_25_ThumbDV_D30G37AJ-if00-port0
- ambe_baud=460800

Verified both serial ports open successfully at 460800 baud.

Real AMBE Probe
---------------

Implemented and verified a real ThumbDV / AMBE-3000 version probe.

Probe TX:

61 00 03 00 31 2F 1D

Probe RX:

61 00 31 00 31 56 31 32 30 2E 45 31 30 30 2E 58 58 58 58 2E 43 31 30 36 2E 47 35 31 34 2E 52 30 30 39 2E 42 30 30 31 30 34 31 31 2E 43 30 30 32 30 32 30 38 00

Parsed AMBE firmware version:

1V120.E100.XXXX.C106.G514.R009.B0010411.C0020208

Verified AMBE Status
--------------------

Both dongles now report responsive:

- DECODE_PRESENT=1
- ENCODE_PRESENT=1
- DECODE_OPEN=1
- ENCODE_OPEN=1
- DECODE_RESPONSIVE=1
- ENCODE_RESPONSIVE=1
- READY=1

Significance
------------

The reflector is now communicating with real AMBE hardware.

This confirms:

- FTDI transport works
- SerialPort works
- DeviceDiscovery works
- Stable device paths work
- AMBEProtocol packet framing works
- AMBEProtocol response parsing works
- AMBEDeviceManager health tracking works
- Dual-dongle architecture is viable

Current AMBE Architecture
-------------------------

CodecManager
  -> AMBEDeviceManager
  -> AMBEProtocol
  -> SerialPort
  -> ThumbDV AMBE Hardware

Next Major Development Area
---------------------------

Real AMBE codec operations.

Immediate next targets:

1. Implement AMBE decode command.
2. Convert source AMBE frames into PCMFrame.
3. Route PCMFrame through AudioLevelManager.
4. Implement AMBE encode command.
5. Convert PCMFrame back to target AMBE frames.
6. Validate D-Star -> YSF real audio path.
7. Validate D-Star -> DMR real audio path.
8. Continue future protocol output work for NXDN, P25, and M17.

Current Overall Status
----------------------

Project Phase:
Live AMBE Hardware Communication Verified

Estimated Overall Completion:
~75%

