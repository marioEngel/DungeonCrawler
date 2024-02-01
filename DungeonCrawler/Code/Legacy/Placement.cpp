#include "Placement.h"
#include "../ECS/Coordinator.h"
#include "../Component/AffiliationComp.h"
#include <vector>
#include "../Component/TileMapComp.h"
#include "../Component/PositionComp.h"
#include "../Component/MatrixPositionComp.h"

extern Coordinator gCoordinator;

void PlacementSystem::init()
{
	std::vector<int> startPos;

	for (auto& const entity : mEntities)
	{
		auto& affiliation = gCoordinator.GetComponent<Affiliation>(entity);
		if (affiliation.affiliation == eAffKind::Map)
		{
			auto& tileMap = gCoordinator.GetComponent<TileMap>(entity);

			//startPos = tileMap.startPoint;
		}
		if (affiliation.affiliation == eAffKind::Player)
		{
			auto& position = gCoordinator.GetComponent<Position>(entity);
			auto& matrixPos = gCoordinator.GetComponent<MatrixPosition>(entity);

			position.pos[0] = float(startPos[0]) * 32.0f;
			position.pos[1] = float(startPos[1]) * 32.0f;

			matrixPos.matrixPosition[0] = startPos[0];
			matrixPos.matrixPosition[1] = startPos[1];
		}
	}
}

void PlacementSystem::placeOnTile() 
{
	for (auto& const entity : mEntities)
	{
		auto& affiliation = gCoordinator.GetComponent<Affiliation>(entity);
		if (affiliation.affiliation == eAffKind::Player)
		{
			auto& position = gCoordinator.GetComponent<Position>(entity);
			auto& matrixPos = gCoordinator.GetComponent<MatrixPosition>(entity);

			int tmpPosX = int(position.pos[0]) / 32;
			int tmpPosY = int(position.pos[1]) / 32;

			int tmpIntX = 0;
			int tmpIntY = 0;

			if ((int(position.pos[0]) % 32) > 16)
			{
				tmpIntX = 1;
			}
			if ((int(position.pos[1]) % 32) > 16)
			{
				tmpIntY = 1;
			}

			position.pos[0] = float(tmpPosX + tmpIntX) * 32.0f;
			position.pos[1] = float(tmpPosY + tmpIntY) * 32.0f;

			matrixPos.matrixPosition[0] = tmpPosX + tmpIntX;
			matrixPos.matrixPosition[1] = tmpPosY + tmpIntY;
		}
	}
}