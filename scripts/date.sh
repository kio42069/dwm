#!/bin/sh

DATE=$(date '+%d-%I:%M ')

# Define colors using statuscolors patch syntax
RED="^c#FF0000^"
YELLOW="^c#FFFF00^"
GREEN="^c#00FF00^"
BLUE="^c#00f7ca^"
RESET="^d^"

echo "${BLUE}󰸘 ${RESET}${BLUE}${DATE}${RESET}"
