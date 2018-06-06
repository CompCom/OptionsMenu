/**
  * Copyright (C) 2017-2018 CompCom
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License
  * as published by the Free Software Foundation; either version 3
  * of the License, or (at your option) any later version.
  */

#ifndef POWERWATCH_H_
#define POWERWATCH_H_

struct PowerWatch
{
    int powerFd = -1, resetFd = -1;

    PowerWatch();
    ~PowerWatch();
    bool buttonPress();
};
#endif
