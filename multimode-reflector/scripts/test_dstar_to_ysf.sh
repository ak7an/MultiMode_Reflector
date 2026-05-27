#!/bin/bash

set -e

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
cd "$ROOT"

echo "[TEST] Clearing packet capture..."
: > packet_capture.log

echo "[TEST] Replaying D-Star sample..."
./packet_replay \
    testdata/dstar_clean_19_replay.log \
    127.0.0.1 \
    9000

sleep 1

echo "[TEST] Dumping YSF packets..."
./ysf_packet_dump packet_capture.log > /tmp/ysf_test_output.txt

cat /tmp/ysf_test_output.txt | head -20

PACKETS=$(grep -c '^YSFD packet #' /tmp/ysf_test_output.txt)

if [ "$PACKETS" -ne 19 ]; then
    echo "[FAIL] Expected 19 YSFD packets, got $PACKETS"
    exit 1
fi

if ! grep -q 'fich FI   : VOICE' /tmp/ysf_test_output.txt; then
    echo "[FAIL] Missing semantic FICH decode"
    exit 1
fi

if ! grep -q 'src       : \[TEST' /tmp/ysf_test_output.txt; then
    echo "[FAIL] Missing source callsign"
    exit 1
fi

echo "[PASS] DSTAR -> YSFD regression test passed"
