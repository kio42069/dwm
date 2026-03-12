#!/bin/sh

case "$BLOCK_BUTTON" in
    1) alacritty -e nmtui & notify-send hi;;          # left click
    2) nmcli radio wifi off ;;       # middle click toggle off
    3) nmcli radio wifi on ;;        # right click toggle on
esac

ssid=$(nmcli -t -f active,ssid dev wifi | grep '^yes' | cut -d: -f2)

if [ -n "$ssid" ]; then
    echo "󰤨 $ssid "
else
    echo "󰤭 offline"
fi
