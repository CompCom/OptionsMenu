#!/bin/sh

if [ ! -f "/media/data/ra_backup/retroarch.cfg" ]; then
  echo "Backup file does not exist."
else
  /etc/options_menu/retroarch/scripts/RASettingsBackup "Restore" "/etc/options_menu/retroarch/default_files/retroarch.cfg" "/etc/libretro/retroarch.cfg" "/media/data/ra_backup/retroarch.cfg"
  /etc/options_menu/retroarch/scripts/RASettingsBackup "Restore" "/etc/options_menu/retroarch/default_files/retroarch-core-options.cfg" "/etc/libretro/retroarch-core-options.cfg" "/media/data/ra_backup/retroarch-core-options.cfg"
  rm -rf /etc/libretro/.config/retroarch/config/
  cp -r /media/data/ra_backup/config /etc/libretro/.config/retroarch
  echo "Retroarch configs restored."
fi
