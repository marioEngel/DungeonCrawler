#pragma once
#include <SDL3/SDL.h>
#include "TileType.h"

class Room
{
public:
	Room(int inX, int inY, int inWidth, int inHeight);
	~Room();
	void setTileType();

	SDL_Rect geom;
private:
	eTileType tileType;
};
