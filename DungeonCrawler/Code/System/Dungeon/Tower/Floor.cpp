#include "Floor.h"
#include <random>
#include "Room.h"
#include <SDL3/SDL.h>
#include "../../../System/Collision/Collision.h"
#include <algorithm>

void Floor::init()
{
	floorGenerationData.extraConnectorChance = 10;
	floorGenerationData.numRoomTries = 10;
	floorGenerationData.roomExtraSize = 2;
	floorGenerationData.windingPercent = 25;

	std::random_device rd;
	uint64_t mSeed = rd();
	// mSeed = 4190745237
	mRNG = RNG(rd());
}


void Floor::setTiles(SDL_Rect& room)
{
	for (int row = room.y; row < room.y + room.h; row++)
	{
		for (int col = room.x; col < room.x + room.w; col++)
		{
			if (row == room.y || row == (room.y + room.h - 1))
			{
				floorTileMap(row, col) = eTileType::WALL;
				regionMap(row, col) = currentRegion;
			}
			else if (col == room.x || col == (room.x + room.w - 1))
			{
				floorTileMap(row, col) = eTileType::WALL;
				regionMap(row, col) = currentRegion;
			}
			else
			{
				floorTileMap(row, col) = eTileType::FLOOR;
				regionMap(row, col) = currentRegion;
			}
		}
	}
}

void Floor::incrementCurrentRegion()
{
	currentRegion++;
}

void Floor::carveFloor(Vector2D<int> pos)
{
	floorTileMap[pos] = eTileType::FLOOR;
	regionMap[pos] = currentRegion;
}

bool Floor::canCarve(Vector2D<int> pos, Vector2D<int> dir)
{
	Vector2D<int> target = pos + dir + dir;

	if (target.x < 1 || target.x >= floorSize.w - 1 || 
		target.y < 1 || target.y >= floorSize.h - 1)
	{
		return false;
	}

	return floorTileMap[target] == eTileType::WALL;
}


void Floor::generate()
{
	int squareSize = 51;

	floorTileMap = Matrix<int>(squareSize);
	regionMap = Matrix<int>(squareSize);
	matrixFillWithElement(floorTileMap, eTileType::WALL);
	matrixFillWithElement(regionMap, -1);


	currentRegion = -1;

	floorSize.w = squareSize;
	floorSize.h = squareSize;

	rooms_add();
	matrixPrintColor(floorTileMap);
	maze_generate();
	regions_connect();
	matrixPrintColor(floorTileMap);
	deadEnds_remove();
	matrixPrintColor(floorTileMap);
}

void Floor::rooms_add()
{
	for (size_t i = 0; i < floorGenerationData.numRoomTries; i++)
	{
		int size = mRNG.range(1, 3 + floorGenerationData.roomExtraSize) * 2 + 1;
		int rectangularity = mRNG.range(0, 1 + size / 2) * 2;
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

		int x = mRNG.range((floorSize.w - width) / 2) * 2 + 1;
		int y = mRNG.range((floorSize.h - height) / 2) * 2 + 1;

		SDL_Rect tmpRoom{ x, y, width, height };
		SDL_Rect tmpRoomWithBorder = addRoomBorder(tmpRoom);
		SDL_Rect tmpRoomWithBorderAdditional = addRoomBorder_doubled(tmpRoom);

		if (tmpRoomWithBorder.x < 0 || tmpRoomWithBorder.y < 0 ||
			tmpRoomWithBorder.x + tmpRoomWithBorder.w > floorSize.w ||
			tmpRoomWithBorder.y + tmpRoomWithBorder.h > floorSize.h)
		{
			continue;
		}

		bool overlaps = false;
		for (const SDL_Rect room : roomVector)
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
			roomVector.emplace_back(tmpRoom);
		}
	}
}

void Floor::maze_generate()
{
	for (int row = 1; row < floorSize.h; row += 2)
	{
		for (int col = 1; col < floorSize.w; col += 2)
		{
			Vector2D<int> pos{ row, col };
			if (floorTileMap[pos] != eTileType::WALL) 
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
				&& mRNG.range(100) > floorGenerationData.windingPercent)
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
	std::vector<ConnectorInfo> connectorRegions{};
	for (int row = floorSize.y + 1; row < floorSize.y + floorSize.w - 1; row++)
	{
		for (int col = floorSize.x + 1; col < floorSize.x + floorSize.h - 1; col++)
		{
			Vector2D<int> pos{ row, col };

			if (floorTileMap[pos] != eTileType::WALL)
			{
				continue;
			}

			std::set<int> surroundingRegions{};
			for (Vector2D<int> direction : mCompass.directions)
			{
				int region = regionMap[pos + direction];
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
		}
	}

	std::vector<int> merged;
	std::set<int> openRegions;
	for (size_t i = 0; i <= currentRegion; i++)
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

		for (size_t i = 0; i <= currentRegion; i++)
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
					if ((chosenConnector.pos - c.pos).calc_amountSquared() < 4)
					{
						return true;
					}

					std::set<int> cMappedRegions{};
					for (int region : c.regions)
					{
						cMappedRegions.insert(merged[region]);
					}

					if (cMappedRegions.size() > 1)
					{
						return false;
					}

					if (mRNG.oneIn(floorGenerationData.extraConnectorChance))
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
	if (mRNG.oneIn(4))
	{
		if (mRNG.oneIn(3))
		{
			floorTileMap[pos] = eTileType::DOOR;
		}
		else
		{
			floorTileMap[pos] = eTileType::WALL;
		}
	}
	else
	{
		floorTileMap[pos] = eTileType::DOOR;
	}
}

void Floor::deadEnds_remove()
{
	bool done = false;

	while (!done)
	{
		done = true;

		std::vector<Vector2D<int>> rectToPos = getPosListFromRect(
			SDL_Rect{ floorSize.x + 1, floorSize.y + 1, floorSize.w - 2, floorSize.h - 2 });
		for (Vector2D<int> pos : rectToPos)
		{
			if (floorTileMap[pos] == eTileType::WALL)
			{
				continue;
			}

			int exits = 0;
			for (Vector2D<int> dir : mCompass.directions)
			{
				if (floorTileMap[pos+dir] != eTileType::WALL)
				{
					exits++;
				}
			}

			if (exits != 1)
			{
				continue;
			}

			done = false;
			floorTileMap[pos] = eTileType::WALL;
		}
	}
}


Floor::Floor()
{
}

Floor::~Floor()
{
}

SDL_Rect addRoomBorder(const SDL_Rect& rect)
{
	return SDL_Rect{
		rect.x - 1,
		rect.y - 1,
		rect.w + 2,
		rect.h + 2
	};
}

SDL_Rect addRoomBorder_doubled(const SDL_Rect& rect)
{
	return SDL_Rect{
		rect.x - 2,
		rect.y - 2,
		rect.w + 4,
		rect.h + 4
	};
}

std::vector<Vector2D<int>> getPosListFromRect(const SDL_Rect& rect)
{
	std::vector<Vector2D<int>> rtnList{};

	for (int row = rect.y; row < rect.y + rect.h; row++)
	{
		for (int col = rect.x; col < rect.x + rect.w; col++)
		{
			rtnList.push_back(Vector2D<int>(row, col));
		}
	}

	return rtnList;
}