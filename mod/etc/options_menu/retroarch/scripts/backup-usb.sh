#!/bin/sh

Backup_To_USB()
{
  if [ -d /media/data/ra_backup ]; then
    rm -rf /media/data/ra_backup/
  fi

  mkdir -pm 777 /media/data/ra_backup
  /etc/options_menu/retroarch/scripts/RASettingsBackup "Backup" "/etc/options_menu/retroarch/default_files/retroarch.cfg" "/etc/libretro/retroarch.cfg" "/media/data/ra_backup/retroarch.cfg"
  /etc/options_menu/retroarch/scripts/RASettingsBackup "Backup" "/etc/options_menu/retroarch/default_files/retroarch-core-options.cfg" "/etc/libretro/retroarch-core-options.cfg" "/media/data/ra_backup/retroarch-core-options.cfg"
  cp -r /etc/libretro/.config/retroarch/config /media/data/ra_backup
}

Backup_To_USB && echo "Retroarch config backed up to USB:/data/ra_backup"
