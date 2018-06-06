#!/bin/sh
#
# Script originally by Daniel Radtke <DanTheMan827>
#
source /etc/preinit
script_init

script_path="$(dirname "$0")"

if [ "$cfg_usb_rw" == "y" ]; then
  unset cfg_usb_rw
  echo "USB write access disabled..."
else
  cfg_usb_rw='y'
  echo "USB write access enabled..."
fi

save_config
echo " "
echo "Rebooting console in 5 seconds..."

sleep(5)

reboot