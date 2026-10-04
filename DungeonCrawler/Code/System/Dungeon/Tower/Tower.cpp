#include "Tower.h"
#include "../../../ECS/Coordinator.h"
#include "Floor.h"
#include "../../../Component/Comp_TileMap.h"
#include "../../../Component/Comp_Position.h"
#include "../Enums/ObjectType.h"

extern Coordinator gCoordinator;

void SysTower::init() {
    const FloorGenerationData generationData{
        .numRoomTries = 15,
        .roomExtraSize = 2,
        .windingPercent = 15,
        .extraConnectorChance = 5
    };

    const int floorSize = 51;

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

    std::vector<const char *> tmpTileTextures =
    {
        "Picture/TileNormal.png",
        "Picture/TileNormal.png",
        "Picture/TileGround.png",
        "Picture/FullPink.png"
    };
    const std::vector<SDL_Texture *> emtpyTexture{};
    const Matrix<int> tmpMatrix = tmpFloor.getMap_tileTypes();
    mCollisionMap = tmpFloor.getMap_collision();

    matrixPrintColor(tmpMatrix);
    matrixPrintColor(mCollisionMap);


    Entity currentFloor = gCoordinator.CreateEntity();
    {
        gCoordinator.AddComponent<TileMap>(
            currentFloor,
            TileMap{
                .paths = tmpTileTextures,
                .textures = emtpyTexture,
                .matrix = tmpMatrix
            }
        );
    }

    tmpFloor.setStartEndPoint();

    currentFloorIndex = 0;
    floorEntityList.push_back(currentFloor);
    floorList.push_back(tmpFloor);
}

void SysTower::syncPlayer() {
    for (const auto &entity: mEntities) {
        auto &pos = gCoordinator.GetComponent<Position>(entity);

        Floor tmpFloor = floorList[currentFloorIndex];
        Matrix<int> tmpObjectMap = tmpFloor.getMap_objects();

        for (int xxx = 0; xxx < tmpObjectMap.width(); xxx++) {
            for (int yyy = 0; yyy < tmpObjectMap.height(); yyy++) {
                if (tmpObjectMap(xxx, yyy) == eObjectType::START_POSITION) {
                    pos.vec = Vector2D<float>(static_cast<float>(xxx) * 32.0f, static_cast<float>(yyy) * 32.0f);
                }
            }
        }
    }
}
