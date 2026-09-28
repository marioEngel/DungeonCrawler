#include "Floor.h"
#include <random>
#include "Room.h"
#include <SDL3/SDL.h>
#include "../../../System/Collision/CollisionFunc.h"
#include <algorithm>
#include "../../../Misc/MiscFunctions.h"

void Floor::init()
{
	mFloorGenerationData.numRoomTries = 10;
	mFloorGenerationData.roomExtraSize = 2;
	mFloorGenerationData.windingPercent = 25;
	mFloorGenerationData.extraConnectorChance = 10;

	std::random_device rd;
	uint64_t mSeed = rd();
	// mSeed = 4190745237
	mRNG = RNG(mSeed);
}

void Floor::init(const FloorGenerationData& inputData)
{
	mFloorGenerationData.extraConnectorChance = inputData.extraConnectorChance;
	mFloorGenerationData.numRoomTries = inputData.numRoomTries;
	mFloorGenerationData.roomExtraSize = inputData.roomExtraSize;
	mFloorGenerationData.windingPercent = inputData.windingPercent;

	std::random_device rd;
	uint64_t mSeed = rd();
	std::cout << mSeed << std::endl;
	mSeed = 341021131;
	mRNG = RNG(mSeed);

}


void Floor::setTiles(SDL_Rect& room)
{
	for (int row = room.y; row < room.y + room.h; row++)
	{
		for (int col = room.x; col < room.x + room.w; col++)
		{
			if (row == room.y || row == (room.y + room.h - 1))
			{
				mTileMap(row, col) = eTileType::WALL;
			}
			else if (col == room.x || col == (room.x + room.w - 1))
			{
				mTileMap(row, col) = eTileType::WALL;
			}
			else
			{
				mTileMap(row, col) = eTileType::FLOOR;
				mRegionMap(row, col) = mCurrentRegion;
			}
		}
	}
}

void Floor::incrementCurrentRegion()
{
	mCurrentRegion++;
}

void Floor::carveFloor(Vector2D<int> pos)
{
	mTileMap[pos] = eTileType::FLOOR;
	mRegionMap[pos] = mCurrentRegion;
}

bool Floor::canCarve(Vector2D<int> pos, Vector2D<int> dir)
{
	Vector2D<int> target = pos + dir + dir;

	if (target.x < 1 || target.x >= mFloorSize.w - 1 || 
		target.y < 1 || target.y >= mFloorSize.h - 1)
	{
		return false;
	}

	return mTileMap[target] == eTileType::ROCK;
}

Matrix<int> Floor::getTileMap()
{
	return mTileMap;
}

Matrix<int> Floor::getObjectMap()
{
	return mObjectMap;
}

void Floor::setStartEndPoint()
{
	SDL_Rect startRoom = mRoomVector[mRNG.rangeEx(mRoomVector.size())];
	SDL_Rect endRoom = chooseFarRoom(startRoom);

	Vector2D<int> startPos = getRandomPointInRoom(shrinkRoomByOne(startRoom));
	Vector2D<int> endPos = getRandomPointInRoom(shrinkRoomByOne(endRoom));

	mObjectMap[startPos] = eObjectType::START_POSITION;
	mObjectMap[endPos] = eObjectType::END_POSITION;
}

SDL_Rect Floor::chooseFarRoom(const SDL_Rect& startRoom)
{
	std::vector<float> weights;
	float totalWeight = 0.0f;

	for (const SDL_Rect& room : mRoomVector)
	{
		float dist = roomDistance(startRoom, room);
		float weight = dist * dist;
		weights.push_back(weight);
		totalWeight += weight;
	}

	float roll = mRNG.range(0.0f, totalWeight);

	float cumulative = 0.0f;
	for (size_t i = 0; i < mRoomVector.size(); i++)
	{
		cumulative += weights[i];
		if (roll <= cumulative)
		{
			return mRoomVector[i];
		}
	}

	return mRoomVector.back();
}

Vector2D<int> Floor::getRandomPointInRoom(const SDL_Rect& room)
{
	int randomCol = 0;
	int randomRow = 0;

	if (room.w == 1)
	{
		randomCol = room.x;
	}
	else
	{
		randomCol = mRNG.range(room.x + 1, room.x + room.w - 1);	
	}
	if (room.h == 1)
	{
		randomRow = room.y;
	}
	else
	{
		randomRow = mRNG.range(room.y + 1, room.y + room.h - 1);
	}

	return Vector2D<int> {randomCol, randomRow};

}


void Floor::generate(int squareSize)
{
	mTileMap = Matrix<int>(squareSize);
	mRegionMap = Matrix<int>(squareSize);
	mObjectMap = Matrix<int>(squareSize);
	matrixFillWithElement(mTileMap, eTileType::ROCK);
	matrixFillWithElement(mRegionMap, -1);


	mCurrentRegion = -1;

	mFloorSize.w = squareSize;
	mFloorSize.h = squareSize;
	
	rooms_add();
	//matrixPrintColor(mTileMap);
	//matrixPrintColor(mRegionMap);
	maze_generate();
	//matrixPrintColor(mTileMap);
	//matrixPrintColor(mRegionMap);
	regions_connect();
	//matrixPrintColor(mTileMap);
	//matrixPrintColor(mRegionMap);
	deadEnds_remove();
	//matrixPrintColor(mTileMap);
	//matrixPrintColor(mRegionMap);
}

void Floor::rooms_add()
{
	for (size_t i = 0; i < mFloorGenerationData.numRoomTries; i++)
	{
		int size = mRNG.range(1, 3 + mFloorGenerationData.roomExtraSize) * 2 + 1;
		int rectangularity = mRNG.range(0, 1 + size / 2) * 2 + 1;
		int width = size;
		int height = size;
		if (mRNG.oneIn(2))
		{
			width += rectangularity;
		}
		else
		{
			height += rectangularity;
		}

		int x = mRNG.range((mFloorSize.w - width) / 2) * 2;
		int y = mRNG.range((mFloorSize.h - height) / 2) * 2;

		SDL_Rect tmpRoom{ x, y, width, height };
		SDL_Rect tmpRoomWithBorder = addRoomBorder(tmpRoom);
		SDL_Rect tmpRoomWithBorderAdditional = addRoomBorder_doubled(tmpRoom);

		if (tmpRoomWithBorder.x < 0 || tmpRoomWithBorder.y < 0 ||
			tmpRoomWithBorder.x + tmpRoomWithBorder.w > mFloorSize.w ||
			tmpRoomWithBorder.y + tmpRoomWithBorder.h > mFloorSize.h)
		{
			continue;
		}

		bool overlaps = false;
		for (const SDL_Rect& room : mRoomVector)
		{
			SDL_Rect roomWithBorderAdditional = addRoomBorder_doubled(room);

			overlaps = check_RectVsRect(tmpRoomWithBorderAdditional, roomWithBorderAdditional);
			if (overlaps)
			{
				break;
			}
		}
		
		if (!overlaps)
		{
			incrementCurrentRegion();
			setTiles(tmpRoom);
			mRoomVector.emplace_back(tmpRoom);
		}
	}
}

void Floor::maze_generate()
{
	for (int row = 1; row < mFloorSize.h; row += 2)
	{
		for (int col = 1; col < mFloorSize.w; col += 2)
		{
			Vector2D<int> pos{ row, col };
			if (mTileMap[pos] != eTileType::ROCK)
			{
				continue;
			}

			maze_grow(pos);
		}
	}
}

void Floor::maze_grow(Vector2D<int> start)
{
	std::vector<Vector2D<int>> cells{};
	Vector2D<int> lastDir{ 0, 0 };

	incrementCurrentRegion();
	carveFloor(start);

	cells.push_back(start);

	while (!cells.empty())
	{
		Vector2D<int> cell = cells.back();

		std::vector < Vector2D<int>> unmadeCells{ };
		for (Vector2D<int> dir : mCompass.directions)
		{
			if (canCarve(cell, dir))
			{
				unmadeCells.push_back(dir);
			}
		}

		if (!unmadeCells.empty())
		{
			Vector2D<int> dir;

			if (std::find(unmadeCells.begin(), unmadeCells.end(), lastDir) != unmadeCells.end() 
				&& mRNG.range(100) > mFloorGenerationData.windingPercent)
			{
				dir = lastDir;
			}
			else
			{
				dir = unmadeCells[mRNG.rangeEx((int)unmadeCells.size())];
			}

			Vector2D<int> doorPos = cell + dir;
			Vector2D<int> nextCell = cell + dir + dir;

			carveFloor(doorPos);
			carveFloor(nextCell);

			cells.push_back(nextCell);
			lastDir = dir;
		}
		else
		{
			cells.pop_back();
			lastDir = Vector2D<int>{ 0, 0 };
		}
	}
}

void Floor::regions_connect() 
{
	//Matrix<int> test{ mFloorSize.w };
	//matrixFillWithElement(test, 0);

	std::vector<ConnectorInfo> connectorRegions{};
	for (int row = mFloorSize.y + 1; row < mFloorSize.y + mFloorSize.w - 1; row++)
	{
		for (int col = mFloorSize.x + 1; col < mFloorSize.x + mFloorSize.h - 1; col++)
		{
			Vector2D<int> pos{ row, col };

			if (mTileMap[pos] != eTileType::WALL)
			{
				continue;
			}

			std::set<int> surroundingRegions{};
			for (Vector2D<int> direction : mCompass.directions)
			{
				int region = mRegionMap[pos + direction];
				if (region != -1)
				{
					surroundingRegions.insert(region);
				}
			}

			if (surroundingRegions.size() < 2)
			{
				continue;
			}

			connectorRegions.push_back({ pos, surroundingRegions });
			//test[pos] = 1;
		}
	}

	//matrixPrintColor(test);

	std::vector<int> merged;
	std::set<int> openRegions;
	for (size_t i = 0; i <= mCurrentRegion; i++)
	{
		merged.push_back(i);
		openRegions.insert(i);
	}

	while (openRegions.size() > 1)
	{
		ConnectorInfo chosenConnector = connectorRegions[mRNG.rangeEx(connectorRegions.size())];
		junction_add(chosenConnector.pos);

		std::set<int> mappedRegions{};
		for (int region : chosenConnector.regions)
		{
			mappedRegions.insert(merged[region]);
		}

		int dest = *mappedRegions.begin();
		std::vector<int> sources(std::next(mappedRegions.begin()), mappedRegions.end());

		for (size_t i = 0; i <= mCurrentRegion; i++)
		{
			if (std::find(sources.begin(), sources.end(), merged[i]) != sources.end())
			{
				merged[i] = dest;
			}

		}
		for (int source : sources)
		{
			openRegions.erase(source);
		}
		connectorRegions.erase(
			std::remove_if(connectorRegions.begin(), connectorRegions.end(),
				[&](const ConnectorInfo& c)
				{
					std::set<int> cMappedRegions{};
					for (int region : c.regions)
					{
						cMappedRegions.insert(merged[region]);
					}

					if (cMappedRegions.size() > 1)
					{
						return false;
					}

					if ((chosenConnector.pos - c.pos).calc_amountSquared() < 4)
					{
						return true;
					}

					if (mRNG.oneIn(mFloorGenerationData.extraConnectorChance))
					{
						junction_add(c.pos);
					}

					return true;
				}),
			connectorRegions.end()
		);
	}
}

void Floor::junction_add(Vector2D<int> pos)
{
	mTileMap[pos] = eTileType::DOOR;
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

void Floor::deadEnds_remove()
{
	bool done = false;

	while (!done)
	{
		done = true;

		std::vector<Vector2D<int>> rectToPos = getPosListFromRect(
			SDL_Rect{ mFloorSize.x + 1, mFloorSize.y + 1, mFloorSize.w - 2, mFloorSize.h - 2 });
		for (Vector2D<int> pos : rectToPos)
		{

			if (mTileMap[pos] == eTileType::ROCK ||
				mTileMap[pos] == eTileType::WALL ||
				mTileMap[pos] == eTileType::DOOR)
			{
				continue;
			}

			int exits = 0;
			int roomNr = 0;
			for (Vector2D<int> dir : mCompass.directions)
			{
				if (mTileMap[pos+dir] == eTileType::FLOOR || 
					mTileMap[pos+dir] == eTileType::DOOR)
				{
					exits++;
				}
				if (mTileMap[pos+dir] == eTileType::WALL)
				{
					roomNr++;
				}
			}

			if (exits != 1 || roomNr == 3)
			{
				continue;
			}


			done = false;
			mTileMap[pos] = eTileType::ROCK;
		}
	}
}


Floor::Floor()
{
}

Floor::~Floor()
{
}