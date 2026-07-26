#pragma once
#include <SDL3/SDL.h>
#include "Geometry/Circle.h"
#include "Geometry/Geometry.h"
#include "Geometry/Line.h"
#include "../../Math/Vector2D.h"
#include "../../Math/GeneralMathStuff.h"
#include "../../ECS/ECS.h"
#include "../../Component/Comp_Position.h"
#include "../../Component/Comp_Movement.h"

struct Contact {
	Vector2D<float> normal;
	float penetration;
	bool valid;
};

struct CollisionData {
	std::array<Entity, 2> collisionPair;
	Contact contact;
};

// in CollisionFunc_check.cpp
// | | | | |
// v v v v v

// simple collision test for geometries
bool check_RectVsRect(SDL_FRect& rect1, SDL_FRect& rect2);
bool check_RectVsPoint(SDL_FRect& rect, Vector2D<float>& positon);
// more advanced collision test for geometries
bool check_Geometry_AABB(Geometry& geom1, Vector2D<float>& pos1, Geometry& geom2, Vector2D<float>& pos2);

// in CollisionFunc_arbitrary.cpp
// | | | | |
// v v v v v

Contact check_Geometry_arbitrary(
	Geometry& geom1, Vector2D<float>& pos1, Movement& mov1,
	Geometry& geom2, Vector2D<float>& pos2, Movement& mov2
);

// in CollisionFunc_misc.cpp
// | | | | |
// v v v v v

void print_Rect_data(SDL_FRect& rect);
SDL_FRect create_Rect(Vector2D<float>& cornerPoint, std::array<float, 2>& remainderData);
Vector2D<float> safeNormalize(Vector2D<float> vec1, Vector2D<float> fallback = { 0.0f, 1.0f });
Vector2D<float> calc_Rect_center(SDL_FRect& rect);
bool compareRect(const SDL_Rect& first, const SDL_Rect& second);

// in CollisionFunc_contact.cpp
// | | | | |
// v v v v v

Contact check_RectVsRect_contact(SDL_FRect& rect1, SDL_FRect& rect2);
Contact check_CircVsCirc_contact(Vector2D<float>& pos1, float radius1, Vector2D<float>& pos2, float radius2);
Contact check_CircleVsRect_contact(Vector2D<float>& circlePos, float radius, SDL_FRect& rect, bool flipPenetration = false);