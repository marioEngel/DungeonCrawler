#include "ConversePosition.h"
#include "../Component/MatrixPositionComp.h"
#include "../Component/PositionComp.h"
#include "../ECS/Coordinator.h"

extern Coordinator gCoordinator;

void ConversePositionSystem::conversePM()
{
	for (auto& const entity : mEntities)
	{
		auto& position = gCoordinator.GetComponent<Position>(entity);
		auto& matrixPos = gCoordinator.GetComponent<MatrixPosition>(entity);

		matrixPos.matrixPosition[0] = int(round(position.pos[0] / 32.0f));
		matrixPos.matrixPosition[1] = int(round(position.pos[1] / 32.0f));
	}
}

void ConversePositionSystem::converseMP()
{
	for (auto& const entity : mEntities)
	{
		auto& position = gCoordinator.GetComponent<Position>(entity);
		auto& matrixPos = gCoordinator.GetComponent<MatrixPosition>(entity);

		position.pos[0] = float(matrixPos.matrixPosition[0]) * 32.0f;
		position.pos[1] = float(matrixPos.matrixPosition[1]) * 32.0f;
	}
}