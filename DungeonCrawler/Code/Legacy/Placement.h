#pragma once
#include "../ECS/System.h"

class PlacementSystem : public Systems
{
public:
	void init();
	void placeOnTile();
};