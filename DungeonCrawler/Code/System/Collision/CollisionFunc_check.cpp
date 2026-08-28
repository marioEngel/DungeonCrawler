#include "CollisionFunc.h"

bool check_RectVsRect(SDL_FRect& rect1, SDL_FRect& rect2)
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

bool check_RectVsRect(SDL_Rect& rect1, SDL_Rect& rect2)
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

bool check_RectVsPoint(SDL_FRect& rect, Vector2D<float>& vec)
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

// more advanced collision test for geometries
bool check_Geometry_AABB(Geometry& geom1, Vector2D<float>& pos1, Geometry& geom2, Vector2D<float>& pos2)
{
	SDL_FRect geom1_AABB = geom1.return_AABB(pos1);
	SDL_FRect geom2_AABB = geom2.return_AABB(pos2);

	return check_RectVsRect(geom1_AABB, geom2_AABB);
}

