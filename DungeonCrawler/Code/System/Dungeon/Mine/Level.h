#pragma once
#include "../../../ECS/System.h"

class SysLevel : public Systems 
{
public:
	void init();
	void update(Entity playerin);

};
