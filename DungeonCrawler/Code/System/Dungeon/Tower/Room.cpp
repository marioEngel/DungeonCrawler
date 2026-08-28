#include "Room.h"

Room::Room(int x, int y, int width, int height)
{
	geom = SDL_Rect{ x, y, width, height };
}

Room::~Room()
{
}

void Room::setTileType()
{
	tileType = eTileType::FLOOR;
}