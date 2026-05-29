#!/usr/bin/env python3

import serial
import sys
import time

if len(sys.argv) != 2:
    print("Usage: probe_thumbdv.py <serial-device>")
    sys.exit(1)

device = sys.argv[1]

tests = [
    ("verstring_candidate", bytes.fromhex("61 00 03 00 31 2F 1D")),
    ("soft_reset_candidate", bytes.fromhex("61 00 07 00 34 05 00 00 0F 00 00")),
    ("old_probe_candidate", bytes.fromhex("61 00 01 01")),
]

print(f"Opening {device}")

with serial.Serial(
    device,
    baudrate=460800,
    bytesize=serial.EIGHTBITS,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    timeout=1.0,
    xonxoff=False,
    rtscts=False,
    dsrdtr=False,
) as ser:
    ser.reset_input_buffer()
    ser.reset_output_buffer()

    for name, packet in tests:
        print()
        print(f"TEST {name}")
        print("TX:", packet.hex(" ").upper())

        ser.write(packet)
        ser.flush()

        time.sleep(0.25)

        rx = ser.read(512)

        if rx:
            print("RX:", rx.hex(" ").upper())
        else:
            print("RX: <none>")
