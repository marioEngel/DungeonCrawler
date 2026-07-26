#pragma once
#include "../../../ECS/System.h"

class SysDisplayFPS : public Systems
{
public:
	void update(float deltaT);
};