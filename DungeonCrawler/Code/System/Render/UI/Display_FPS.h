#pragma once
#include "../../../ECS/System.h"

class System_Display_FPS : public Systems
{
public:
	void update(float deltaT);
};