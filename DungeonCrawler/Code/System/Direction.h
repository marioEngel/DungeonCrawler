#pragma once
#include "../ECS/System.h"

class DirectionSystem : public Systems
{
public:
	void makeMarker();
	void update();
};