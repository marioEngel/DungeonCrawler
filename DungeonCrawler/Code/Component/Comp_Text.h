#pragma once
#include <SDL3/SDL.h>
#include <string>
#include <SDL3_ttf/SDL_ttf.h>
#include "Comp_TextureLight.h"

struct Text
{
	std::string text;
	int width;
	int height;
	ColorValues textColor{ 255, 255, 255 };

	bool reloadTexture = false;
	TTF_Font* font = NULL;
	SDL_Texture* SDL = NULL;
};
