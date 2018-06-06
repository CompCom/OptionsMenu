#!/bin/sh

#Create Data directories if it's not already created and mounted...
mkdir -m 777 /media/data/
mkdir -m 777 /media/data/log
mkdir -m 777 /media/data/log/RetroArchConfig

#Clear down the log files
rm /media/data/log/Hakchi_retroarch_debug.log
rm -rf /media/data/log/RetroArchConfig/*

echo "$(chmod -R 777 /etc/libretro)" >> /media/data/log/Hakchi_retroarch_debug.log #Set full permissions for retroarch
echo "Dumping retroarch config to log folder..." >> /media/data/log/Hakchi_retroarch_debug.log
echo "$(cp -r /etc/libretro/.config /media/data/log/RetroArchConfig)" >> /media/data/log/Hakchi_retroarch_debug.log
echo "$(cp -r /etc/libretro/retroarch-core-options.cfg /media/data/log/RetroArchConfig)" >> /media/data/log/Hakchi_retroarch_debug.log
echo "$(cp -r /etc/libretro/retroarch.cfg /media/data/log/RetroArchConfig)" >> /media/data/log/Hakchi_retroarch_debug.log
if /bin/retroarch-mini null &> /media/data/log/Hakchi_retroarch_debug.log ; then
	echo "RetroArch successfully ran using the test rig at $(date)" >> /media/data/log/Hakchi_retroarch_debug.log
else
	echo "RetroArch FAILED to run using the test rig at $(date)" >> /media/data/log/Hakchi_retroarch_debug.log
fi