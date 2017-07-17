#!/bin/sh

# Usage:
# (1) su to switch to super user firstly
# (2) source ./env.sh
# (3) ./debug.sh <application name>

if [ $# -eq 0 ]
then
    echo './debug.sh <application name>'
    exit 1
fi

./sdk/tools/OpenOCD/ramload.py ./bin/mw302_rd/$1.axf
