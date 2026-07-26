#pragma once
#include "../../ECS/System.h"

class SysDirectionPlayer : public Systems
{
public:
	void makeMarker();
	void update();
};