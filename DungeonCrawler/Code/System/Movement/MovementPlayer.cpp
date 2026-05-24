#include "MovementPlayer.h"
#include "../../Component/Comp_DirectionDecision.h"
#include "../../ECS/Coordinator.h"
#include "../../Component/Comp_Movement.h"
#include <SDL3/SDL.h>
#include "../../Component/Comp_Position.h"

extern Coordinator gCoordinator;
extern float gDeltaT;

void MovementPlayerSystem::update()
{
	for (auto& const entity : mEntities)
	{
		auto& decision = gCoordinator.GetComponent<DirectionDecision>(entity);
		auto& movement = gCoordinator.GetComponent<Movement>(entity);
		auto& normalPos = gCoordinator.GetComponent<Position>(entity);

		// ??? print pos
		// std::cout << "Position: " << normalPos.pos << '\n';

		normalPos.pos = normalPos.pos + decision.direction * movement.speed * gDeltaT;
	}
}