#!/bin/sh

[ ! -d "/media/data/ra_bios" ] && mkdir -p /media/data/ra_bios
rsync -a /media/data/ra_bios/* /etc/libretro/system
rsync -a /etc/libretro/system/* /media/data/ra_bios
echo "BIOS file(s) transfered."
