#include "Movement.h"
#include "../Component/MoveDecisionComp.h"
#include "../ECS/Coordinator.h"
#include "../Component/MovementComp.h"
#include <SDL.h>
#include "../Component/PositionComp.h"
#include "../Component/MatrixPositionComp.h"
#include "../System/Placement.h"

extern Coordinator gCoordinator;
extern float gDeltaT;
extern basicVector gBasicVector;
extern std::shared_ptr<PlacementSystem> placementSystem;

void MovementSystem::update()
{
	/* Movement structure
	 * Press button -> Move slowly to next position (tile of map) -> wait shortly 
	 * 1. Check if move was possible
	 * 2. Check if in pause phase
     * 3. Check if on coldown (in the movement phase)
	 * 
	 */
	for (auto& const entity : mEntities)
	{
		auto& decision = gCoordinator.GetComponent<MoveDecision>(entity);
		auto& movement = gCoordinator.GetComponent<Movement>(entity);
		auto& normalPos = gCoordinator.GetComponent<Position>(entity);
		auto& matrixPos = gCoordinator.GetComponent<MatrixPosition>(entity);

		// can you move to the wanted position?
		if (decision.possible)
		{
			if (!movement.onPause)
			{
				if (!movement.onColdown)
				{
					Vector2D<int> tmpDirect;
					switch (decision.direction)
					{
					case eMoveDirection::UPWARDS:
						tmpDirect = gBasicVector.basicUpward;
						break;
					case eMoveDirection::DOWNWARDS:
						tmpDirect = gBasicVector.basicDownward;
						break;
					case eMoveDirection::LEFT:
						tmpDirect = gBasicVector.basicLeft;
						break;
					case eMoveDirection::RIGHT:
						tmpDirect = gBasicVector.basicRight;
						break;
					default:
						break;
					}

					matrixPos.matrixPosition = matrixPos.matrixPosition + tmpDirect;
					movement.startTime = SDL_GetTicks();
					movement.onColdown = true;
				}
				else
				{
					movement.endTime = SDL_GetTicks();
					if (movement.endTime - movement.startTime - movement.coldown > 0)
					{
						movement.onColdown = false;
						// round to exact values for matrix stuff maybe move later
						placementSystem->placeOnTile();
						//normalPos.pos[0] = float(std::round(normalPos.pos[0]));
						//normalPos.pos[1] = float(std::round(normalPos.pos[1]));
						movement.onPause = true;
						movement.startTime = SDL_GetTicks();
						decision.inputCooldown = false;
						decision.direction = eMoveDirection::IDLE;
					}
					else
					{
						switch (decision.direction)
						{
						case eMoveDirection::UPWARDS:
							normalPos.pos[1] += -movement.speed * gDeltaT;
							break;
						case eMoveDirection::DOWNWARDS:
							normalPos.pos[1] += movement.speed * gDeltaT;
							break;
						case eMoveDirection::LEFT:
							normalPos.pos[0] += -movement.speed * gDeltaT;
							break;
						case eMoveDirection::RIGHT:
							normalPos.pos[0] += movement.speed * gDeltaT;
							break;
						default:
							break;
						}
					}
				}
			}
			else
			{
				movement.endTime = SDL_GetTicks();
				if (movement.endTime - movement.startTime - movement.pause > 0)
				{
					movement.onPause = false;
				}
			}
		}
		else
		{
			decision.inputCooldown = false;
			//std::cout << "Collision\n";
		}
	}
}