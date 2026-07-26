#include "MovementObject.h"
#include "../../ECS/Coordinator.h"
#include "../../Component/Comp_Movement.h"
#include <SDL3/SDL.h>
#include "../../Component/Comp_Position.h"
#include "../../Component/Comp_FaceDirection.h"

extern Coordinator gCoordinator;
extern float gDeltaT;

void SysMovementObject::update()
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