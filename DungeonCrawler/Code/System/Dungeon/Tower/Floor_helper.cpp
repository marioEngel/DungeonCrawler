#pragma once
#include "Floor.h"

SDL_Rect addRoomBorder(const SDL_Rect& rect)
{
	return SDL_Rect{
		rect.x - 1,
		rect.y - 1,
		rect.w + 2,
		rect.h + 2
	};
}

SDL_Rect addRoomBorder_doubled(const SDL_Rect& rect)
{
	return SDL_Rect{
		rect.x - 2,
		rect.y - 2,
		rect.w + 4,
		rect.h + 4
	};
}

std::vector<Vector2D<int>> getPosListFromRect(const SDL_Rect& rect)
{
	std::vector<Vector2D<int>> rtnList{};

	for (int row = rect.y; row < rect.y + rect.h; row++)
	{
		for (int col = rect.x; col < rect.x + rect.w; col++)
		{
			rtnList.push_back(Vector2D<int>(row, col));
		}
	}

	return rtnList;
}

float roomDistance(const SDL_Rect& room_a, const SDL_Rect& room_b)
{
	float centerAx = room_a.x + room_a.w / 2.0f;
	float centerAy = room_a.y + room_a.h / 2.0f;
	float centerBx = room_b.x + room_b.w / 2.0f;
	float centerBy = room_b.y + room_b.y / 2.0f;

	float dx = centerBx - centerAx;
	float dy = centerBy - centerAy;
	return std::sqrt(dx * dx + dy * dy);
}