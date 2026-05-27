# MultiMode Reflector — Project State

## Repository

GitHub:
https://github.com/ak7an/MultiMode_Reflector

Active Branch:
next-feature

Latest Confirmed Commit:
956b329 — Use D-Star network frame parser in protocol handler

Recent Stable Commits:
- f3b646f — Add DSTAR to YSF regression test
- 4eaa89a — Add D-Star network frame shell
- 956b329 — Use D-Star network frame parser in protocol handler

---

# Current Status

The reflector is now a stable modular protocol engine with:

- D-Star ingest
- YSF ingest
- MediaFrame abstraction
- Bidirectional DSTAR ↔ YSF transcoding routes
- MediaOutputQueue
- MediaOutputWorker
- MediaPacer
- JitterBuffer
- Clean shutdown handling
- Replay tooling
- Packet inspection tooling
- Automated DSTAR → YSFD regression testing

The current transcoder is still a stub/pass-through transcoder.
No AMBE/vocoder conversion yet.

---

# Major Architecture Achieved

## Media Abstraction

`MediaFrame` remains the central protocol-agnostic object.

Contains:

- protocol
- streamId
- sequence
- frameType
- endOfTransmission
- createdAt
- sourcePeer
- sourceCallsign
- payload

---

# YSF Protocol Stack

YSF is now structured into protocol layers:

- YSFEncoder
- YSFNetworkFrame
- YSFFich
- YSFFichFields
- YSFFrameType
- YSFFrameMapper

## YSF Build Path

MediaFrame
→ YSFNetworkFrame::build()
→ YSFFich::build()
→ YSFEncoder
→ ProtocolEncoder

## YSF Parse Path

YSFD packet
→ YSFNetworkFrame::parse()
→ YSFFich::parse()
→ MediaFrame
→ MediaRouter

## YSF Network Packet

Current test packet size:

155 bytes

Current layout:

0-3   : YSFD
4-5   : stream id
6     : sequence
7     : eot
8     : frame type
9-18  : source callsign padded to 10 bytes
19-28 : destination padded to 10 bytes
29-34 : semantic FICH placeholder
35+   : payload

## Current Semantic FICH Fields

Implemented in:

- src/protocol/ysf_fich_fields.h
- src/protocol/ysf_fich.h
- src/protocol/ysf_fich.cpp

Current fields:

- frameInformation
- communicationType
- dataType
- callMode

Verified voice-frame FICH bytes:

01 00 02 00 sequence eot

Verified semantic decode:

- FI = VOICE
- DT = VOICE_FULL_RATE
- CM = 0

---

# D-Star Protocol Stack

D-Star now includes:

- DStarNetworkFrame

Implemented files:

- src/protocol/dstar_network_frame.h
- src/protocol/dstar_network_frame.cpp

`DStarProtocol` now parses through:

DStarNetworkFrame::parse()

This replaced inline DSVT extraction logic.

D-Star encoder still needs to be refactored to use:

DStarNetworkFrame::build()

That is the current next step.

---

# Current Working Test Tools

## Replay Tool

packet_replay

Usage:

./packet_replay testdata/dstar_clean_19_replay.log 127.0.0.1 9000

## YSF Packet Dump Tool

Source:

tools/ysf_packet_dump.cpp

Local compiled helper binary:

./ysf_packet_dump

The binary itself should remain untracked.

## Automated Regression Test

Script:

scripts/test_dstar_to_ysf.sh

Run:

./scripts/test_dstar_to_ysf.sh

Expected:

[PASS] DSTAR -> YSFD regression test passed

Current regression verifies:

- 19 replay packets transmitted
- 19 YSFD packets generated
- YSF packet length
- source callsign padding
- destination padding
- semantic FICH decode
- DSTAR → YSF transport integrity

---

# Verified Successful Output

Most recent successful regression:

[PASS] DSTAR -> YSFD regression test passed

Verified YSFD dump:

YSFD packet #1
  len       : 155
  streamId  : 4660
  sequence  : 0
  eot       : 0
  frameType : 2
  src       : [TEST      ]
  dst       : [ALL       ]
  src hex   : 54 45 53 54 20 20 20 20 20 20
  dst hex   : 41 4c 4c 20 20 20 20 20 20 20
  fich hex  : 01 00 02 00 00 00
  fich FI   : VOICE
  fich DT   : VOICE_FULL_RATE
  fich CM   : 0

---

# Config State

Runtime config file:

reflector.ini

Important setting:

ysf_frame_mode=network

Supported values:

ysf_frame_mode=synthetic
ysf_frame_mode=network

reflector.ini is runtime-local and may remain uncommitted.

---

# Known Limitations

## No real vocoder conversion yet

Current transcoder changes framing only.

No AMBE conversion.

## YSF FICH still semantic placeholder

Structured and named, but not real Yaesu bit-level encoding yet.

## D-Star build path incomplete

Parse path uses:

DStarNetworkFrame::parse()

Encode path still needs:

DStarNetworkFrame::build()

## Real D-Star header transport incomplete

Replay currently uses synthetic identity:

TEST

## Loop suppression/origin ownership incomplete

Future work:

- stream ownership
- origin suppression
- loop prevention
- replay topology protection

---

# Do Not Break

- MediaFrame abstraction
- ProtocolEncoder abstraction
- MediaOutputQueue
- MediaOutputWorker
- MediaPacer timing
- JitterBuffer
- Synthetic YSF mode
- Synthetic D-Star mode
- DSTAR → YSF regression test
- YSF packet dump tool
- Runtime-selectable ysf_frame_mode

---

# Current Next Step

Refactor DStarEncoder to use:

DStarNetworkFrame::build()

Goal:

MediaFrame
→ DStarNetworkFrame::build()
→ DStarEncoder
→ ProtocolEncoder

This completes D-Star structural symmetry with YSF.

After that:

1. Add D-Star packet dump tool
2. Add YSF → DSTAR regression test
3. Add semantic D-Star frame mapper
4. Add real D-Star header transport
5. Add stream ownership/origin suppression
6. Add real YSF FICH bit encoding
7. Add vocoder boundary layer
8. Begin AMBE/vocoder integration

---

# Fresh Chat Startup Prompt

Read PROJECT_STATE.md and continue from Current Next Step. Preserve regression tests and synthetic modes.
