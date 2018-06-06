#!/bin/sh

# Get Sprite Sheet code based on External UI hmod by DanTheMan827

source /etc/preinit
script_init

case "$(cat "$mountpoint/etc/clover/boardtype")-$(cat "$mountpoint/etc/clover/REGION")" in
    dp-nes-EUR_USA)
      echo -ne "$mountpoint/usr/share/clover-ui/resources/sprites/nes.png"
      ;;
    dp-hvc-JPN)
      echo -ne "$mountpoint/usr/share/clover-ui/resources/sprites/hvc.png"
      ;;
    dp-shvc-USA)
      echo -ne "$mountpoint/usr/share/ui/snes-usa/resources/sprites/packed.png"
      ;;
    dp-shvc-EUR)
      echo -ne "$mountpoint/usr/share/ui/snes-eur/resources/sprites/packed.png"
      ;;
    dp-shvc-JPN)
      echo -ne "$mountpoint/usr/share/ui/shvc/resources/sprites/packed.png"
      ;;
esac
