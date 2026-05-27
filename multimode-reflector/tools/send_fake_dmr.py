#!/usr/bin/env python3

import socket

packet = bytearray(55)

packet[0:4] = b'DMRD'

packet[4] = 0x12
packet[5] = 0x34

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

sock.sendto(packet, ("127.0.0.1", 9000))

print("Fake DMR packet sent")
