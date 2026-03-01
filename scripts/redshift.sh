#!/bin/bash
# redshift-toggle.sh
# Toggle Redshift 4500K using a state file

STATE_FILE="$HOME/.redshift_toggle_state"

# Read previous state, default to off
if [[ -f "$STATE_FILE" ]]; then
    STATE=$(cat "$STATE_FILE")
else
    STATE="off"
fi

if [[ "$STATE" == "off" ]]; then
    # Turn Redshift on
    redshift -O 4000
    echo "on" > "$STATE_FILE"
else
    # Turn Redshift off (reset to default)
    redshift -x
    echo "off" > "$STATE_FILE"
fi
