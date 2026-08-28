#include "MovementTile.h"
#include "../../../ECS/Coordinator.h"
#include "../../../Component/Comp_MovementTile.h"
#include "../../../Component/Comp_DirectionDecision.h"
#include "../../../Component/Comp_Position.h"
#include <SDL3/SDL.h>
#include "../../../Misc/MiscFunctions.h"

extern Coordinator gCoordinator;

void SysMovementTile::update(float delta)
{
	for (const auto& entity : mEntities)
	{
		auto& movementTile = gCoordinator.GetComponent<MovementTile>(entity);
		auto& normalPos = gCoordinator.GetComponent<Position>(entity);

		if (movementTile.onCooldown)
		{
			Uint32 now = SDL_GetTicks();
			float t = float(now - movementTile.timeStart) / float(movementTile.cooldown);
			t = clamp(t, 0.0f, 1.0f);

			normalPos.pos = movementTile.positionStart + (movementTile.positionEnd - movementTile.positionStart) * t;

			if (t >= 1.0f)
			{
				normalPos.pos = movementTile.positionEnd; // guarantee exact landing
				movementTile.onCooldown = false;
				movementTile.onPause = true;
				movementTile.timePause = now;
			}
		}
		else if (movementTile.onPause)
		{
			if (SDL_GetTicks() - movementTile.timePause > movementTile.pause)
			{
				movementTile.onPause = false;
			}
		}
	}
}