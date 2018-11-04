#!/bin/sh

# Get Sprite Sheet code based on External UI hmod by DanTheMan827

source /etc/preinit
script_init

case "$sftype-$sfregion" in
    nes-usa)
      echo -ne "$mountpoint/usr/share/clover-ui/resources/sprites/nes.png"
      ;;
    nes-eur)
      echo -ne "$mountpoint/usr/share/clover-ui/resources/sprites/nes.png"
      ;;
    nes-jpn)
      echo -ne "$mountpoint/usr/share/clover-ui/resources/sprites/hvc.png"
      ;;
    hvcj-jpn)
      echo -ne "$mountpoint/usr/share/ui/hvc/resources/sprites/packed.png"
      ;;
    snes-usa)
      echo -ne "$mountpoint/usr/share/ui/snes-usa/resources/sprites/packed.png"
      ;;
    snes-eur)
      echo -ne "$mountpoint/usr/share/ui/snes-eur/resources/sprites/packed.png"
      ;;
    snes-jpn)
      echo -ne "$mountpoint/usr/share/ui/shvc/resources/sprites/packed.png"
      ;;
esac
