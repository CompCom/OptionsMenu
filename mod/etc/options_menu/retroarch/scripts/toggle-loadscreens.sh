#!/bin/sh

source /etc/preinit
script_init

ok=0

echo "Toggling RetroArch and Canoe loading screens." && echo " "

if [ -f "$rootfs/share/retroarch/assets/RAloading-min.png" ] && [ -f "$rootfs/share/retroarch/assets/loading-min.png" ]; then
  mv  "$rootfs/share/retroarch/assets/RAloading-min.png" "$rootfs/share/retroarch/assets/RAloading-min.png_DISABLED"
  echo "RetroArch loading screen disabled..." && echo " "
  mv  "$rootfs/share/retroarch/assets/loading-min.png" "$rootfs/share/retroarch/assets/loading-min.png_DISABLED"
  echo "Canoe loading screen disabled..." && echo " "
  ok=1
fi

if [ -f "$rootfs/share/retroarch/assets/RAloading-min.png_DISABLED" ] && [ -f "$rootfs/share/retroarch/assets/loading-min.png_DISABLED" ] && [ "$ok" == 0 ]; then
  mv  "$rootfs/share/retroarch/assets/RAloading-min.png_DISABLED" "$rootfs/share/retroarch/assets/RAloading-min.png"
  echo "RetroArch loading screen enabled..." && echo " "
  mv  "$rootfs/share/retroarch/assets/loading-min.png_DISABLED" "$rootfs/share/retroarch/assets/loading-min.png"
  echo "Canoe loading screen enabled..." && echo " "
  ok=1
fi

if [ "$ok" == 1 ]; then
  echo "RetroArch load screen toggle complete!"
else
  echo "Failed to locate load screens... Please ensure you are using the latest RetroArch 'Neo' Build!"
fi
