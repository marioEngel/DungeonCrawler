#pragma once
#include "../../../ECS/System.h"
#include "Floor.h"	

class SysTower : public Systems
{
public:
	void init();
	void syncPlayer();

private:
	std::vector<Entity> floorEntityList;
	std::vector<Floor> floorList;
	int currentEntity = -1;
};