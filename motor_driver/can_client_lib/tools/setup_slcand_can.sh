#!/bin/bash
# Make sure the script runs with super user privileges.
[ "$UID" -eq 0 ] || exec sudo bash "$0" "$@"

# setup slcad
# see https://canable.io/getting-started.html#socketcan-linux
sudo slcand -o -c -s6 /dev/ttyACM* can0
sudo ifconfig can0 txqueuelen 1000 # buffer size
sudo ifconfig can0 up
sudo ifconfig can0