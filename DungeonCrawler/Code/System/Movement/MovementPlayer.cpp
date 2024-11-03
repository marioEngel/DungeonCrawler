#include "MovementPlayer.h"
#include "../../Component/MoveDecisionComp.h"
#include "../../ECS/Coordinator.h"
#include "../../Component/MovementComp.h"
#include <SDL.h>
#include "../../Component/PositionComp.h"

extern Coordinator gCoordinator;
extern float gDeltaT;

void MovementPlayerSystem::update()
{
	for (auto& const entity : mEntities)
	{
		auto& decision = gCoordinator.GetComponent<MoveDecision>(entity);
		auto& movement = gCoordinator.GetComponent<Movement>(entity);
		auto& normalPos = gCoordinator.GetComponent<Position>(entity);

		//int x;
		//int y;
		//SDL_GetMouseState(&x, &y);
		//std::cout << "x|y : " << x << "|" << y << std::endl;
		normalPos.pos = normalPos.pos + decision.direction * movement.speed * gDeltaT;
	}
}