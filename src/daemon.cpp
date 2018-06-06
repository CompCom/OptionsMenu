/**
  * Copyright (C) 2017-2018 CompCom
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License
  * as published by the Free Software Foundation; either version 3
  * of the License, or (at your option) any later version.
  */

#include "framework/controller.h"

#include <thread>
#include <chrono>
#include <fstream>

int main()
{
    Controller c(1);
    GameButton button1 = L, button2 = R, button3 = R;
    
    std::ifstream in("/etc/options_menu/button.cfg");
    if(in.is_open())
    {
        int temp;
        in >> temp;
        button1 = (GameButton)temp;
        in >> temp;
        button2 = (GameButton)temp;
        in >> temp;
        button3 = (GameButton)temp;
        in.close();
    }
    
    system("sh /etc/options_menu/scripts/GetSpriteSheet.sh > /tmp/spritesheet");
    
    const auto waitTime = std::chrono::milliseconds(100);
    const short holdThreshold = 10;
    short holdCount = 0;
    
    for(;;)
    {
        c.Update();
        if(c.PeekButtonStatus(button1) && c.PeekButtonStatus(button2) && c.PeekButtonStatus(button3))
        {
            ++holdCount;
            if(holdCount >= holdThreshold)
            {
                if(system("[ ! -f \"/tmp/options.flag\" ] && [ $(free -m | awk 'NR==3 { print $4; }') -gt 35 ] && /bin/sh /etc/options_menu/scripts/PauseUI.sh") == 0)
                    system("exec /etc/options_menu/options");
                holdCount = 0;
                c.Update();
                c.Reset();
            }
        }
        else if(holdCount)
            holdCount = 0;
        std::this_thread::sleep_for(waitTime);
    }
    
    return 0;
}
