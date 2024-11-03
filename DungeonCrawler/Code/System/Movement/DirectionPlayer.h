#pragma once
#include "../../ECS/System.h"

class DirectionPlayerSystem : public Systems
{
public:
	void makeMarker();
	void update();
};