# MultiMode Reflector — Project State

## Repository

GitHub:
https://github.com/ak7an/MultiMode_Reflector

Active Branch:
next-feature

Last Known Good Commit:
a8e6e47

---

# Project Goal

Build a true multi-protocol digital voice reflector/transcoding core supporting:

- D-Star
- YSF / System Fusion
- DMR
- P25
- NXDN
- M17
- XLX-style routing

Core architecture is protocol-agnostic using MediaFrame abstraction.

---

# Current Architecture

## Input Layer

- Epoll UDP server
- ProtocolDetector identifies incoming protocol
- ProtocolManager dispatches to handlers

## Media Abstraction

MediaFrame contains:

- protocol
- streamId
- sequence
- frameType
- endOfTransmission
- createdAt
- payload

## Routing Layer

MediaRouter performs:

- FORWARD
- TRANSCODE
- RECORD
- DROP

Currently supported transcoding routes:

- DSTAR -> YSF
- YSF -> DSTAR

## Transcoder

Current transcoder is stub/pass-through only.

No vocoder conversion yet.

Current behavior:

- copies MediaFrame
- updates protocol
- refreshes createdAt timestamp

## Output Layer

MediaOutputQueue
→ MediaOutputWorker
→ MediaPacer
→ ProtocolEncoder
→ DebugUdpSender

Output port configurable:

debug_output_port=9001

---

# Current Synthetic Protocols

## Synthetic YSF

Magic:
YSFB

Purpose:
Synthetic bridge transport/testing only.

Recognized by:
ProtocolDetector

Encoded by:
YSFEncoder

Parsed by:
YSFProtocol

---

## Synthetic D-Star

Magic:
DSBB

Purpose:
Synthetic bridge transport/testing only.

Recognized by:
ProtocolDetector

Encoded by:
DStarEncoder

---

# Working Features

## Verified

- D-Star ingest
- YSF ingest
- MediaFrame reconstruction
- Bidirectional transcoding routes
- MediaOutputQueue operation
- Media pacing
- Packet replay tooling
- Packet capture tooling
- Synthetic protocol regeneration
- Configurable output port

---

# Known Limitations

## No real vocoder transcoding

Current transcoder only changes protocol labels.

## No real protocol framing

Current packets are synthetic transport wrappers:

- YSFB
- DSBB

## Loopback recursion possible

Synthetic packets can re-enter reflector if replay/output topology is incorrect.

## No stream ownership/origin suppression yet

Needed later.

---

# Important Files

## Core

src/core/media_router.cpp
src/core/transcoder.cpp
src/core/media_output_worker.cpp
src/core/media_output_queue.cpp
src/core/jitter_buffer.cpp

## Protocol

src/protocol/protocol_detector.cpp
src/protocol/protocol_encoder.cpp
src/protocol/dstar_protocol.cpp
src/protocol/ysf_protocol.cpp
src/protocol/ysf_encoder.cpp
src/protocol/dstar_encoder.cpp

## Network

src/net/epoll_server.cpp

---

# Test Tools

## Replay

packet_replay

Usage:

./packet_replay packet_capture.log 127.0.0.1 9000

## Listener

ysf_listener

## Packet Capture

packet_capture.log

---

# Expected Successful Logs

## DSTAR -> YSF

Transcoder stub: FROM=DSTAR TO=YSF

## YSF -> DSTAR

Transcoder stub: FROM=YSF TO=DSTAR

## Output

MediaOutputWorker encoded packet

---

# Current Next Step

Begin replacing synthetic YSF packets with minimally valid real YSF network framing.

Priority:

1. Real YSF framing
2. Real D-Star framing
3. Callsign/header transport
4. Session ownership
5. Loop suppression
6. Real vocoder integration

---

# Do Not Break

- MediaFrame abstraction
- MediaOutputQueue
- MediaPacer timing
- ProtocolEncoder abstraction
- Bidirectional transcoding routes
- Synthetic protocol test capability
