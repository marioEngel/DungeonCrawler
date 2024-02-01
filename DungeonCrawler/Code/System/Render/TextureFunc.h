#pragma once
#include <SDL.h>
#include <SDL_image.h>


SDL_Texture* LoadTexture(const char* path);

void drawAndClear();

void LoadTexture(const char* path, SDL_Rect& originRec, SDL_Rect& destRect);