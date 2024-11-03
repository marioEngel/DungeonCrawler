#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include "../../Game.h"

SDL_Texture* LoadTexture(const char* path, SDL_Renderer* tmpRenderer = Game::renderer);
SDL_Texture* LoadTexture2(const char* path, SDL_Renderer* tmpRenderer = Game::renderer);

void drawAndClear(SDL_Texture*  rendTex1, SDL_Texture* rendTex2, SDL_Texture* rendTex3);

//void LoadTexture(const char* path, SDL_Rect& originRec, SDL_Rect& destRect, SDL_Renderer* tmpRenderer = Game::renderer);