#!/bin/sh

PROGRAM="/usr/bin/butler"
PID=$(pidof $PROGRAM)

GDB="gdb -annotate 3 -statistics -d /usr/src -e $PROGRAM -s /usr/lib/debug$PROGRAM -p $PID"

GDB="gdb -d /usr/src $PROGRAM -p $PID"

GDB="gdb -d /usr/src $PROGRAM"

echo --------------------------------------------------------------------------
echo $GDB
echo --------------------------------------------------------------------------

$GDB

