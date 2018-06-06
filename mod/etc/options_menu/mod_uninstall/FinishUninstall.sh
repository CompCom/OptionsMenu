#!/bin/sh

source /etc/preinit
script_init

mkdir -p "$installpath/transfer"
copy "/tmp/uninstall" "$installpath/transfer"
sync
reboot
