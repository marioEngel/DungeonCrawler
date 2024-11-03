#include "MovementPlayer_decision.h"
#include "../../ECS/Coordinator.h"
#include "../../System/KeyInput/KeyboardInput.h"
#include "../../Component/InputComp.h"
#include "../../Component/MoveDecisionComp.h"
#include "../Dungeon/Dungeon.h"
#include "../../Component/MovementComp.h"

extern Coordinator gCoordinator;
extern KeyboardInput gKeyboardInput;

void MovementPlayer_decisionSystem::update()
{
	for (auto& const entity : mEntities)
	{
		auto& input = gCoordinator.GetComponent<InputKeys>(entity);
		auto& decision = gCoordinator.GetComponent<MoveDecision>(entity);
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

//void MovementDecisionSystem::checkPossibility()
//{
//	for (auto& const entity : mEntities)
//	{
//		auto& decision = gCoordinator.GetComponent<MoveDecision>(entity);
//		auto& matrixPos = gCoordinator.GetComponent<MatrixPosition>(entity);
//
//		Vector2D<int> tmpDirection{};
//		switch (decision.direction)
//		{
//		case eMoveDirection::DOWNWARDS:
//			tmpDirection = gBasicVector.basicDownward;
//			break;
//		case eMoveDirection::UPWARDS:
//			tmpDirection = gBasicVector.basicUpward;
//			break;
//		case eMoveDirection::RIGHT:
//			tmpDirection = gBasicVector.basicRight;
//			break;
//		case eMoveDirection::LEFT:
//			tmpDirection = gBasicVector.basicLeft;
//			break;
//		default:
//			break;
//		}
//
//		decision.possible = gDungeonSystem.checkCollision(matrixPos.matrixPosition + tmpDirection);
//	}
//}

/*bool MovementDecisionSystem::printStuff()
{
	if (gKeyboardInput.getButtonState(SDL_SCANCODE_L) == eButtonState::RELEASED)
	{
		for (auto& const entity : mEntities)
		{
			auto& decision = gCoordinator.GetComponent<MoveDecision>(entity);

			std::cout << "decision direction: " << decision.direction << '\n' <<
				"decision input cooldown: " << decision.inputCooldown << '\n' <<
				"decision possible: " << decision.possible << '\n';
		}

		return false;
	}
	
	return false;
}*/