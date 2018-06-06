#!/bin/sh

#Create Data directories if it's not already created and mounted...
mkdir -m 777 /media/data/
mkdir -m 777 /media/data/log

#Clear down the log files
rm /media/data/log/Hakchi_file_structure.log

echo "Dumping complete file structure to /media/data/log/ ..."
echo "$(ls -lhAR /)" &> /media/data/log/Hakchi_file_structure.log
echo "File structure dump was successful!"
