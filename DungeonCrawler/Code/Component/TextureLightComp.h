#pragma once
#include <SDL.h>
#include <vector>

// simple Texture
struct TextureLight
{
	const char* path;
	int textureHeight;
	int textureWidth;
	int scale;
	double angle;

	SDL_Texture* textureSDL = NULL;
};