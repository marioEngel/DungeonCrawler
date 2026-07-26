#pragma once
#include "../Math/Matrix.h"
#include <SDL3/SDL.h>
#include "../Math/Vector2D.h"

struct TileMap
{
	Matrix matrix;							// simple 2D matrix 
	std::vector<const char*> paths;			// vector with every path for all tiles
	std::vector<SDL_Texture*> textures;		// vector with textures
	Vector2D<int> dimension { 0, 0 };
	SDL_Texture* tileMapTexture = nullptr;
};