/**
  * Copyright (C) 2017-2020 CompCom
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License
  * as published by the Free Software Foundation; either version 3
  * of the License, or (at your option) any later version.
  */

#include "framework/controller.h"
#include "framework/powerwatch.h"
#include "framework/sdl_helper.h"

#include <iostream>
#include <chrono>
#include <thread>
#include <fstream>
#include <vector>

struct Mod
{
    std::string name;
    Texture texture;
    void Draw(SDL_Renderer* renderer)
    {
        texture.Draw(renderer);
    }
};

int main(int argc, char * argv[])
{
    std::string folderLocation(argv[0]);
    folderLocation = folderLocation.substr(0, folderLocation.find_last_of('/')+1);
    std::vector<Mod> installedMods;

    {
        char buf[256];
        auto pipe = popen("hakchi pack_list", "r");
        if(pipe)
        {
            while(!feof(pipe))
            {
                if(fgets(buf, 256, pipe) > 0)
                    installedMods.push_back({buf});
            }
            pclose(pipe);
        }
    }

    if(installedMods.size() == 0)
    {
        std::cerr << "Cannot find mods";
        return 1;
    }

    if(SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cerr << "Cannot initialise SDL.\n";
        return 1;
    }

    Controller controller(1);
    PowerWatch powerWatch;
    auto window = SDL_CreateWindow("", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, SDL_WINDOW_FULLSCREEN);
    auto renderer = SDL_CreateRenderer(window, -1, 0);
    if(renderer == NULL) {
        std::cerr << "Cannot create renderer.\n";
        SDL_DestroyWindow(window);
        return 1;
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);

    Texture titleText("Module Uninstaller", 36, renderer, 640, 60, true);
    Texture modListText("Installed Modules", 18, renderer, 30, 120);
    Texture modUninstallText("Modules To Uninstall", 18, renderer, 890, 120);

    for(Mod & mod : installedMods)
    {
        mod.texture = Texture(mod.name, 14, renderer);
        mod.texture.rect.x = 60;
    }

    Texture pointerTexture("->", 14, renderer, 30, 150, false, 0xFF00FF00);
    Texture instructionsText("START - Uninstall Mods/Exit   A - Add mod to uninstall list   B - Undo", 12, renderer, 640, 648, true);
    int installListOffset = 0;
    int currentId = 0;
    const int displayCount = 20;
    std::vector<Mod> uninstallList;
    const auto fpsTime = std::chrono::milliseconds(33);
    auto nextFrameTime = std::chrono::system_clock::now()+fpsTime;
    for(;;)
    {
        SDL_Event e;
        //Poll for quit event (CTRL+C)/Close
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                return 0;
            }
        }

        controller.Update();
        if(powerWatch.buttonPress())
        {
            break;
        }
        if(controller.GetButtonStatus(UP))
        {
            if(currentId > 0)
            {
                currentId--;
                pointerTexture.rect.y = 150 + 20*currentId;
            }
            else if(installListOffset)
            {
                installListOffset--;
            }
        }
        else if(controller.GetButtonStatus(DOWN))
        {
            if(currentId < displayCount-1 && installListOffset + currentId+1 < installedMods.size())
            {
                currentId++;
                pointerTexture.rect.y = 150 + 20*currentId;
            }
            else if(installListOffset + displayCount < installedMods.size())
            {
                installListOffset++;
            }
        }
        else if(controller.GetButtonStatus(A))
        {
            if(currentId != -1 && installedMods.size())
            {
                installedMods[installListOffset+currentId].texture.rect.x = 890;
                uninstallList.push_back(installedMods[installListOffset+currentId]);
                installedMods.erase(installedMods.begin()+installListOffset+currentId);
                if(installListOffset && (installListOffset + displayCount) > installedMods.size())
                {
                    --installListOffset;
                }
                if(installListOffset==0 && currentId >= installedMods.size())
                {
                    currentId = installedMods.size()-1;
                    pointerTexture.rect.y = 150 + 20*currentId;
                }
            }
        }
        else if(controller.GetButtonStatus(B))
        {
            if(uninstallList.size())
            {
                uninstallList.back().texture.rect.x = 60;
                installedMods.push_back(uninstallList.back());
                uninstallList.pop_back();
                if(currentId == -1)
                {
                    currentId = 0;
                    pointerTexture.rect.y = 150;
                }
            }
        }
        else if(controller.GetButtonStatus(START))
        {
            if(uninstallList.size() == 0)
                break;
            Texture warning("Are you sure you want to remove these modules?", 24, renderer, 640, 248, true);
            Texture buttons("Yes - Start+Select                No - B", 24, renderer, 640, 300, true);
            for(;;)
            {
                controller.Update();
                if(controller.GetButtonStatus(B) || powerWatch.buttonPress())
                {
                    uninstallList.clear();
                    break;
                }
                else if(controller.PeekButtonStatus(START) && controller.PeekButtonStatus(SELECT))
                {
                    break;
                }
                SDL_RenderClear(renderer);
                warning.Draw(renderer);
                buttons.Draw(renderer);
                SDL_RenderPresent(renderer);
                std::this_thread::sleep_until(nextFrameTime);
                nextFrameTime += fpsTime;
            }
            if(uninstallList.size())
            {
                std::ofstream out("/tmp/uninstall");
                if(out.is_open())
                {
                    for(auto & mod : uninstallList)
                    {
                        out << mod.name << " ";
                    }
                    out.close();
                    system(("/bin/sh " + folderLocation + "/FinishUninstall.sh &").c_str());
                }
            }
            break;
        }

        SDL_RenderClear(renderer);
        titleText.Draw(renderer);
        modListText.Draw(renderer);
        modUninstallText.Draw(renderer);
        pointerTexture.Draw(renderer);
        instructionsText.Draw(renderer);
        int y = 150;
        for(int i = installListOffset, len = i+std::min(displayCount,(int)installedMods.size()-i); i < len; ++i)
        {
            installedMods[i].texture.rect.y = y;
            installedMods[i].texture.Draw(renderer);
            y+=20;
        }
        y = 150;
        for(auto & mod : uninstallList)
        {
            mod.texture.rect.y = y;
            mod.texture.Draw(renderer);
            y+=20;
        }

        SDL_RenderPresent(renderer);

        //Simple FPS limiter
        std::this_thread::sleep_until(nextFrameTime);
        nextFrameTime += fpsTime;
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
