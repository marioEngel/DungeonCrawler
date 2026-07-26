#pragma once
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include "../../../Game.h"
#include <SDL3_ttf/SDL_ttf.h>

SDL_Texture* LoadTexture(const char* path, SDL_Renderer* tmpRenderer = Game::renderer);
SDL_Texture* LoadTextureText(TTF_Font* font, std::string text, SDL_Renderer* tmpRenderer, SDL_Color color = SDL_Color{ 255, 255, 255, 255 });

void drawAndClear(SDL_Texture* rendTex1, SDL_Texture* rendTex2, SDL_Texture* rendTex3, SDL_Texture* rendTex4, bool lightOn = true);

//void LoadTexture(const char* path, SDL_Rect& originRec, SDL_Rect& destRect, SDL_Renderer* tmpRenderer = Game::renderer);