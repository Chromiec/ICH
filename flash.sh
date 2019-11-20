#!/bin/bash

# Usage: ./flash.sh <SERIAL_PORT> <HEX_FILE>

if [ $# -eq 0 ]; then
    SERIAL_PORT="/dev/ttyUSB0"
else
    SERIAL_PORT=$1
fi

if [ $# -lt 2  ]; then
    HEX_FILE="blinky.hex"
else
    HEX_FILE=$2
fi

# Make sure hex file exists
if [ ! -f "$HEX_FILE" ]; then
    echo -e "\033[1;31m$HEX_FILE not found\033[0m"
    echo "Make sure to compile first"
    exit 1
fi

# Make sure hex file exists
if [ ! -e "$SERIAL_PORT" ]; then
    echo -e "\033[1;31m$SERIAL_PORT not found\033[0m"
    echo "Make sure the device is connected properly"
    exit 1
fi

echo -e "\033[1;34mFlashing...\033[0m"
avrdude -patmega328p -carduino -P"$SERIAL_PORT" -b115200 -D -Uflash:w:"$HEX_FILE":i
