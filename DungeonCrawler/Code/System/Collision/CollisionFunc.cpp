#include "CollisionFunc.h"

bool check_RectVsRect(const SDL_FRect& rect1, const SDL_FRect& rect2)
{
	bool rtnBool = false;

	if ((rect1.x + rect1.w) > rect2.x &&
		rect1.x < (rect2.x + rect2.w) &&
		(rect1.y + rect1.h) > rect2.y &&
		rect1.y < (rect2.y + rect2.h))
	{
		rtnBool = true;
	}

	return rtnBool;
}

bool check_RectVsRect(const SDL_Rect& rect1, const SDL_Rect& rect2)
{
	bool rtnBool = false;

	if ((rect1.x + rect1.w) > rect2.x &&
		rect1.x < (rect2.x + rect2.w) &&
		(rect1.y + rect1.h) > rect2.y &&
		rect1.y < (rect2.y + rect2.h))
	{
		rtnBool = true;
	}

	return rtnBool;
}

bool check_RectVsPoint(const SDL_FRect& rect, const Vector2D<float>& vec)
{
	bool rtnBool = false;

	if (vec[0] >= rect.x &&
		vec[0] <= (rect.x + rect.w) &&
		vec[1] >= rect.y &&
		vec[1] <= (rect.y + rect.h))
	{
		rtnBool = true;
	}

	return rtnBool;
}
