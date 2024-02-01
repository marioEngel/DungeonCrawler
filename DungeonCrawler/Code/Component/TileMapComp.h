#pragma once
#include "../Math/Matrix.h"
#include <SDL.h>

struct TileMap
{
	Matrix matrix;							// simple 2D matrix 
	std::vector<const char*> paths;			// vector with every path for all tiles
	std::vector<SDL_Texture*> textures;		// vector with textures
};