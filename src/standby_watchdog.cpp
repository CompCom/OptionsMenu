/**
  * Copyright (C) 2018 Swingflip and CompCom
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License
  * as published by the Free Software Foundation; either version 3
  * of the License, or (at your option) any later version.
  */

#include "framework/controller.h"
#include "framework/powerwatch.h"

#include <thread>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/reboot.h>
#include <linux/reboot.h>

Controller c(1);
PowerWatch pw;
const auto fpsTime = std::chrono::milliseconds(200);
auto nextUpdateTime = std::chrono::system_clock::now()+fpsTime;

int DisplayMenu()
{
    system("standby ReloadImagePayload");
    system("cat /tmp/power_menu_screen > /dev/fb0");
    auto fpsTime = std::chrono::milliseconds(33); // Check for input faster
    //system("echo DEBUG: Displaying hibernate menu...");
    for(;;)
    {
        c.Update();
        if(c.PeekButtonStatus(A)) //Accept (Hibernate)
        {
            //system("echo DEBUG: Accepted hibernate...");
            return 1;
        }
        else if(c.PeekButtonStatus(X)) //Accept (Standby)
        {
            //system("echo DEBUG: Accepted standby...");
            return 2;
        }
        else if(c.PeekButtonStatus(Y)) //Cancel
        {
            //system("echo DEBUG: Cancelled hibernate...");
            return 3;
        }
        else if(pw.buttonPress())
        {
            return 3;
        }
        system("cat /tmp/power_menu_screen > /dev/fb0"); // This will fix the framebuffer getting overwritten by a stack process
        std::this_thread::sleep_until(nextUpdateTime);
        nextUpdateTime+=fpsTime;
    }
}

int GetState()
{
    for(;;)
    {
        c.Update();
        if(c.PeekButtonStatus(L) && c.PeekButtonStatus(R) && c.GetButtonStatus(UP))
        {
            system("standby DisplayMenu");
            return DisplayMenu();
        }
        else
        {
            std::this_thread::sleep_until(nextUpdateTime);
            nextUpdateTime+=fpsTime;
        }
    }
}

void Hibernate()
{
    system("standby Hibernate");

    for(;;)
    {
        c.Update();
        if(c.PeekButtonStatus(L) && c.PeekButtonStatus(R) && c.GetButtonStatus(UP)) //L+R+SELECT = Hibernate / Reboot from Hibernate
        {
            //system("echo Core Temperature at the time of reboot out of hibernation: $(hakchi hwmon)c");
            system("echo 1 > /sys/devices/virtual/disp/disp/attr/lcd");
            break;
        }
        else if(c.PeekButtonStatus(L) && c.PeekButtonStatus(R) && c.GetButtonStatus(DOWN))
        {
            //system("echo Core Temperature at the time of shutdown out of hibernation: $(hakchi hwmon)c");
            system("standby HibernateReboot &");
            exit(0);
        }
        else if(pw.buttonPress())
        {
            system("standby Resume &");
            exit(0);
        }
        std::this_thread::sleep_until(nextUpdateTime);
        nextUpdateTime+=fpsTime;
    }
}

void Standby()
{
    system("standby Standby");

    for(;;)
    {
        c.Update();
        if(c.PeekButtonStatus(L) && c.PeekButtonStatus(R) && c.GetButtonStatus(UP)) //L+R+SELECT = Hibernate / Reboot from Hibernate
        {
            //system("echo Core Temperature at the time of reboot out of standby: $(hakchi hwmon)c");
            system("echo Rebooting console out from standby mode...");
            sync();
            setuid(0);
            reboot(RB_AUTOBOOT);
            exit(0);
        }
        if(c.PeekButtonStatus(L) && c.PeekButtonStatus(R) && c.GetButtonStatus(DOWN))
        {
            //system("echo Core Temperature at the time of shutdown out of standby: $(hakchi hwmon)c");
            system("echo Shutting down console out from standby mode...");
            sync();
            setuid(0);
            reboot(LINUX_REBOOT_CMD_POWER_OFF);
            exit(0);
        }
        std::this_thread::sleep_until(nextUpdateTime);
        nextUpdateTime+=fpsTime;
    }
}

int main(int argc, char * argv[])
{
    if(argc == 2 && strcmp(argv[1], "--displayMenu") == 0)
    {
        switch(DisplayMenu())
        {
        case 1:
            Hibernate();
            break;
        case 2:
            Standby();
            break;
        }
        system("rm -f /tmp/power_menu_screen");
    }
    else
    {
        for(;;)
        {
            switch(GetState())
            {
            case 1:
                Hibernate();
                break;
            case 2:
                Standby();
                break;
            }
            system("standby Resume");
        }
    }
}
