#include "MovementPlayerDecision.h"
#include "../../ECS/Coordinator.h"
#include "../../System/KeyInput/KeyboardInput.h"
#include "../../Component/Comp_InputKeys.h"
#include "../../Component/Comp_DirectionDecision.h"
#include "../../Component/Comp_Movement.h"

extern Coordinator gCoordinator;
extern KeyboardInput gKeyboardInput;

void SysMovementPlayerDecision::update()
{
	for (const auto& entity : mEntities)
	{
		auto& input = gCoordinator.GetComponent<InputKeys>(entity);
		auto& decision = gCoordinator.GetComponent<DirectionDecision>(entity);
		auto& movement = gCoordinator.GetComponent<Movement>(entity);

		decision.direction = Vector2D<float>{ 0.0, 0.0 };

		if (gKeyboardInput.getButtonState(input.MovementUp) ||
			gKeyboardInput.getButtonState(input.MovementUp) == eButtonState::HELD)
		{
			decision.direction = decision.direction + Vector2D<float>{0.0, -1.0};
		}
		else if (gKeyboardInput.getButtonState(input.MovementDown) ||
			gKeyboardInput.getButtonState(input.MovementDown) == eButtonState::HELD)
		{
			decision.direction = decision.direction + Vector2D<float>{0.0, 1.0};
		}

		if (gKeyboardInput.getButtonState(input.MovementLeft) ||
			gKeyboardInput.getButtonState(input.MovementLeft) == eButtonState::HELD)
		{
			decision.direction = decision.direction + Vector2D<float>{-1.0, 0.0};
		}
		else if (gKeyboardInput.getButtonState(input.MovementRight) ||
			gKeyboardInput.getButtonState(input.MovementRight) == eButtonState::HELD)
		{
			decision.direction = decision.direction + Vector2D<float>{1.0, 0.0};
		}

		decision.direction.normalize();
		movement.direction = decision.direction;
		//std::cout << decision.direction << std::endl;
	}
}
