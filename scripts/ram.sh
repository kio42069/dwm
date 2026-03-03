#!/bin/bash
RAM=$(free -h | awk '/^Mem/ { print$3"/"$2 }' | sed s/i//g)
echo "^c#b2d4fa^"󰍛 $RAM"^d^ "
