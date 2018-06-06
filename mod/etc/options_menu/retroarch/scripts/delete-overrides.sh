#!/bin/sh

mv /etc/libretro/.config/retroarch/config/remaps /etc/libretro/.config/retroarch/
rm -r /etc/libretro/.config/retroarch/config/*
mv /etc/libretro/.config/retroarch/remaps /etc/libretro/.config/retroarch/config/
echo "Overrides deleted."
