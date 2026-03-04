#!/bin/sh
apps=$(grep -r "^Name=" /usr/share/applications ~/.local/share/applications 2>/dev/null | cut -d= -f2)
choice=$(printf "%s\n" "$apps" | dmenu -i -b -fn "JetBrainsMono Nerd Font:size=10" -nb \#000000 -nf \#ffffff -sb \#b2d4fa -sf \#000000)
file=$(grep -rl "Name=$choice" /usr/share/applications ~/.local/share/applications 2>/dev/null | head -n1)
[ -n "$file" ] && gtk-launch "$(basename "$file" .desktop)"
