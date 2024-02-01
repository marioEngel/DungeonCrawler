#pragma once
#include "../Math/Vector2D.h"

/* (x|y)
 * (0|0) top left  corner
 * (n|n) bot right corner
 * (n|0) top right corner
 * (0|n) bot left  corner
 */
struct MatrixPosition
{
	Vector2D<int> matrixPosition;
};