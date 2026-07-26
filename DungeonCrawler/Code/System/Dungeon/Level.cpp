#include "Level.h"
#include "../../Component/Comp_Level.h"
#include "../../ECS/Coordinator.h"

extern Coordinator gCoordinator;

void SysLevel::init()
{
	for (auto& const entity : mEntities)
	{
		auto& level = gCoordinator.GetComponent<Level>(entity);

		level.dungeon->init();
	}
}

void SysLevel::update(Entity player)
{

}