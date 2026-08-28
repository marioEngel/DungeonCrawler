#pragma once
#include <vector>
#include <SDL3/SDL.h>
#include "../../../Math/Matrix.h"

class Floor
{
public:
	Floor();
	~Floor();

	void generate();
	void add_rooms();
	void connect_rooms();
	void set_tiles(SDL_Rect& room);

private:
	int numRoomTries;
	int roomExtraSize;
	int windingPercent;

	SDL_Rect floorSize;
	Matrix<int> floorTileMap{ 10, 10 };

	std::vector<SDL_Rect> roomVector;
};

Floor::Floor()
{
}

Floor::~Floor()
{
}