#!/bin/bash
set -e

OUT="captures/ysf/ysf_capture_$(date +%Y%m%d_%H%M%S).txt"

echo "Capturing YSF debug packets to $OUT"
echo "Press Ctrl+C to stop."

./ysf_listener | tee "$OUT"
