#include "MovementObject.h"
#include "../../ECS/Coordinator.h"
#include "../../Component/MovementComp.h"
#include <SDL.h>
#include "../../Component/PositionComp.h"
#include "../../Component/FaceDirection.h"

extern Coordinator gCoordinator;
extern float gDeltaT;

void MovementObjectSystem::update()
{
	for (auto& const entity : mEntities)
	{
		auto& movement = gCoordinator.GetComponent<Movement>(entity);
		auto& normalPos = gCoordinator.GetComponent<Position>(entity);
		auto& direction = gCoordinator.GetComponent<FaceDirection>(entity);

		normalPos.pos = normalPos.pos + Vector2D<float>(
			std::cos(direction.angle) * movement.speed * gDeltaT, 
			-std::sin(direction.angle) * movement.speed * gDeltaT
		);
	}
}