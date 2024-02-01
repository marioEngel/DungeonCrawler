#include "Level.h"
#include <random>

// =============== Level stuff ====================

Level::Level()
{
	levelSize = 0;
	roomVector = std::vector<Room>{};
}

Level::~Level()
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

void Level::createLevel(int roomCount)
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
	tmpRoomData.push_back(helpCreateLevel{ Vector2D<int>{}, tmpSize });
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
		if (entry.helpVector[0] + entry.helpSize > xMax)
		{
			xMax = entry.helpVector[0] + entry.helpSize;
		}
		if (entry.helpVector[1] - entry.helpSize < yMin)
		{
			yMin = entry.helpVector[1] - entry.helpSize;
		}
		if (entry.helpVector[1] > yMax)
		{
			yMax = entry.helpVector[1];
		}
	}

	Vector2D<int> shiftVector{ xMin - 3, yMax + 3 };
	// Shift it, Flip it
	for (auto& const entry : tmpRoomData)
	{
		entry.helpVector = entry.helpVector - shiftVector;
		entry.helpVector[1] = -entry.helpVector[1];
	}

	levelSize = std::max(xMax - xMin, yMax - yMin) + 6;

	for (auto& const entry : tmpRoomData)
	{
		roomVector.push_back(Room{ entry.helpVector, entry.helpSize });
	}

	roomVector[isStart].makeStart();
	roomVector[isEnd].makeEnd();

	/*
   for (auto& const entry : tmpRoomData)
   {
	   std::cout << entry.helpVector << " size: " << entry.helpSize << std::endl;
   }
	*/
}

Matrix Level::createCollision()
{
	Matrix tmpCollisionMap{};
	makeEmptyMatrix(tmpCollisionMap, levelSize, levelSize);

	for (auto& const entry : roomVector)
	{
		for (int i = 0; i < (entry.return_roomSize() + 2); i++)
		{
			tmpCollisionMap[i + entry.return_cornerPoint()[0] - 1][entry.return_cornerPoint()[1] - 1] = 1;
			tmpCollisionMap[i + entry.return_cornerPoint()[0] - 1][entry.return_cornerPoint()[1] + entry.return_roomSize_minus1() + 1] = 1;
			if (i == 0 || i == (entry.return_roomSize() + 1))
			{
				for (int j = 0; j < entry.return_roomSize(); j++)
				{
					tmpCollisionMap[i + entry.return_cornerPoint()[0] - 1][j + entry.return_cornerPoint()[1]] = 1;
				}
			}
		}
	}

	return tmpCollisionMap;
}

Matrix Level::createRender()
{
	Matrix tmpRenderMap{};
	makeEmptyMatrix(tmpRenderMap, levelSize, levelSize);

	for (auto& const entry : roomVector)
	{
		for (int i = 0; i < entry.return_roomSize(); i++)
		{
			for (int j = 0; j < entry.return_roomSize(); j++)
			{
				tmpRenderMap[i + entry.return_cornerPoint()[0]][j + entry.return_cornerPoint()[1]] = 1;
			}
		}
	}

	return tmpRenderMap;
}

Room& Level::return_StartingRoom()
{
	Room* rtnStartRoom = nullptr;
	for (auto& const entrie : roomVector)
	{
		if (entrie.return_isStartRoom())
		{
			rtnStartRoom = &entrie;
		}
	}

	return *rtnStartRoom;
}