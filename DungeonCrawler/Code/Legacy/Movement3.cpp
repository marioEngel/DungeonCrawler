#include "Movement.h"
#include "../Component/PositionComp.h"
#include "../Component/InputComp.h"
#include "../ECS/Coordinator.h"
#include "../System/KeyInput/KeyboardInput.h"
#include "../Component/MovementComp.h"
#include "../Component/MatrixPositionComp.h"
#include "Dungeon/Dungeon.h"

// ============== work ==========================

extern Coordinator gCoordinator;
extern KeyboardInput gKeyboardInput;
extern float gDeltaT;
extern std::shared_ptr<DungeonSystem> dungeonSystem;

void MovementSystem::setCollision()
{
	collisonMatrix = dungeonSystem->return_collisionMap();
}

void MovementSystem::update()
{
	for (auto& const entity : mEntities)
	{
		auto& position = gCoordinator.GetComponent<Position>(entity);
		auto& input = gCoordinator.GetComponent<InputKeys>(entity);
		auto& movementData = gCoordinator.GetComponent<Movement>(entity);
		auto& matrixPos = gCoordinator.GetComponent<MatrixPosition>(entity);

		/*  -check if coldown is up if yes register movement input
			-if input is registered set flag to direction and move sprite steadily to position	
			-wait a short period after reaching destination
			-check if collision in way
		 */
		if (!movementData.onPause) {
			if (!movementData.onColdown)
			{
				if (gKeyboardInput.getButtonState(input.MovementUp) ||
					gKeyboardInput.getButtonState(input.MovementUp) == eButtonState::HELD)
				{
					if (collisonMatrix[matrixPos.row - 1][matrixPos.collum] == 1)
					{
						std::cout << "Hit Wall" << std::endl;
					}
					else
					{
						matrixPos.row -= 1;
						movementData.direction = 1;
						movementData.startTime = SDL_GetTicks();
						movementData.onColdown = true;
					}
				}
				if (gKeyboardInput.getButtonState(input.MovementDown) ||
					gKeyboardInput.getButtonState(input.MovementDown) == eButtonState::HELD)
				{
					if (collisonMatrix[matrixPos.row + 1][matrixPos.collum] == 1)
					{
						std::cout << "Hit Wall" << std::endl;
					}
					else
					{
						matrixPos.row += 1;
						movementData.direction = 2;
						movementData.startTime = SDL_GetTicks();
						movementData.onColdown = true;
					}
				}
				if (gKeyboardInput.getButtonState(input.MovementLeft) ||
					gKeyboardInput.getButtonState(input.MovementLeft) == eButtonState::HELD)
				{
					if (collisonMatrix[matrixPos.row][matrixPos.collum - 1] == 1)
					{
						std::cout << "Hit Wall" << std::endl;
					}
					else
					{
						matrixPos.collum -= 1;
						movementData.direction = 3;
						movementData.startTime = SDL_GetTicks();
						movementData.onColdown = true;
					}
				}
				if (gKeyboardInput.getButtonState(input.MovementRight) ||
					gKeyboardInput.getButtonState(input.MovementRight) == eButtonState::HELD)
				{
					if (collisonMatrix[matrixPos.row][matrixPos.collum + 1] == 1)
					{
						std::cout << "Hit Wall" << std::endl;
					}
					else
					{
						matrixPos.collum += 1;
						movementData.direction = 4;
						movementData.startTime = SDL_GetTicks();
						movementData.onColdown = true;
					}
				}
			}
			if (movementData.onColdown)
			{
				movementData.endTime = SDL_GetTicks();
				if (movementData.endTime - movementData.startTime - movementData.coldown > 0)
				{
					//std::cout << movementData.endTime - movementData.startTime - movementData.coldown << std::endl;
					movementData.onColdown = false;
					// round to exact values
					position.xPos = float(std::round(position.xPos));
					position.yPos = float(std::round(position.yPos));
					movementData.onPause = true;
					movementData.startTime = SDL_GetTicks();
				}
				else
				{
					switch (movementData.direction)
					{
					case 1:
						position.yPos += -movementData.speed * gDeltaT;
						break;
					case 2:
						position.yPos += movementData.speed * gDeltaT;
						break;
					case 3:
						position.xPos += -movementData.speed * gDeltaT;
						break;
					case 4:
						position.xPos += movementData.speed * gDeltaT;
						break;
					default:
						break;
					}
				}
			}
		}
		if (movementData.onPause)
		{
			movementData.endTime = SDL_GetTicks();
			if (movementData.endTime - movementData.startTime - movementData.pause > 0)
			{
				movementData.onPause = false;
			}
		}
		//std::cout << "PositionX: " << position.xPos << " | PositionY: " << position.yPos << std::endl;
	}
}