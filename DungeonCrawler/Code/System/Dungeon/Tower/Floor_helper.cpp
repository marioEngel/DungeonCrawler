#include "Floor.h"

SDL_Rect addRoomBorder(const SDL_Rect& rect)
{
	return SDL_Rect{
		.x = rect.x - 1,
		.y = rect.y - 1,
		.w = rect.w + 2,
		.h = rect.h + 2
	};
}

SDL_Rect addRoomBorder_doubled(const SDL_Rect& rect)
{
	return SDL_Rect{
		.x = rect.x - 2,
		.y = rect.y - 2,
		.w = rect.w + 4,
		.h = rect.h + 4
	};
}

SDL_Rect shrinkRoomByOne(const SDL_Rect& rect)
{
	return SDL_Rect{
		.x = rect.x + 1,
		.y = rect.y + 1,
		.w = rect.w - 2,
		.h = rect.h - 2
	};
}

std::vector<Vector2D<int>> getPosListFromRect(const SDL_Rect& rect)
{
	std::vector<Vector2D<int>> rtnList{};

	for (int row = rect.y; row < rect.y + rect.h; row++)
	{
		for (int col = rect.x; col < rect.x + rect.w; col++)
		{
			rtnList.emplace_back(row, col);
		}
	}

	return rtnList;
}

float roomDistance(const SDL_Rect& room_a, const SDL_Rect& room_b)
{
	const float centerAx = static_cast<float>(room_a.x) + static_cast<float>(room_a.w) / 2.0f;
	const float centerAy = static_cast<float>(room_a.y) + static_cast<float>(room_a.h) / 2.0f;
	const float centerBx = static_cast<float>(room_b.x) + static_cast<float>(room_b.w) / 2.0f;
	const float centerBy = static_cast<float>(room_b.y) + static_cast<float>(room_b.h) / 2.0f;

	const float dx = centerBx - centerAx;
	const float dy = centerBy - centerAy;
	return std::sqrt(dx * dx + dy * dy);
}
