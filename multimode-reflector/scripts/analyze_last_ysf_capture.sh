#!/bin/bash
set -e

LATEST=$(ls -t captures/ysf/ysf_capture_*.txt 2>/dev/null | head -1)

if [ -z "$LATEST" ]; then
    echo "No YSF captures found."
    exit 1
fi

echo "Using capture: $LATEST"

HEX=$(grep "Received" "$LATEST" | tail -1 | sed 's/^.*bytes: //')

if [ -z "$HEX" ]; then
    echo "No packet data found in capture."
    exit 1
fi

./ysf_analyze "$HEX"
