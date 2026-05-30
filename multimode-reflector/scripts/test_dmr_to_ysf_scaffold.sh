#!/usr/bin/env bash
set -euo pipefail

LOG_FILE="/tmp/multimode-reflector-dmr-to-ysf.log"

rm -f "$LOG_FILE"

echo "Starting reflector..."
build/reflector > "$LOG_FILE" 2>&1 &
REFLECTOR_PID=$!

cleanup() {
    if kill -0 "$REFLECTOR_PID" 2>/dev/null; then
        kill -INT "$REFLECTOR_PID" 2>/dev/null || true
        wait "$REFLECTOR_PID" 2>/dev/null || true
    fi
}
trap cleanup EXIT

sleep 2

echo "Sending fake DMR packet..."
python3 tools/send_fake_dmr.py

sleep 2

echo "Stopping reflector..."
cleanup
trap - EXIT

echo "Checking log..."

grep -q "DMR MediaFrame created" "$LOG_FILE"
grep -q "Transcoder codec path: FROM=DMR TO=YSF" "$LOG_FILE"
grep -q "YSFFich build:" "$LOG_FILE"
grep -q "YSFVoiceFrame build:" "$LOG_FILE"
grep -q "YSFEncoder network packet:" "$LOG_FILE"

echo "PASS: DMR to YSF scaffold regression"

echo
echo "Key log lines:"
grep -E "DMR MediaFrame created|FROM=DMR TO=YSF|YSFFich build|YSFVoiceFrame build|YSFEncoder network packet" "$LOG_FILE"
