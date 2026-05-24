#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include <functional>

struct ColorValues
{
	int r;
	int g;
	int b;
};

// simple Texture
struct TextureLight
{
	std::tuple<int, int, float, float, float> functionParams;
	std::function<SDL_Texture* (SDL_Renderer*, int, int, float, float, float)> function;

	int width = 720;
	int height = 720;
	float angle = 0.0f;
	ColorValues colorRGB{ 255, 255, 255 };

	SDL_Texture* SDL = NULL;
};

