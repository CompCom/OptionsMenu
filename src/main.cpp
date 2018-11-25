/**
  * Copyright (C) 2017-2018 CompCom
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License
  * as published by the Free Software Foundation; either version 3
  * of the License, or (at your option) any later version.
  */

#include "framework/sdl_helper.h"
#include "framework/controller.h"
#include "framework/powerwatch.h"
#include "command.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <dirent.h>

void sReplace(std::string & command, std::string oldString, std::string newString)
{
    size_t pos;
    while((pos = command.find(oldString)) != std::string::npos)
    {
        command.replace(pos, oldString.size(), newString);
    }
}

int main(int argc, char * argv[])
{
    std::string optionsLocation(argv[0]);
    optionsLocation = optionsLocation.substr(0, optionsLocation.find_last_of('/')+1);
    std::string commandLocation(optionsLocation + "commands/");
    std::string scriptLocation(optionsLocation + "scripts/");
    std::string spriteSheetLocation;
    std::ifstream in("/tmp/spritesheet");
    std::getline(in, spriteSheetLocation);
    in.close();
    std::string titleString("Options");

    //Check for external drive
    bool usbReady = false;
    if(auto dir = opendir("/media/hakchi/"))
    {
        closedir(dir);
        usbReady = true;
    }

    //Parse args
    for(int i = 1; i < argc; ++i)
    {
        if(strcmp(argv[i], "--commandPath") == 0)
        {
            commandLocation = argv[i+1];
            ++i;
        }
        else if(strcmp(argv[i], "--scriptPath") == 0)
        {
            scriptLocation = argv[i+1];
            ++i;
        }
        else if(strcmp(argv[i], "--title") == 0)
        {
            titleString = argv[i+1];
            ++i;
        }
    }

    //Read commands from command folder
    std::vector<Command> commands;
    if(auto dir = opendir(commandLocation.c_str()))
    {
        //Find all commands in folder
        std::list<std::string> fileList;
        while(auto entry = readdir(dir))
        {
            if(entry->d_type == DT_REG && entry->d_name[0] == 'c')
                fileList.push_back(entry->d_name);
        }
        closedir(dir);

        fileList.sort();
        for(auto & file : fileList)
        {
            in.open(commandLocation+file);
            if(in.is_open())
            {
                Command c(in);
                if(!(commands.size() == 0 && c.command.size() == 0))
                {
                    if(!c.usbOnly || (c.usbOnly && usbReady))
                    {
                        sReplace(c.command, "%options_path%", optionsLocation);
                        sReplace(c.command, "%script_dir%", scriptLocation);
                        commands.push_back(c);
                    }
                }
            }
        }
    }
    else
    {
        std::cerr << "Cannot open input folder.\n";
        exit(1);
    }

    int currentCommandId = 0;

    //Create SDL Window/Renderer and Controller Handler
    SDL_Context sdl_context(std::chrono::milliseconds(33), false);
    auto renderer = sdl_context.renderer;
    Controller controller(1);

    //Create Options Flag to prevent multiple menu launches
    system("touch /tmp/options.flag");

    //Create Background
    auto spriteSheet = std::shared_ptr<SDL_Texture>(LoadTexturePNG(renderer, spriteSheetLocation), SDL_DestroyTexture);
    Texture banner(optionsLocation + "/images/banner.png", renderer, 640, 185, true);
    Sprite menuU = { spriteSheet, {1,43,432,40}, {-8,-24,1296,120} };
    Sprite menuL = { spriteSheet, {1,1,432,40}, {-8,630,1296,120} };
    SDL_SetRenderDrawColor(renderer, 0x6e, 0x6e, 0x6e, 0xFF);

    //Create Textures for Strings
    Texture titleText(titleString, 36, renderer, 640, 185, true);
    Texture pointerText("->", 16, renderer, 20, 260, false, 0xFF00FF00);
    SDL_Rect & pointerRect = pointerText.rect;
    Texture CompComText("created by CompCom", 16, renderer, 1100, 620, true);

    //Create Command Texture
    for(Command & c : commands)
        c.texture = Texture(c.name, 16, renderer, 50, 0);

    const int DisplayItemCount = 16;
    int topListItemNumber = 1;
    std::shared_ptr<Texture> PreviewImage;
    auto SetCurrentCommand = [&] (int newCommandId)
    {
        currentCommandId = newCommandId;
        const Command & currentCommand = commands[currentCommandId];

        bool updateCommandYPos = false;
        if(currentCommandId < topListItemNumber)
        {
            topListItemNumber = currentCommandId;
            updateCommandYPos = true;
        }
        else if(currentCommandId >= topListItemNumber+DisplayItemCount)
        {
            topListItemNumber = currentCommandId-DisplayItemCount+1;
            updateCommandYPos = true;
        }
        if(updateCommandYPos)
        {
            int y = 260;
            for(int i = 0, count = std::min(DisplayItemCount,static_cast<int>(commands.size())); i < count; ++i)
            {
                commands[i+topListItemNumber].texture.rect.y = y;
                y += 18;
            }
        }

        pointerRect.y = currentCommand.texture.rect.y;
        if(currentCommand.previewImage.size())
            PreviewImage = std::make_shared<Texture>(currentCommand.previewImage, renderer, currentCommand.previewImageX, currentCommand.previewImageY);
        else
            PreviewImage.reset();
    };
    SetCurrentCommand(0);

    const std::string exitCommand = "/bin/sh " + optionsLocation +"/scripts/ResumeUI.sh";
    for(;;)
    {
        //Clear Buffer and Update Input State
        sdl_context.StartFrame();
        controller.Update();

        SDL_Event e;
        //Poll for quit event (CTRL+C)/Close
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                return 0;
            }
        }

        if(sdl_context.powerwatch->buttonPress())
        {
            system(exitCommand.c_str());
            break;
        }
        if(controller.GetButtonStatus(A) || controller.GetButtonStatus(START))
        {
            if(commands[currentCommandId].runInternal)
            {
                commands[currentCommandId].RunCommand(sdl_context, &controller, menuL, menuU);
            }
            else
            {
                system(commands[currentCommandId].command.c_str());
                if(commands[currentCommandId].restartUI)
                    system(exitCommand.c_str());
                break;
            }
        }
        else if(controller.GetButtonStatus(UP))
        {
            int newCommandId = currentCommandId;
            do { newCommandId = (newCommandId-1+commands.size())%commands.size(); }
            while(commands[newCommandId].command.size() == 0);
            SetCurrentCommand(newCommandId);
        }
        else if(controller.GetButtonStatus(DOWN))
        {
            int newCommandId = currentCommandId;
            do { newCommandId = (newCommandId+1)%commands.size(); }
            while(commands[newCommandId].command.size() == 0);
            SetCurrentCommand(newCommandId);
        }
        else if(controller.GetButtonStatus(B))
            SetCurrentCommand(commands.size()-1);

        //Draw all textures
        banner.Draw(renderer);
        menuU.Draw(renderer);
        menuL.Draw(renderer);
        titleText.Draw(renderer);
        for(int i = 0, count = std::min(DisplayItemCount,static_cast<int>(commands.size())); i < count; ++i)
        {
            commands[i+topListItemNumber].texture.Draw(renderer);
        }
        if(PreviewImage.get())
            PreviewImage->Draw(renderer);
        pointerText.Draw(renderer);
        CompComText.Draw(renderer);

        //Render Framebuffer and Wait for Next Frame
        sdl_context.EndFrame();
    }

    return 0;
}
