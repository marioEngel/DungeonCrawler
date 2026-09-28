#include "Tower.h"
#include "../../../ECS/Coordinator.h"
#include "Floor.h"
#include "../../../Component/Comp_TileMap.h"
#include "../../../Component/Comp_Position.h"


extern Coordinator gCoordinator;

void SysTower::init()
{
	FloorGenerationData generationData{ 15, 2, 15, 5 };
	int floorSize = 51;

	//for (size_t i = 0; i < 50; i++)
	//{
	//	std::cout << "test " << i << ": \n";
	//	Floor testFloor{};
	//	testFloor.init(generationData);
	//	testFloor.generate(floorSize);
	//	Matrix<int> testMatrix = testFloor.getTileMap();
	//	matrixPrintColor(testMatrix);
	//}


	Floor tmpFloor;
	tmpFloor.init(generationData);
	tmpFloor.generate(floorSize);

	std::vector<const char*> tmpTileTextures =
	{
		"Picture/TileNormal.png",
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

	currentFloorIndex = 0;
	floorEntityList.push_back(currentFloor);
	floorList.push_back(tmpFloor);
}

void SysTower::syncPlayer()
{
	for (const auto& entity : mEntities)
	{
		auto& pos = gCoordinator.GetComponent<Position>(entity);
		
		Floor tmpFloor = floorList[currentFloorIndex];
		Matrix<int> tmpObjectMap = tmpFloor.getObjectMap();

		for (int row = 0; row < tmpObjectMap.rows(); row++)
		{
			for (int col = 0; col < tmpObjectMap.cols(); col++)
			{
				if (tmpObjectMap(row, col) == eObjectType::START_POSITION)
				{
					pos.vec = Vector2D<float>(float(row) * 32.0f, float(col) * 32.0f);
				}
			}
		}
	}
}