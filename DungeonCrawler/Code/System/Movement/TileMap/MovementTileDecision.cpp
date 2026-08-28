#include "MovementTileDecision.h"
#include "../../../ECS/Coordinator.h"
#include "../../KeyInput/KeyboardInput.h"
#include "../../../Component/Comp_InputKeys.h"
#include "../../../Component/Comp_DirectionDecision.h"
#include "../../../Component/Comp_MovementTile.h"
#include "../../../Component/Comp_Position.h"


extern Coordinator gCoordinator;
extern KeyboardInput gKeyboardInput;

void SysMovementTileDecision::update()
{
	for (const auto& entity : mEntities)
	{
		auto& input = gCoordinator.GetComponent<InputKeys>(entity);
		auto& decision = gCoordinator.GetComponent<DirectionDecision>(entity);
		auto& movementTile = gCoordinator.GetComponent<MovementTile>(entity);
		auto& position = gCoordinator.GetComponent<Position>(entity);

		if (movementTile.onCooldown || movementTile.onPause)
			continue; // already moving/pausing — ignore input until the tile step finishes


		Vector2D<float> dir{ 0.0f, 0.0f };
		if (gKeyboardInput.getButtonState(input.MovementUp) == eButtonState::HELD)
			dir = { 0.0f, -1.0f };
		else if (gKeyboardInput.getButtonState(input.MovementDown) == eButtonState::HELD)
			dir = { 0.0f, 1.0f };
		else if (gKeyboardInput.getButtonState(input.MovementLeft) == eButtonState::HELD)
			dir = { -1.0f, 0.0f };
		else if (gKeyboardInput.getButtonState(input.MovementRight) == eButtonState::HELD)
			dir = { 1.0f, 0.0f };

		if (dir[0] != 0.0f || dir[1] != 0.0f)
		{
			decision.direction = dir;
			movementTile.direction = dir;
			movementTile.positionStart = position.pos;
			movementTile.positionEnd = movementTile.positionStart + dir * movementTile.tileSize;
			movementTile.onCooldown = true;
			movementTile.timeStart = SDL_GetTicks();
		}
	}
}