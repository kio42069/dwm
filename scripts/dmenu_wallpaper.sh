#!/usr/bin/env bash

# slightly scuffed wallpaper picker menu for use with pywal - uses nsxiv if installed, otherwise uses dmenu

FOLDER=~/Pictures/Wallpapers/ # wallpaper folder
# SCRIPT=~/scripts/pywal16 # script to run after wal for refreshing programs, etc.


menu () {
        if command -v nsxiv >/dev/null; then 
                CHOICE=$(nsxiv -otb $FOLDER/*)
        else 
                CHOICE=$(echo -e "Random\n$(command ls -v $FOLDER)" | dmenu -c -l 15 -i -p "Wallpaper: ")
        fi

        case $CHOICE in
                Random) 
                        wal -i "$FOLDER" -n --backend colorz # -n skips pywal's broken wallpaper setter
                        feh --bg-fill "$(< ~/.cache/wal/wal)" # reads the randomly chosen image path
                        ;; 
                *.*) 
                        wal -i "$CHOICE" -n --backend colorz
                        feh --bg-fill "$CHOICE"
                        ;;
                *) exit 0 ;;
        esac
}

case "$#" in
        0) menu ;;
        1) 
                wal -i "$1" -n --backend colorz
                # Check if argument is a directory (random) or a specific file
                if [ -d "$1" ]; then
                        feh --bg-fill "$(< ~/.cache/wal/wal)"
                else
                        feh --bg-fill "$1"
                fi
                ;;
        2) 
                wal -i "$1" --theme $2 -n
                if [ -d "$1" ]; then
                        feh --bg-fill "$(< ~/.cache/wal/wal)"
                else
                        feh --bg-fill "$1"
                fi
                ;;
        *) exit 0 ;;
esac
# case $CHOICE in
# 		Random) wal -i "$FOLDER" -o $SCRIPT ;; # dmenu random option
# 		*.*) wal -i "$CHOICE" -o $SCRIPT ;;
# 		*) exit 0 ;;
# esac
# }

# If given arguments:
# First argument will be used by pywal as wallpaper/dir path
# Second will be used as theme (use wal --theme to view built-in themes)

# case "$#" in
# 		0) menu ;;
# 		1) wal -i "$1" -o $SCRIPT ;;
# 		2) wal -i "$1" --theme $2 -o $SCRIPT ;;
# 		*) exit 0 ;;
# esac
