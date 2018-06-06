#!/bin/sh

echo "                  MM                      " 
echo "                MM  MM                    " 
echo "              MM      MM                  User: $USER" 
if [ -z "$HOSTNAME" ]; then
	echo "            MM          MM                Hostname:  Not Applicable" 
else
	echo "            MM          MM                Hostname: $HOSTNAME" 
fi
echo "  MMMMMMMMMM    MM  MM    MMMMMMMMMM      OS: $MODNAME $(uname -s -m)"  
echo "    MM          MM  MM          MM        Kernel: $(uname -r)" 
echo "      MM                      MM          Terminal Build:  v0.1 (alpha)" 
echo "        MM                  MM            Shell: SHELL" 
echo "          MM              MM              ModPath: $MODPATH" 
echo "        MM      MMMMMM      MM            Installed Binaries: $(ls /bin/ -1 | wc -l)" 
echo "      MM    MMMM      MMMM    MM          " 
echo "    MM  MMMM              MMMM  MM        " 
echo "  MMMMMM                      MMMMMM      " 
echo "  " 
sleep 2
echo "======================================================================================" 
echo "                          http://github.com/swingflip" 
echo "======================================================================================" 	   
echo "Disk mounting information:" 
echo "$(df)" 
#top has a big overhead for some reason on the mini. Doesn't give much useful information anyway...
sleep 2
echo "======================================================================================" 	
echo "CPU analysis information:" 
echo "$(top -b -n 1 | head -n 15)" 
sleep 2
echo "======================================================================================" 
echo "RAM analysis information:" 
echo "$(free)" 
echo "======================================================================================" 
echo "Benchmark ran successfully!!" 
echo "" 
