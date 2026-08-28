#include "Floor.h"
#include "../../../Misc/RNG.h"
#include <random>
#include "Room.h"
#include <SDL3/SDL.h>
#include "../../../System/Collision/Collision.h"

void Floor::generate()
{

	//matrixCreate_Element(floorTileMap, floorSize, eTileType::WALL);


}

void Floor::add_rooms()
{
	for (size_t i = 0; i < numRoomTries; i++)
	{
		std::random_device rd;
		RNG rng(rd());
		int size = rng.range(1, 3 + roomExtraSize) * 2 + 1;
		int rectangularity = rng.range(0, 1 + size / 2) * 2;
		int width = size;
		int heigt = size;
		if (rng.oneIn(2)) 
		{
			width += rectangularity;
		}
		else
		{
			heigt += rectangularity;
		}

		int x = rng.range((floorSize.w - width) / 2) * 2 + 1;
		int y = rng.range((floorSize.h - heigt) / 2) * 2 + 1;

		SDL_Rect tmpRoom{ x, y, width, heigt };

		bool overlaps = false;
		for (SDL_Rect room : roomVector)
		{
			overlaps = check_RectVsRect(room, tmpRoom);
			if (overlaps)
			{
				break;
			}
		}
		
		if (!overlaps)
		{
			set_tiles(tmpRoom);
			roomVector.emplace_back(tmpRoom);
		}
	}
}

void Floor::set_tiles(SDL_Rect& room)
{
	for (int row = room.y; row < room.y + room.w; row++)
	{
		for (int col = room.x; col < room.x + room.h; col++)
		{
			floorTileMap(row, col) = eTileType::FLOOR;
		}
	}
}

void Floor::connect_rooms() 
{
	for (int row = floorSize.y + 1; row < floorSize.y + floorSize.w - 1; row++)
	{
		for (int col = floorSize.x + 1; col < floorSize.x + floorSize.h - 1; col++)
		{
			Vector2D<int> pos{ row, col };
			floorTileMap[pos];


		}
	}
}
