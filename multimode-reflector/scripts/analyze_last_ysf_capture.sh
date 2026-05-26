#!/bin/bash
set -e

for FILE in $(ls -t captures/ysf/*.txt 2>/dev/null); do
    HEX=$(grep "Received" "$FILE" | tail -1 | sed 's/^.*bytes: //')

    if [ -n "$HEX" ]; then
        echo "Using capture: $FILE"
        ./ysf_analyze "$HEX"
        exit 0
    fi
done

echo "No packet data found in any YSF capture."
exit 1
