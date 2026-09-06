#pragma once
#include "../../../Math/Vector2D.h"


struct Compass
{
	Vector2D<int> NORTH{ 0, 1 };
	Vector2D<int> SOUTH { 0, -1 };
	Vector2D<int> EAST { 1, 0 };
	Vector2D<int> WEST { -1, 0 };

	std::array<Vector2D<int>, 4> directions { NORTH, SOUTH, EAST, WEST };
};
