/**
  * Copyright (C) 2017-2018 CompCom
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License
  * as published by the Free Software Foundation; either version 3
  * of the License, or (at your option) any later version.
  */

#include "controller.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <sstream>

Controller::Controller(int id)
{
#ifdef __arm__
    std::stringstream s;
    s << "/dev/input/by-path/platform-twi." << id << "-event-joystick";
    fd = open(s.str().c_str(), O_RDONLY | O_NONBLOCK);
    if(fd == -1)
    {
      std::cerr << "Cannot access controller.\n";
      exit(1);
    }
#endif // __arm__

    Reset();
}
Controller::~Controller()
{
#ifdef __arm__
    if(fd != -1)
        close(fd);
#endif // __arm__
}
bool Controller::PeekButtonStatus(GameButton button)
{
    return buttons[button];
}
bool Controller::GetButtonStatus(GameButton button)
{
    bool result = buttons[button];
    if(result) buttons[button] = 0;
    return result;
}
void Controller::Update()
{
#ifdef __arm__
    int len;
    while((len = read(fd, &buttonBuffer, sizeof(ButtonEvent)*10)) > 0)
    {
        len/=sizeof(ButtonEvent);
        for(int i = 0; i < len; ++i)
        {
            auto & buttonEvent = buttonBuffer[i];
            //Unk1 equals 1 when displaying button press status
            if(buttonEvent.unk1 == 1)
            {
                buttons[buttonEvent.button] = buttonEvent.pressed;
            }
        }
    }
#endif // __arm__
}
void Controller::Reset()
{
    buttons[A] = 0;
    buttons[B] = 0;
    buttons[X] = 0;
    buttons[Y] = 0;
    buttons[L] = 0;
    buttons[R] = 0;
    buttons[SELECT] = 0;
    buttons[START] = 0;
    buttons[LEFT] = 0;
    buttons[RIGHT] = 0;
    buttons[UP] = 0;
    buttons[DOWN] = 0;
}
