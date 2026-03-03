#!/bin/sh

BAT_INFO=$(acpi -b)
PERCENT=$(echo "$BAT_INFO" | awk -F'[,:%]' '{print $3}')
STATUS=$(echo "$BAT_INFO" | awk '{print $3}' | tr -d ',')

# Define colors using statuscolors patch syntax
RED="^c#FF0000^"
YELLOW="^c#FFFF00^"
GREEN="^c#00FF00^"
RESET="^d^"

if [ "$STATUS" = "Discharging" ]; then
    if [ "$PERCENT" -le 15 ]; then
        COLOR=$RED
    elif [ "$PERCENT" -le 40 ]; then
        COLOR=$YELLOW
    else
        COLOR=$GREEN
    fi
else
    COLOR=$GREEN
fi

echo "${COLOR} ${PERCENT}% ${STATUS}${RESET} "
