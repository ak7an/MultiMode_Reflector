#!/usr/bin/env python3

import socket

HOST = "127.0.0.1"
PORT = 17000

stream_id = 0x1700
sequence = 0
eot = 0

payload = bytes([
    0x17, 0x01, 0x17, 0x02, 0x17,
    0x03, 0x17, 0x04, 0x17
])

packet = bytearray()
packet += b"M17D"
packet += stream_id.to_bytes(2, "big")
packet += sequence.to_bytes(1, "big")
packet += eot.to_bytes(1, "big")
packet += len(payload).to_bytes(2, "big")
packet += payload

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.sendto(packet, (HOST, PORT))

print(f"Sent fake M17 packet to {HOST}:{PORT} len={len(packet)}")
