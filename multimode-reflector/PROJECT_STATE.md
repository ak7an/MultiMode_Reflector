# MultiMode Reflector — Project State

## Repository

GitHub:
https://github.com/ak7an/MultiMode_Reflector

Active Branch:
next-feature

Latest Confirmed Commit:
35aa5cd — Add DMR encoder dispatch

---

# Project Vision

Build a:

- single-module
- multimode
- multitranscode
- peer-capable reflector

Supporting:

- D-Star
- YSF
- DMR
- NXDN
- P25
- M17

Using:

- DVSI AMBE-3000 USB dongles
- dynamic transcoding architecture
- XLXD-compatible peering
- DPlus / DExtra / DCS support
- callinghome / hosts file compatibility

The reflector intentionally avoids:

- 26-module XLX complexity
- oversized routing architectures
- unnecessary operational overhead

Goal:

Simple operational model with powerful internal transcoding capability.

---

# Core Design Philosophy

The reflector architecture is based on:

Protocol
→ MediaFrame abstraction
→ Dynamic routing policy
→ Transcoding matrix
→ Encoder dispatch
→ Network output

Protocols should integrate horizontally without requiring router rewrites.

The architecture prioritizes:

- scalability
- protocol symmetry
- deterministic timing
- queue stability
- future AMBE resource management
- clean runtime behavior

---

# Current Architecture

Implemented core systems:

- MediaFrame abstraction
- ProtocolManager
- ProtocolDetector
- MediaRouter
- TranscodingPolicy
- TranscodingTargets
- TranscodingMatrix
- MediaOutputQueue
- MediaOutputWorker
- MediaPacer
- JitterBuffer
- ActiveStream tracking
- PacketCapture
- LoopGuard
- PeerManager
- Dynamic encoder gating

---

# Protocol Status

## D-Star

Implemented:

- protocol detection
- network frame parsing
- synthetic bridge encoder
- network frame builder
- MediaFrame ingest
- DSTAR → YSF transcoding path
- DSTAR encoder dispatch

Operational status:
WORKING

---

## YSF

Implemented:

- protocol detection
- YSFD network parsing
- synthetic bridge parsing
- FICH parsing/building
- network frame builder
- encoder dispatch
- MediaFrame ingest
- YSF → DSTAR transcoding path

Operational status:
WORKING

---

## DMR

Implemented:

- protocol detection
- protocol skeleton
- MediaFrame parsing
- network builder skeleton
- encoder dispatch
- dynamic routing integration

Operational status:
FRAMEWORK ONLY

Current limitations:

- no real DMR voice handling
- no slot handling
- no LC parsing
- no AMBE handling
- no real network interoperability yet

---

## NXDN

Status:
PLANNED

---

## P25

Status:
PLANNED

---

## M17

Status:
PLANNED

---

# Current Working Features

Validated working:

- DSTAR ingest
- YSF ingest
- DMR ingest skeleton
- DSTAR ↔ YSF transcoding pipeline
- DMR dynamic transcoding pipeline
- Dynamic transcoding target selection
- Encoder capability gating
- Protocol scalability architecture
- Queueing/pacing framework
- Regression testing framework

---

# Current Limitations

Not yet implemented:

- real AMBE transcoding
- DVSI USB management
- DMR slot handling
- DMR LC handling
- real DMR interoperability
- NXDN implementation
- P25 implementation
- M17 implementation
- XLXD peering
- DPlus support
- DExtra support
- DCS support
- callinghome support
- production security hardening
- stream arbitration
- advanced loop prevention
- RF edge-case handling

---

# Known Good Tests

Validated repeatedly:

## DSTAR → YSFD regression

Script:
./scripts/test_dstar_to_ysf.sh

Result:
PASS

---

## DMR ingress skeleton

Tool:
./tools/send_fake_dmr.py

Validated:

DMR detect
→ DMR protocol dispatch
→ MediaFrame creation
→ dynamic routing
→ transcoding policy
→ DSTAR output
→ YSF output

Result:
PASS

---

# Important Design Decisions

## Single Module Only

The reflector intentionally remains:

- one module
- one routing domain
- one transcoding core

No 26-module XLX operational model.

---

## Dynamic Routing

Routing must remain:

- protocol-agnostic
- policy-driven
- matrix-driven

Avoid protocol-pair hardcoding.

---

## Future AMBE Management

Current transcoder is a stub.

Future design must support:

- multiple DVSI dongles
- resource allocation
- transcoder scheduling
- stream ownership
- transcoder exhaustion handling

---

# Next Priorities

1. Real DMR frame handling
2. AMBE transcoder architecture
3. DVSI dongle integration
4. XLXD peer networking
5. DPlus support
6. DExtra support
7. DCS support
8. CallingHome compatibility
9. NXDN ingest
10. P25 ingest
11. M17 ingest
12. Real RF testing

---

# Current Development Status

Architecture maturity:
~35%

Operational maturity:
~20%

Real-world RF readiness:
~10%

Major architectural risk:
Largely resolved

Primary remaining work:
Implementation depth and interoperability


---

# Expanded System Design Direction

## Operational Philosophy

The reflector is intended to become a usable standalone multimode system BEFORE optional add-ons are introduced.

Core functionality should provide:

- multimode protocol interoperability
- transcoding
- XLXD-compatible peering
- DPlus / DExtra / DCS support
- callinghome compatibility
- DMR master-style operation
- single-room operational simplicity

without requiring external network dependency systems such as BrandMeister.

---

# DMR Design Direction

DMR support is intentionally designed around:

- XLX DMR Master style behavior
- local reflector-owned routing
- local TG/room mapping
- no external DMR network dependency
- no BrandMeister integration
- no external TG routing systems

DMR exists as a protocol interface to the reflector core.

Architecture target:

DMR repeater/hotspot/client
→ reflector DMR master
→ MediaFrame core
→ multimode transcoding engine

---

# Configurability Goals

Future operator configurability should include:

- protocol enable/disable
- configurable ports per protocol
- non-standard port support
- TG / room assignment
- protocol room mapping
- peer configuration
- transcoding policy configuration

Example future configuration concept:

[Protocols]
DSTAR=1
YSF=1
DMR=1
NXDN=0
P25=0
M17=0

[Ports]
DSTAR=9000
YSF=42000
DMR=62031

[Rooms]
DefaultRoom=4000
DMRTalkgroup=4000
YSFRoom=4000

---

# Core vs Add-On Philosophy

## Core Reflector Features

The following are considered part of the core reflector system:

- MediaFrame engine
- transcoding engine
- protocol stacks
- XLXD peering
- DPlus
- DExtra
- DCS
- callinghome
- DMR master behavior
- dynamic routing policy
- protocol configurability

A core-only build should already be a usable operational reflector.

---

## Optional Add-Ons

The following are considered optional deployment add-ons:

- MMDVM modem attachment
- repeater mode
- hotspot mode
- client mode
- web dashboard
- advanced monitoring
- distributed transcoder pools
- advanced visualization systems

These should remain modular and should not complicate the core reflector architecture.

---

# Future Protocol Edge Model

Long-term architecture direction:

[ Protocol Edge ]
        ↓
[ MediaFrame Core ]
        ↓
[ Routing / Policy ]
        ↓
[ Transcoding ]
        ↓
[ Output Protocol Edge ]

Protocols should behave as interchangeable edge adapters around a unified multimode transcoding core.

