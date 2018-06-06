#!/bin/sh

source /etc/preinit
script_init

usleep 200000
if type -t uiresume 1> /dev/null; then
  uiresume
else
  lsof -n | grep -F "/dev/fb0" | awk '{print $1}' | sort -u | xargs -r kill -s CONT
fi
rm /tmp/options.flag
