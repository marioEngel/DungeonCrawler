#include "Floor.h"
#include <random>
#include "Room.h"
#include <SDL3/SDL.h>
#include "../../../System/Collision/CollisionFunc.h"
#include <algorithm>
#include "../../../Misc/MiscFunctions.h"
#include "../Enums/ObjectType.h"

void Floor::init() {
    mFloorGenerationData.numRoomTries = 10;
    mFloorGenerationData.roomExtraSize = 2;
    mFloorGenerationData.windingPercent = 25;
    mFloorGenerationData.extraConnectorChance = 10;

    std::random_device rd;
    uint64_t mSeed = rd();
    //mSeed = 170038745;
    mRNG = RNG(mSeed);
}

void Floor::init(const FloorGenerationData &inputData) {
    mFloorGenerationData.extraConnectorChance = inputData.extraConnectorChance;
    mFloorGenerationData.numRoomTries = inputData.numRoomTries;
    mFloorGenerationData.roomExtraSize = inputData.roomExtraSize;
    mFloorGenerationData.windingPercent = inputData.windingPercent;

    std::random_device rd;
    uint64_t mSeed = rd();
    std::cout << mSeed << std::endl;
    //mSeed = 2576283255;
    mRNG = RNG(mSeed);
}

void Floor::setStartEndPoint() {
    const SDL_Rect startRoom = mRoomVector[mRNG.rangeEx(mRoomVector.size())];
    const SDL_Rect endRoom = chooseFarRoom(startRoom);

    const Vector2D<int> startPos = getRandomPointInRoom(shrinkRoomByN(startRoom));
    const Vector2D<int> endPos = getRandomPointInRoom(shrinkRoomByN(endRoom));

    mMap_objects[startPos] = eObjectType::START_POSITION;
    mMap_objects[endPos] = eObjectType::END_POSITION;
}

// ------------- private funcions ------------------------------

Matrix<int> Floor::getMap_tileTypes() {
    return mMap_tileTypes;
}

Matrix<int> Floor::getMap_objects() {
    return mMap_objects;
}

Matrix<int> Floor::getMap_collision() {
    return mMap_collision;
}

void Floor::setTiles(SDL_Rect &room) {
    for (int xxx = room.x; xxx < room.x + room.w; xxx++) {
        for (int yyy = room.y; yyy < room.y + room.h; yyy++) {
            if (xxx == room.x || xxx == (room.x + room.w - 1) ||
                yyy == room.y || yyy == (room.y + room.h - 1)) {
                mMap_tileTypes(xxx, yyy) = eTileType::WALL;
            } else {
                mMap_tileTypes(xxx, yyy) = eTileType::FLOOR;
                mMap_regions(xxx, yyy) = mCurrentRegion;
            }
        }
    }
}

void Floor::incrementCurrentRegion() {
    mCurrentRegion++;
}

void Floor::carveFloor(Vector2D<int> pos) {
    mMap_tileTypes[pos] = eTileType::FLOOR;
    mMap_regions[pos] = mCurrentRegion;
}

bool Floor::canCarve(Vector2D<int> pos, Vector2D<int> dir) {
    const Vector2D<int> target = pos + dir + dir;

    if (target.x < 1 || target.x >= mFloorSize.w - 1 ||
        target.y < 1 || target.y >= mFloorSize.h - 1) {
        return false;
    }

    return mMap_tileTypes[target] == eTileType::ROCK;
}

SDL_Rect Floor::chooseFarRoom(const SDL_Rect &startRoom) {
    std::vector<float> weights;
    float totalWeight = 0.0f;

    for (const SDL_Rect &room: mRoomVector) {
        const float dist = rectDistance(startRoom, room);
        float weight = dist * dist;
        weights.push_back(weight);
        totalWeight += weight;
    }

    const float roll = mRNG.range(0.0f, totalWeight);

    float cumulative = 0.0f;
    for (size_t i = 0; i < mRoomVector.size(); i++) {
        cumulative += weights[i];
        if (roll <= cumulative) {
            return mRoomVector[i];
        }
    }

    return mRoomVector.back();
}

Vector2D<int> Floor::getRandomPointInRoom(const SDL_Rect &room) {
    int randomX = 0;
    int randomY = 0;

    if (room.w == 1) {
        randomX = room.x;
    } else {
        randomX = mRNG.range(room.x + 1, room.x + room.w - 1);
    }
    if (room.h == 1) {
        randomY = room.y;
    } else {
        randomY = mRNG.range(room.y + 1, room.y + room.h - 1);
    }

    return Vector2D<int>{randomX, randomY};
}

void Floor::junction_add(Vector2D<int> &pos) {
    mMap_tileTypes[pos] = eTileType::DOOR;
    //if (mRNG.oneIn(4))
    //{
    //	if (mRNG.oneIn(3))
    //	{
    //		mTileMap[pos] = eTileType::DOOR;
    //	}
    //	else
    //	{
    //		mTileMap[pos] = eTileType::WALL;
    //	}
    //}
    //else
    //{
    //	mTileMap[pos] = eTileType::DOOR;
    //}
}

void Floor::createCollisionMap() {
    for (int xxx = 0; xxx < mMap_tileTypes.width(); xxx++) {
        for (int yyy = 0; yyy < mMap_tileTypes.height(); yyy++) {
            if (mMap_tileTypes(xxx, yyy) == eTileType::WALL ||
                mMap_tileTypes(xxx, yyy) == eTileType::ROCK) {
                mMap_collision(xxx, yyy) = 1;
            }
        }
    }
}

Floor::Floor() {
}

Floor::~Floor() {
}
