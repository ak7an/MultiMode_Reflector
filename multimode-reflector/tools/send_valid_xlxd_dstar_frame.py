#!/usr/bin/env python3

import socket
import sys

host = "127.0.0.1"
port = 10001
reflector = "XLX999"
module = "A"

stream_id = 1234
sequence = 1
voice_payload = b"ABCDEFGHI"

if len(sys.argv) > 1:
    stream_id = int(sys.argv[1], 0)

if len(sys.argv) > 2:
    sequence = int(sys.argv[2], 0)

dsvt = bytearray(27 + len(voice_payload))
dsvt[0:4] = b"DSVT"
dsvt[12] = (stream_id >> 8) & 0xff
dsvt[13] = stream_id & 0xff
dsvt[14] = sequence & 0xff
dsvt[27:] = voice_payload

packet = b"XLXF"
packet += reflector.encode("ascii")[:6].ljust(6, b" ")
packet += module.encode("ascii")[0:1]
packet += len(dsvt).to_bytes(2, "big")
packet += bytes(dsvt)

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.sendto(packet, (host, port))

print(
    f"Sent valid XLXD D-Star frame: "
    f"stream_id={stream_id} sequence={sequence} "
    f"dsvt_len={len(dsvt)} xlxd_len={len(packet)}"
)
