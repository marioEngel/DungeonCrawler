#pragma once
#include "../../Math/Vector2D.h"
#include <SDL.h>
#include "../../Math/Vector2D.h"
#include "Geometry/Geometry.h"
#include "../../ECS/ECS.h"
#include "../../Math/GeneralMathStuff.h"
#include "Geometry/Line.h"
#include "Geometry/Circle.h"
#include "../../Component/PositionComp.h"
#include "../../Component/MovementComp.h"

struct CollisionData
{
	std::array<Entity, 2> collisionPair;
	Vector2D<float> normDirection;
	bool collided;
};

struct CollisionDataSub
{
	Vector2D<float> normDirection;
	bool collided;
};

enum eRectangleSide
{
	TOP,
	BOT,
	LEFT,
	RIGHT,
};

enum eRectCorner
{
	TopRight,
	TopLeft,
	BotRight,
	BotLeft
};

// in Collision_check.cpp
// | | | | |
// v v v v v

// simple collision test for geometries
bool check_RectVsRect(SDL_Rect& rect1, SDL_Rect& rect2);
bool check_RectVsPoint(SDL_Rect& rect, Vector2D<float>& positon);
bool check_RectVsCirc(SDL_Rect& rect, float& circ_radius, Vector2D<float> pos);
bool check_CircleVsCircle(float& radius1, Vector2D<float>& position1, float& radius2, Vector2D<float>& position2);
bool check_PointVsRectEdge(SDL_Rect& rect, Vector2D<float>& point);
// if pos true else false
bool check_determinante(float& circ_radius, Vector2D<float>& pos, Line& line);
bool check_firstPointCloser(Vector2D<float>& point1, Vector2D<float>& point2, Vector2D<float>& referencePoint);

// more advanced collision test for geometries
bool check_Geometry_AABB(Geometry& geom1, Vector2D<float>& pos1, Geometry& geom2, Vector2D<float>& pos2);
CollisionDataSub check_Geometry_arbitrary(
	Geometry& geom1, Vector2D<float>& pos1, Movement& mov1,
	Geometry& geom2, Vector2D<float>& pos2, Movement& mov2
);

// in Collision_calc.cpp
// | | | | |
// v v v v v

CollisionDataSub calc_RectVsRect_distVec(SDL_Rect& rect1, SDL_Rect& rect2, Vector2D<float> rect1_direction);
CollisionDataSub calc_CircVsCirc_distVec(float& circ1_radius, SDL_Rect& circ1_AABB, float& circ2_radius, SDL_Rect& circ2_AABB);
CollisionDataSub calc_RectVsCirc_distVec(SDL_Rect& rect, float& circ_radius, SDL_Rect& circ_AABB, bool playerRect, Vector2D<float> player_direction);

// in Collision_crossPoint.cpp
// | | | | |
// v v v v v

Vector2D<float> crossPoint_LineVsLine(Line& line1, Line& line2);
std::vector<Vector2D<float>> crossPoints_RectVsLine(SDL_Rect& rect, Line& line);
std::vector<Vector2D<float>> crossPoints_RectOpenVsLine(SDL_Rect& rect, Line& line);
std::vector<Vector2D<float>> crossPoints_CircVsLine(float& circ_radius, Vector2D<float>& pos, Line& line);
std::vector<Vector2D<float>> crossPoints_RectVsCirc(SDL_Rect& rect, float& circ_radius, Vector2D<float>& pos);

// in Collision_misc.cpp
// | | | | |
// v v v v v

void print_Rect_data(SDL_Rect& rect);
SDL_Rect create_Rect(Vector2D<float>& cornerPoint, std::array<float, 2>& remainderData);
Vector2D<float> calc_Rect_center(SDL_Rect& rect);
Line create_Line(eRectangleSide side, SDL_Rect& rect);
eRectangleSide side_RectVsLine(SDL_Rect& rect, Line& line, Vector2D<float>& circleCentre);
eRectangleSide convert_int_eRectangleSide(int& input);
void printRectangleSide(eRectangleSide& tmpSide);
Vector2D<float> rtnCorner(SDL_Rect& rect, eRectCorner rtnCorner);
