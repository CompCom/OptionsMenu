#!/bin/bash

echo "Testing internet connection..."
sleep 1
if nc -zw1 google.com 443; then
  echo "You are already connected to the internet."
else
  echo "Restarting network. Please wait."
  /etc/init.d/S92networking restart
  echo "Testing internet connection..."
sleep 5
  if nc -zw1 google.com 443; then
    echo "Success! You are now connected to the internet."
  else
    echo "Connection failed. Please connect your WiFi adapter."
  fi
fi


