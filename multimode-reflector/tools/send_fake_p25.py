#!/usr/bin/env python3

import socket

HOST = "127.0.0.1"
PORT = 41000

stream_id = 0x2500
sequence = 0
eot = 0

payload = bytes([
    0x21, 0x22, 0x23, 0x24, 0x25,
    0x26, 0x27, 0x28, 0x29
])

packet = bytearray()
packet += b"P25D"
packet += stream_id.to_bytes(2, "big")
packet += sequence.to_bytes(1, "big")
packet += eot.to_bytes(1, "big")
packet += len(payload).to_bytes(2, "big")
packet += payload

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.sendto(packet, (HOST, PORT))

print(f"Sent fake P25 packet to {HOST}:{PORT} len={len(packet)}")
