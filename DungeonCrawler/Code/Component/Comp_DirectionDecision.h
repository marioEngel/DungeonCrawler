#pragma once
#include "../Math/Vector2D.h"

//enum eMoveDirection
//{
//	IDLE,
//	UPWARDS,
//	DOWNWARDS,
//	LEFT,
//	RIGHT
//};
//
//struct basicVector
//{
//	Vector2D<int> basicUpward;
//	Vector2D<int> basicDownward;
//	Vector2D<int> basicRight;
//	Vector2D<int> basicLeft;
//};

struct DirectionDecision
{
	Vector2D<float> direction{ 0, 0 };
};