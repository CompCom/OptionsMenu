#!/bin/bash

echo "Grabbing Internal IP..."

ifconfig | grep "inet " | grep -v 127.0.0.1|awk 'match($0, /([0-9]+\.[0-9]+\.[0-9]+\.[0-9]+)/) {print "Your internal IP is: " substr($0,RSTART,RLENGTH)}'

echo "Grabbing External IP... (Might take a bit)"

PUBLIC_IP=$(wget http://ipecho.net/plain -O - -q)

wget -q --tries=10 --timeout=10 --spider http://google.com

if [[ $? -eq 0 ]]; then
        echo "You are online!"
        echo "Your external IP is:" $PUBLIC_IP
else
        echo "You are offline"
fi
