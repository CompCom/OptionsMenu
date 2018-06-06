#!/bin/sh

source /etc/preinit
script_init

Backup_To_NAND()
{
  if [ -d "$rootfs/etc/ra_backup" ]; then
    rm -rf "$rootfs/etc/ra_backup/"
  fi

  mkdir -pm 777 "$rootfs/etc/ra_backup"
  /etc/options_menu/retroarch/scripts/RASettingsBackup "Backup" "/etc/options_menu/retroarch/default_files/retroarch.cfg" "/etc/libretro/retroarch.cfg" "$rootfs/etc/ra_backup/retroarch.cfg"
  /etc/options_menu/retroarch/scripts/RASettingsBackup "Backup" "/etc/options_menu/retroarch/default_files/retroarch-core-options.cfg" "/etc/libretro/retroarch-core-options.cfg" "$rootfs/etc/ra_backup/retroarch-core-options.cfg"
  cp -r /etc/libretro/.config/retroarch/config "$rootfs/etc/ra_backup"
}

Backup_To_NAND && echo "Retroarch config backed up to /etc/ra_backup"
