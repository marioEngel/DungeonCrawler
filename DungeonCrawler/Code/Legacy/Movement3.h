#pragma once
#include "../ECS/System.h"
#include "../Math/Matrix.h"

class MovementSystem : public Systems
{
public:
	void setCollision();
	void update();

private:
	Matrix collisonMatrix;
};