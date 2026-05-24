#pragma once
#include <SDL3/SDL.h>
#include <string>
#include <SDL3_ttf/SDL_ttf.h>

struct Text
{
	std::string text;
	int width;
	int height;

	TTF_Font* font = NULL;
	SDL_Texture* SDL = NULL;
};
