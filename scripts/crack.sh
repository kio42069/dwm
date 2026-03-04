#!/bin/sh

# Move to 9th workspace (tag 9)
xdotool key super+9

# Toggle (collapse) bar
xdotool key super+b

# Change wallpaper (set your image path)
feh --bg-fill "$HOME/Pictures/wallpapers/other.jpg"
