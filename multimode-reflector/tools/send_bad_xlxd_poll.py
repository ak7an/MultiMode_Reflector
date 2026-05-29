#!/usr/bin/env python3

import socket
import sys

host = "127.0.0.1"
port = 10001
reflector = "XLX123"
module = "B"

if len(sys.argv) > 1:
    reflector = sys.argv[1]

if len(sys.argv) > 2:
    module = sys.argv[2]

packet = b"XLXP"
packet += reflector.encode("ascii")[:6].ljust(6, b" ")
packet += module.encode("ascii")[0:1]

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.sendto(packet, (host, port))

print(f"Sent bad XLXD poll: reflector={reflector} module={module} to {host}:{port}")
