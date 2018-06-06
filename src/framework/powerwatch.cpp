/**
  * Copyright (C) 2017-2018 CompCom
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License
  * as published by the Free Software Foundation; either version 3
  * of the License, or (at your option) any later version.
  */

#include "powerwatch.h"

#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
#include <fstream>
#include <iostream>

PowerWatch::PowerWatch()
{
#ifdef __arm__
    if(auto dir = opendir("/dev/input/"))
    {
        while(auto entry = readdir(dir))
        {
            std::string name(entry->d_name);
            if(name.find("event") != std::string::npos)
            {
                std::ifstream in("/sys/class/input/"+name+"/device/name");
                if(in.good())
                {
                    std::string temp;
                    std::getline(in, temp);
                    in.close();
                    if(temp == "sunxi-knob")
                        powerFd = open(("/dev/input/"+name).c_str(), O_RDONLY | O_NONBLOCK);
                    else if(temp == "sunxi-keyboard")
                        resetFd = open(("/dev/input/"+name).c_str(), O_RDONLY | O_NONBLOCK);
                }
            }
        }
        closedir(dir);
    }
    else
    {
        std::cerr << "Cannot open input folder.\n";
        exit(1);
    }

    if(powerFd == -1 || resetFd == -1)
    {
        std::cerr << "Cannot access power buttons.\n";
        exit(1);
    }
#endif
}

PowerWatch::~PowerWatch()
{
    if(powerFd != -1)
        close(powerFd);
    if(resetFd != -1)
        close(resetFd);
}

bool PowerWatch::buttonPress()
{
#ifndef __arm__
    return 0;
#endif
    static char c[16];
    return (read(powerFd, c, 16) > 0) || (read(resetFd, c, 16) > 0);
}
