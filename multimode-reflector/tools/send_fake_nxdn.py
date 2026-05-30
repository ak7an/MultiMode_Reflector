#!/usr/bin/env python3

import socket

HOST = "127.0.0.1"
PORT = 41400

stream_id = 0x2770
sequence = 0
eot = 0

payload = bytes([
    0x11, 0x22, 0x33, 0x44, 0x55,
    0x66, 0x77, 0x88, 0x99
])

packet = bytearray()
packet += b"NXDN"
packet += stream_id.to_bytes(2, "big")
packet += sequence.to_bytes(1, "big")
packet += eot.to_bytes(1, "big")
packet += len(payload).to_bytes(2, "big")
packet += payload

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.sendto(packet, (HOST, PORT))

print(f"Sent fake NXDN packet to {HOST}:{PORT} len={len(packet)}")
