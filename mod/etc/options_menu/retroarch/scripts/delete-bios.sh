#!/bin/sh

mv /etc/libretro/system/NstDatabase.xml /etc/libretro
mv /etc/libretro/system/PPSSPP /etc/libretro
rm -rf /etc/libretro/system/*
mv /etc/libretro/NstDatabase.xml /etc/libretro/system
mv /etc/libretro/PPSSPP /etc/libretro/system
echo "BIOS file(s) deleted."
