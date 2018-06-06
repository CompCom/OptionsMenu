#!/bin/sh

if [ -f "/etc/options_menu/retroarch/default_files/retroarch.cfg" ]; then
  cp /etc/options_menu/retroarch/default_files/retroarch.cfg /etc/libretro/
  cp /etc/options_menu/retroarch/default_files/retroarch-core-options.cfg /etc/libretro/
  echo "Restored Default Settings."
else
  echo "Cannot locate default configuration files."
  echo "Please ensure you have the latest RetroArch 'Neo' installed"
fi
