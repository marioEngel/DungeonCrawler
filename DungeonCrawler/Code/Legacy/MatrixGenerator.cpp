#include "MatrixGenerator.h"
#include <iostream>
#include <random>
#include "../../Math/Vector2D.h"

Room::Room(int pointX, int pointY, int size)
{
	cornerPoint[0] = pointX;
	cornerPoint[1] = pointY;
	roomSize = size;
}

Room::~Room()
{
}

int Room::returnRoomSize()
{
	return roomSize;
}

int Room::returnRoomSizeSquare()
{
	return roomSize * roomSize;
}

int Room::returnCornerX()
{
	return cornerPoint[0];
}

int Room::returnCornerY()
{
	return cornerPoint[1];
}

void Room::printData()
{
	std::cout << "Corner: (" << cornerPoint[0] << ", " << cornerPoint[1] << ")\n";
	std::cout << "Roomsize: " << roomSize << std::endl;
	for (size_t i = 0; i < doors.size(); i++)
	{
		std::cout << "Door " << i << ": (" << doors[i][0] << ", " << doors[i][1] << ")\n";
	}
	std::cout << " " << std::endl;
}

void Room::createRandomDoors(int count)
{
	for (int i = 0; i < count; i++)
	{
		bool fittingDoor = true;
		// random number stuff
		std::default_random_engine generator;
		std::uniform_int_distribution<int> distribution(1, roomSize - 2);
		std::uniform_int_distribution<int> coinflip(0, 1);
		std::random_device rd;
		generator.seed(rd());

		int tmpXval;
		int tmpYval;
		do
		{
			// horizontal
			if (coinflip(generator) == 0)
			{
				// top  
				if (coinflip(generator) == 0)
				{
					tmpYval = cornerPoint[1];
					tmpXval = distribution(generator) + cornerPoint[0];
				}
				// bottom
				else
				{
					tmpYval = cornerPoint[1] + roomSize - 1;
					tmpXval = distribution(generator) + cornerPoint[0];
				}
			}
			// vertical 
			else
			{
				// left 
				if (coinflip(generator) == 0)
				{
					tmpXval = cornerPoint[0];
					tmpYval = distribution(generator) + cornerPoint[1];
				}
				// right
				else
				{
					tmpXval = cornerPoint[0] + roomSize - 1;
					tmpYval = distribution(generator) + cornerPoint[1];
				}
			}

			for (size_t i = 0; i < doors.size(); i++)
			{
				if (tmpXval != doors[i][0] && 
					tmpXval != doors[i][0] + 1 && 
					tmpXval != doors[i][0] - 1 &&
					tmpYval != doors[i][1] &&
					tmpYval != doors[i][1] + 1 &&
					tmpYval != doors[i][1] - 1)
				{
					fittingDoor = true;
				}
				else
				{
					fittingDoor = false;
				}
			}
		} while (!fittingDoor);
	
		doors.push_back({ tmpXval, tmpYval });
	}
}

std::vector<std::array<int, 2>> Room::returnDoors()
{
	return doors;
}

// =====================LEVEL===============================================

Level::Level(int size)
{
	levelSize = size;
	Matrix tmpMatrix{};
	makeEmptyMatrix(tmpMatrix, size, size);
	LevelSpace = tmpMatrix;
}

Level::~Level()
{
}

void Level::printStuff()
{
	std::cout << "Levelsize: " << levelSize << std::endl;
	//std::cout << "Count: " << entryCount << std::endl;
	/*
	for (size_t i = 0; i < freeSpace.size(); i++)
	{
		std::cout << freeSpace[i][0] << " : " << freeSpace[i][1] << std::endl;
	}
	*/

	for (size_t i = 0; i < roomVector.size(); i++)
	{
		roomVector[i].printData();
	}
}

void Level::createRandomRoom()
{
	// random number stuff
	std::default_random_engine generator;
	std::normal_distribution<double> distribution(8., 2.0);
	std::uniform_int_distribution<int> intribution(2, levelSize - 3);
	std::random_device rd;
	generator.seed(rd());

	int tmpRoomSize = int(round(distribution(generator)));

	std::array<int, 2> tmpCorner;

	bool isFree = false;
	do
	{
		tmpCorner[0] = intribution(generator);
		tmpCorner[1] = intribution(generator);

		if ((tmpCorner[0] + tmpRoomSize < levelSize - 2) 
			&& (tmpCorner[1] + tmpRoomSize < levelSize - 2))
		{
			if (LevelSpace[tmpCorner[0]][tmpCorner[1]] == 0)
			{
				int count = 0;
				int widerRoomSize = tmpRoomSize + 4; // two rows free around rooms
				for (int i = 0; i < widerRoomSize * widerRoomSize; i++)
				{
					if (LevelSpace[tmpCorner[0] + i % widerRoomSize - 2]
						[tmpCorner[1] + i / widerRoomSize - 2] == 1)
					{
						count++;
					}
				}

				if (count == 0)
				{
					isFree = true;
				}
			}
		}
	} while (!isFree);

	Room tmpRoom{ tmpCorner[0] ,tmpCorner[1], tmpRoomSize };
	tmpRoom.createRandomDoors(2);
	roomVector.push_back(tmpRoom);

	std::vector<int> tmpIndex;

	int furtherRoomSize = tmpRoomSize + 4;
	for (int i = 0; i < furtherRoomSize * furtherRoomSize; i++)
	{
		
		LevelSpace[tmpCorner[0] + i % furtherRoomSize - 2]
			[tmpCorner[1] + i / furtherRoomSize - 2] = 1;
	}
}

void Level::createLevel(int roomCount)
{
	for (int i = 0; i < roomCount; i++)
	{
		this->createRandomRoom();;
	}
}

Matrix Level::returnLevelMatrix()
{
	Matrix rtnMatrix{};
	makeEmptyMatrix(rtnMatrix, levelSize, levelSize);
	for (size_t i = 0; i < roomVector.size(); i++)
	{
		//roomVector[i].printData();
		for (int j = 0; j < roomVector[i].returnRoomSizeSquare(); j++)
		{
			rtnMatrix[roomVector[i].returnCornerX() + j % roomVector[i].returnRoomSize()]
				[roomVector[i].returnCornerY() + j / roomVector[i].returnRoomSize()] = 1;
		}
		std::vector<std::array<int, 2>> tmpDoor = roomVector[i].returnDoors();
		for (size_t i = 0; i < tmpDoor.size(); i++)
		{
			rtnMatrix[tmpDoor[i][0]][tmpDoor[i][1]] = 2;
		}
	}

	return rtnMatrix;
}

// ===================== TEST ===============================================

// =============== Room2 stuff ====================
Room2::Room2(int xCornerPoint, int yCornerPoint, int size)
{
	cornerPoint = Vector2D<int>{ xCornerPoint, yCornerPoint };
	roomSize = size;
	isStartRoom = false;
	isEndRoom = false;
}

Room2::Room2(Vector2D<int> inputPoints, int size)
{
	cornerPoint = inputPoints;
	roomSize = size;
	isStartRoom = false;
	isEndRoom = false;
}

Room2::~Room2()
{
}

int Room2::return_roomSize()
{
	return roomSize;
}

int Room2::return_roomSize_squared()
{
	return roomSize * roomSize;
}

Vector2D<int> Room2::return_cornerPoint()
{
	return cornerPoint;
}

void Room2::change_cornerPoint(Vector2D<int> shiftVector)
{
	cornerPoint = cornerPoint - shiftVector;
}

void Room2::change_cornerPoint(int xShift, int yShift)
{
	cornerPoint = cornerPoint - Vector2D<int>{xShift, yShift};
}

void Room2::makeStart(bool value)
{
	isStartRoom = value;
}

void Room2::makeEnd(bool value)
{
	isEndRoom = value;
}

// =============== Level2 stuff ====================

Level2::Level2(int size)
{
	levelSize = size;
	roomVector = std::vector<Room2>{};
}

Level2::~Level2()
{
}

// struct for the createLevel() function not needed otherwise maybe add in header
struct helpCreateLevel
{
	Vector2D<int> helpVector;
	int helpSize;
};

bool checkOverlap(Vector2D<int> newPosition, int newSize, helpCreateLevel currentRoomData, int additionalEdge = 2)
{
	bool isOverlapping = false;

	if ((newPosition[0] + newSize + additionalEdge) > (currentRoomData.helpVector[0] - additionalEdge)
		&& (newPosition[0] - additionalEdge) < (currentRoomData.helpVector[0] + currentRoomData.helpSize + additionalEdge))
	{
		if ((newPosition[1] + newSize + additionalEdge) > (currentRoomData.helpVector[1] - additionalEdge)
			&& (newPosition[1] - additionalEdge) < (currentRoomData.helpVector[1] + currentRoomData.helpSize + additionalEdge))
		{
			isOverlapping = true;
		}
	}

	return isOverlapping;
}

void sizeRange(int& insertSize)
{
	if (insertSize < 4)
	{
		insertSize = 4;
	}
	else if (insertSize > 10)
	{
		insertSize = 10;
	}
}

void Level2::createLevel(int roomCount)
{
	/*
	 * 1. Create a reference room at (0,0)
	 * 2. Generate the other rooms in reference with gauss distribution (or other in the future)
	 * 3. Room size with an gamma distribution or chi squared distribution [I choose you gamma]
	 * 4. Calculate xmin, xmax, ymin an ymax of all rooms
	 * 5. Shift every postion by (xmin, ymin) and (3, 3) (three tiles edge)
	 * 6. New room size is max(xmax, ymax) + 6
	 * 7. create everything
	 */

	// random number stuff
	// optimize distribution parameters sizeRange() function
	std::default_random_engine generator;
	std::gamma_distribution<double> distribution_roomSize(3.0, 2.0);
	std::normal_distribution<double> distribution_roomDistance(0.0, 10);
	std::uniform_int_distribution<int> distribution_roomFlags(0, roomCount - 1);
	//std::normal_distribution<double> distribution_roomDistanceY;
	std::random_device rd;
	generator.seed(rd());

	std::vector<helpCreateLevel> tmpRoomData;
	int tmpSize;
	Vector2D<int> tmpRoomPosition;

	// reference Room
	tmpSize = int(round(distribution_roomSize(generator)));
	sizeRange(tmpSize);
	//tmpRoomData.push_back(std::array<int, 3> {0, 0, tmpSize});
	tmpRoomData.push_back(helpCreateLevel{Vector2D<int>{}, tmpSize});
	bool roomPossible;
	int randXpos;
	int randYpos;
	int isStart = distribution_roomFlags(generator);
	int isEnd = 0;
	do
	{
		isEnd = distribution_roomFlags(generator);
	} while (isEnd == isStart);

	for (int i = 0; i < roomCount - 1; i++)
	{
		do
		{
			roomPossible = true;
			randXpos = int(round(distribution_roomDistance(generator)));
			randYpos = int(round(distribution_roomDistance(generator)));
			tmpRoomPosition = Vector2D<int>{ randXpos, randYpos };
			tmpSize = int(round(distribution_roomSize(generator)));
			sizeRange(tmpSize);

			for (auto& const entry : tmpRoomData)
			{
				if (checkOverlap(tmpRoomPosition, tmpSize, entry))
				{
					roomPossible = false;
				}
			}
			if (roomPossible)
			{
				tmpRoomData.push_back(helpCreateLevel{ tmpRoomPosition, tmpSize });
			}
		} while (!roomPossible);
	}

	/* normal coordinate system(0, 0) in centre
	 * -> SDL2 coordinates centre upper left corner, y into down direction
	 * xMin -> -inf
	 * xMax -> inf
	 * yMin -> inf
	 * yMax -> -inf
	 */ 
	int xMin = 0;
	int xMax = 0;
	int yMin = 0;
	int yMax = 0;

	for (auto& const entry : tmpRoomData)
	{
		if (entry.helpVector[0] < xMin)
		{
			xMin = entry.helpVector[0];
		}
		if (entry.helpVector[0] > xMax)
		{
			xMax = entry.helpVector[0];
		}
		if (entry.helpVector[1] < yMin)
		{
			yMin = entry.helpVector[1];
		}
		if (entry.helpVector[1] > yMax)
		{
			yMax = entry.helpVector[1];
		}
	}

	Vector2D<int> shiftVector{ xMin - 3, -yMax - 3 };
	
	for (auto& const entry : tmpRoomData)
	{
		entry.helpVector = entry.helpVector - shiftVector;
	}

	levelSize = std::max(xMax - xMin, yMax - yMin);

	for (auto& const entry : tmpRoomData)
	{
		roomVector.push_back(Room2{ entry.helpVector, entry.helpSize });
	}

	roomVector[isStart].makeStart();
	roomVector[isEnd].makeEnd();
	
	std::cout << isStart << ' ' << isEnd << ' ' << levelSize << std::endl;


	 /*
	for (auto& const entry : tmpRoomData)
	{
		std::cout << entry.helpVector << " size: " << entry.helpSize << std::endl;
		entry.helpVector = entry.helpVector	 - shiftVector;
	}
	

	std::cout << '\n';
	std::cout << shiftVector;
	std::cout << '\n' << '\n';

	for (auto& const entry : tmpRoomData)
	{
		std::cout << entry.helpVector << " size: " << entry.helpSize << std::endl;
	}
	 */
}