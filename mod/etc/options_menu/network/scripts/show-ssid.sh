#!/bin/bash

echo "Scanning for available networks..."
wpa_cli scan >/dev/null 2>& 1
if  ( ifconfig wlan0 | head -3 | tail -1 | grep UP ) > /dev/null 2>& 1
then
  wpa_cli scan_results | sed -r "s/\\/.*//g;s/'wlan0'//g;" | awk '{print $4" "$5}'
else
  echo "Cannot find any WiFi Networks."
fi
echo "Scan complete."
