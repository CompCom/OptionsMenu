/**
  * Copyright (C) 2017-2018 CompCom
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License
  * as published by the Free Software Foundation; either version 3
  * of the License, or (at your option) any later version.
  */

#ifndef COMMAND_H_
#define COMMAND_H_

#include "framework/sdl_helper.h"

class Controller;

struct Command
{
    Command();
    Command(std::ifstream & in);
    void RunCommand(SDL_Context & sdl_context, Controller * controller, Sprite & menuL, Sprite & menuU) const;

    std::string name;
    std::string command;
    bool runInternal = true;
    bool restartUI = false;
    bool ignoreInterrupt = false;
    bool usbOnly = false;
    Texture texture;
    std::string previewImage;
    int previewImageX = 0;
    int previewImageY = 0;
    int previewImageWidth = -1;
    int previewImageHeight = -1;
};

#endif
