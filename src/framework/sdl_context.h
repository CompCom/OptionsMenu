/**
  * Copyright (C) 2017-2018 CompCom
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License
  * as published by the Free Software Foundation; either version 3
  * of the License, or (at your option) any later version.
  */

#ifndef SDL_CONTEXT_H_
#define SDL_CONTEXT_H_

#include <string>
#include <memory>
#include <chrono>

class PowerWatch;
class SDL_Window;
class SDL_Renderer;

struct SDL_Context
{
public:
    SDL_Context(std::chrono::milliseconds fpsTime = std::chrono::milliseconds(33), bool powerButtonExit = true);
    ~SDL_Context();
    void StartFrame();
    void EndFrame();

    SDL_Window* window;
    SDL_Renderer* renderer;
    std::chrono::milliseconds fpsTime;
    PowerWatch * powerwatch;

private:
    std::chrono::time_point<std::chrono::system_clock> nextFrameTime;
    bool powerButtonExit;
};

#endif
