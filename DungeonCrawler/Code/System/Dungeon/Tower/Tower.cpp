#include "Tower.h"
#include "../../../ECS/Coordinator.h"
#include "Floor.h"
#include "../../../Component/Comp_TileMap.h"
#include "../../../Component/Comp_Position.h"


extern Coordinator gCoordinator;

void SysTower::init()
{
	FloorGenerationData generationData{ 15, 2, 15, 10 };
	int floorSize = 75;

	Floor tmpFloor;


	tmpFloor.init(generationData);
	tmpFloor.generate(floorSize);

	std::vector<const char*> tmpTileTextures =
	{
		"Picture/TileNormal.png",
		"Picture/TileGround.png",
		"Picture/FullPink.png"
	};
	std::vector<SDL_Texture*> emtpyTexture{};
	Matrix<int> tmpMatrix = tmpFloor.getTileMap();
	matrixPrintColor(tmpMatrix);


	Entity currentFloor = gCoordinator.CreateEntity();
	{
		gCoordinator.AddComponent<TileMap>(
			currentFloor,
			TileMap{
				tmpTileTextures,
				emtpyTexture,
				tmpMatrix
			}
		);
	}

	tmpFloor.setStartEndPoint();

	currentEntity = 0;
	floorEntityList.push_back(currentFloor);
	floorList.push_back(tmpFloor);
}

void SysTower::syncPlayer()
{
	for (const auto& entity : mEntities)
	{
		auto pos = gCoordinator.GetComponent<Position>(entity);
		
		Floor tmpFloor = floorList[currentEntity];
		Matrix<int> tmpObjectMap = tmpFloor.getObjectMap();

		matrixPrintColor(tmpObjectMap);

		for (int row = 0; row < tmpObjectMap.rows(); row++)
		{
			for (int col = 0; col < tmpObjectMap.cols(); col++)
			{
				if (tmpObjectMap(row, col) == eObjectType::START_POSITION)
				{
					pos.pos = Vector2D<float>(float(row) * 32.0f, float(col) * 32.0f);
					std::cout << "Here" << std::endl;
				}
			}
		}
	}
}