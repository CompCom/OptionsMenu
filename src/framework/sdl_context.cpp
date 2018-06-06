/**
  * Copyright (C) 2017-2018 CompCom
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License
  * as published by the Free Software Foundation; either version 3
  * of the License, or (at your option) any later version.
  */

#include "sdl_context.h"
#include "powerwatch.h"

#include <iostream>
#include <thread>
#include <SDL.h>

SDL_Context::SDL_Context(std::chrono::milliseconds fpsTime, bool powerButtonExit) : fpsTime(fpsTime), powerButtonExit(powerButtonExit)
{
    powerwatch = new PowerWatch();
    window = SDL_CreateWindow("", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, SDL_WINDOW_FULLSCREEN);
    renderer = SDL_CreateRenderer(window, -1, 0);
    if(renderer == NULL) {
        std::cerr << "Cannot create renderer.\n";
        SDL_DestroyWindow(window);
        exit(1);
    }
    nextFrameTime = std::chrono::system_clock::now()+fpsTime;
}

SDL_Context::~SDL_Context()
{
    delete powerwatch;
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void SDL_Context::StartFrame()
{
    if(powerButtonExit && powerwatch->buttonPress())
        exit(1);
    SDL_RenderClear(renderer);
}

void SDL_Context::EndFrame()
{
    SDL_RenderPresent(renderer);
    std::this_thread::sleep_until(nextFrameTime);
    nextFrameTime += fpsTime;
}
