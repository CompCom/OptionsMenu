/**
  * Copyright (C) 2017-2018 CompCom
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License
  * as published by the Free Software Foundation; either version 3
  * of the License, or (at your option) any later version.
  */

#include "command.h"
#include "framework/controller.h"

#include <fstream>
#include <list>
#include <vector>

Command::Command() {}

Command::Command(std::ifstream & in)
{
    std::string temp;
    while(in.good())
    {
        std::getline(in, temp);
        if(temp[0]=='#')
            continue;
        int index = temp.find('=');
        std::string param = temp.substr(0, index), value = temp.substr(index+1);
        if(param.compare("COMMAND_NAME")==0)
            name = value;
        else if(param.compare("COMMAND_TYPE")==0)
            runInternal = (value == "INTERNAL");
        else if(param.compare("RESTART_UI")==0)
            restartUI = (value == "TRUE");
        else if(param.compare("IGNORE_INTERRUPT")==0)
            ignoreInterrupt = (value == "TRUE");
        else if(param.compare("COMMAND_STR")==0)
            command = value;
        else if(param.compare("USB_ONLY")==0)
            usbOnly = (value == "TRUE");
        else if(param.compare("PREVIEW_IMAGE")==0)
            previewImage = value;
        else if(param.compare("PREVIEW_IMAGE_X")==0)
            previewImageX = std::stoi(value);
        else if(param.compare("PREVIEW_IMAGE_Y")==0)
            previewImageY = std::stoi(value);
    }
    in.close();
}

void Command::RunCommand(SDL_Context & sdl_context, Controller * controller, Sprite & menuL, Sprite & menuU) const
{
    std::list<Texture> textList;
    FILE* pipe = popen(command.c_str(), "r");
    if(pipe)
    {
        auto renderer = sdl_context.renderer;
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        char buffer[128] = {0};

        auto render = [&](Texture * closeText = nullptr)
        {
            sdl_context.StartFrame();
            menuU.Draw(renderer);
            menuL.Draw(renderer);
            int y = 100;
            for(auto & t : textList)
            {
                t.rect.y = y;
                t.Draw(renderer);
                y+=10;
            }
            if(closeText)
                closeText->Draw(renderer);
            sdl_context.EndFrame();
        };

        while(!feof(pipe))
        {
            if(fgets(buffer, 128, pipe) > 0)
            {
                std::string sBuffer(buffer);
                int pos = sBuffer.find('\n');
                if(pos > 0)
                    sBuffer[pos] = '\0';
                textList.push_back(Texture(sBuffer, 8, renderer, 30));
                
                if(textList.size() > 40)
                {
                    textList.erase(textList.begin());
                }
            }

            controller->Update();
            if(!ignoreInterrupt && controller->GetButtonStatus(B))
                break;

            render();
        }
        pclose(pipe);
        Texture closeText("Press B to exit.", 12, renderer, 30, 610);

        while (!controller->GetButtonStatus(B))
        {
            controller->Update();
            render(&closeText);
        }
        SDL_SetRenderDrawColor(renderer, 0x6e, 0x6e, 0x6e, 0xFF);
    }
}
