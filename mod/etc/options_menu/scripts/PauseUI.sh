#!/bin/sh

source /etc/preinit
script_init

if type -t uipause 1> /dev/null; then
  uipause
else
  lsof -n | grep -F "/dev/fb0" | awk '{print $1}' | sort -u | xargs -r kill -s STOP
fi
