#!/usr/bin/env python3

import socket
import sys

host = "127.0.0.1"
port = 10001
reflector = "XLX999"
module = "A"
payload = b"TESTFRAME"

if len(sys.argv) > 1:
    reflector = sys.argv[1]

if len(sys.argv) > 2:
    module = sys.argv[2]

if len(sys.argv) > 3:
    payload = sys.argv[3].encode("utf-8")

packet = b"XLXF"
packet += reflector.encode("ascii")[:6].ljust(6, b" ")
packet += module.encode("ascii")[0:1]
packet += len(payload).to_bytes(2, "big")
packet += payload

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.sendto(packet, (host, port))

print(
    f"Sent fake XLXD frame: reflector={reflector} "
    f"module={module} payload_len={len(payload)} to {host}:{port}"
)
