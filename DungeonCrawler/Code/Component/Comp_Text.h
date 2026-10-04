#pragma once
#include <SDL3/SDL.h>
#include <string>
#include <SDL3_ttf/SDL_ttf.h>
#include "Comp_TextureLight.h"

struct Text {
    std::string text;
    int width;
    int height;
    ColorValues textColor{.r = 255, .g = 255, .b = 255};

    bool reloadTexture = false;
    TTF_Font *font = nullptr;
    SDL_Texture *SDL = nullptr;
};
