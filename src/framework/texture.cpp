/**
  * Copyright (C) 2017-2018 CompCom
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License
  * as published by the Free Software Foundation; either version 3
  * of the License, or (at your option) any later version.
  */

#include "texture.h"
#include "font8x8_basic.h"

#include <iostream>
#include <cstring>
#include <png.h>

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    #define rmask 0xff000000
    #define gmask 0x00ff0000
    #define bmask 0x0000ff00
    #define amask 0x000000ff
#else
    #define rmask 0x000000ff
    #define gmask 0x0000ff00
    #define bmask 0x00ff0000
    #define amask 0xff000000
#endif

Texture::Texture() {}

Texture::Texture(const std::string & text, int fontSize, SDL_Renderer* renderer, int x, int y, bool centerText, const int color)
{
    texture = std::shared_ptr<SDL_Texture>(WriteText(text, fontSize, renderer, rect.w, rect.h, color), SDL_DestroyTexture);
    rect.x = (centerText) ? x-rect.w/2 : x;
    rect.y = (centerText) ? y-rect.h/2 : y;
}

Texture::Texture(const std::string & pngFilePath, SDL_Renderer* renderer, int x, int y, bool centerImg)
{
    texture = std::shared_ptr<SDL_Texture>(LoadTexturePNG(renderer, pngFilePath, &rect), SDL_DestroyTexture);
    rect.x = (centerImg) ? x-rect.w/2 : x;
    rect.y = (centerImg) ? y-rect.h/2 : y;
}

void Texture::Draw(SDL_Renderer* renderer)
{
    if(texture.get())
      SDL_RenderCopyEx(renderer, texture.get(), NULL, &rect, 0, NULL, SDL_FLIP_NONE);
}

void Texture::Draw(SDL_Renderer* renderer, SDL_RendererFlip flip_enum)
{
    if(texture.get())
      SDL_RenderCopyEx(renderer, texture.get(), NULL, &rect, 0, NULL, flip_enum);
}

void Sprite::Draw(SDL_Renderer * renderer)
{
    SDL_RenderCopyEx(renderer, texture.get(), &sRect, &dRect, 0, NULL, SDL_FLIP_NONE);
}

SDL_Texture * LoadTexturePNG(SDL_Renderer *renderer, std::string file, SDL_Rect * rect)
{
    SDL_Texture* texture = nullptr;

    png_image png;
    memset(&png, 0, sizeof(png));
    png.version = PNG_IMAGE_VERSION;
    png.opaque = NULL;

    if(png_image_begin_read_from_file(&png, file.c_str()) == 0)
    {
        std::cerr << "Cannot open png file: " << file << "\n";
        return nullptr;
    }
    //Set PNG Format
    png.format = PNG_FORMAT_RGBA;
    if(png.width == 0 || png.height == 0)
    {
        std::cerr << "Invalid png: " << file << "\n";
        png_image_free(&png);
        return nullptr;
    }
    if(rect)
    {
        rect->h = png.height;
        rect->w = png.width;
    }
    //Create surface
    auto surface = SDL_CreateRGBSurface(0, png.width, png.height, 32, rmask, gmask, bmask, amask);

    if(surface)
    {
        //Read png data to surface
        png_image_finish_read(&png, NULL, surface->pixels, 0, NULL);
        png_image_free(&png);
        //Convert surface to texture
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }
    else
    {
        std::cerr << "Error creating surface for: " << file << "\n";
        exit(1);
    }

    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    return texture;
}

SDL_Texture * WriteText(const std::string & text, int fontSize, SDL_Renderer* renderer, int & textureWidth, int & textureHeight, const int color)
{
    if(text.size()==0)
        return nullptr;
    textureWidth = fontSize*text.size();
    textureHeight = fontSize;
    auto surface = SDL_CreateRGBSurface(0, 8*text.size(), 8, 32, rmask, gmask, bmask, amask);
    uint* pixels = reinterpret_cast<uint*>(surface->pixels);
    int pitch = 8*text.size();
    for(unsigned int i = 0; i < text.size(); ++i)
    {
        auto bitmap = font8x8_basic[text[i]];
        int offset = i*8;
        for(int y = 0; y < 8; ++y)
        {
            for(int x = 0; x < 8; ++x)
            {
                pixels[x+offset+(y*pitch)] = (bitmap[y] & 1 << x) ? color : 0;
            }
        }
    }
    auto texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    SDL_FreeSurface(surface);
    return texture;
}
