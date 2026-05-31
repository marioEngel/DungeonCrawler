#pragma once

enum eRectangleSide
{
	TOP,
	BOT,
	LEFT,
	RIGHT
};

enum eRectCorner
{
	TopRight,
	TopLeft,
	BotRight,
	BotLeft
};

//eRectangleSide side_RectVsLine(SDL_FRect& rect, Line& line, Vector2D<float>& circleCentre);
eRectangleSide convert_int_eRectangleSide(int& input);
void printRectangleSide(eRectangleSide& tmpSide);
Line create_Line(eRectangleSide side, SDL_FRect& rect);
Vector2D<float> rtnCorner(SDL_FRect& rect, eRectCorner rtnCorner);

bool check_RectVsCirc(SDL_FRect& rect, float& circ_radius, Vector2D<float> pos);
bool check_CircleVsCircle(float& radius1, Vector2D<float>& position1, float& radius2, Vector2D<float>& position2);
bool check_PointVsRectEdge(SDL_FRect& rect, Vector2D<float>& point);
//// if pos true else false
bool check_determinante(float& circ_radius, Vector2D<float>& pos, Line& line);
bool check_firstPointCloser(Vector2D<float>& point1, Vector2D<float>& point2, Vector2D<float>& referencePoint);

// in Collision_calc.cpp
// | | | | |
// v v v v v

CollisionDataSub calc_RectVsRect_distVec(SDL_FRect& rect1, SDL_FRect& rect2, Vector2D<float> rect1_direction, Vector2D<float> rect2_direction);
CollisionDataSub calc_CircVsCirc_distVec(float& circ1_radius, SDL_FRect& circ1_AABB, float& circ2_radius, SDL_FRect& circ2_AABB);
CollisionDataSub calc_RectVsCirc_distVec(SDL_FRect& rect, float& circ_radius, SDL_FRect& circ_AABB, Vector2D<float> rect_dir, Vector2D<float> circ_dir);

// in Collision_crossPoint.cpp
// | | | | |
// v v v v v

Vector2D<float> crossPoint_LineVsLine(Line& line1, Line& line2);
std::vector<Vector2D<float>> crossPoints_RectVsLine(SDL_FRect& rect, Line& line);
std::vector<Vector2D<float>> crossPoints_RectOpenVsLine(SDL_FRect& rect, Line& line);
std::vector<Vector2D<float>> crossPoints_CircVsLine(float& circ_radius, Vector2D<float>& pos, Line& line);
std::vector<Vector2D<float>> crossPoints_RectVsCirc(SDL_FRect& rect, float& circ_radius, Vector2D<float>& pos);