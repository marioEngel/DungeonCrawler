#pragma once
#include <array>
#include <vector>
#include "../../Math/Matrix.h"
#include "../../Math/Vector2D.h"

// ============= For testing will be moved into the dungeon folder ===============
// Room2 and Level2 are created for testing purposes 

/* ===== Room class =====
 * The class represents a room in a level of the dungeon
 * In the beginning:
 * - has at least one "door" into other rooms
 * - will have a random amount of enimies 
 * - has a flag for the starting room: start position (first floor/level) or downward stairs (the ongoing floors/levels)
 * - has a flag for the end room: upward stairs 
 * - the to points above will be asigned by the Level class 
 * - maybe some items and other stuff
 * In the future:
 * - maybe room templates
 * - more complex room shapes (not square)
 */
class Room
{
public:
	Room(int pointX, int pointY, int size);
	~Room();
	int returnRoomSize();
	int returnRoomSizeSquare();
	int returnCornerX();
	int returnCornerY();
	void printData();
	void createRandomDoors(int count = 2);
	std::vector<std::array<int, 2>> returnDoors();

private:
	std::array<int, 2> cornerPoint;
	int roomSize; // square rooms only
	std::vector<std::array<int, 2>> doors;
};

class Room2
{
public:
	Room2(int xCornerPoint, int yCornerPoint, int size);
	Room2(Vector2D<int> inputPoints, int size);
	~Room2();
	
	int return_roomSize();
	int return_roomSize_squared();
	Vector2D<int> return_cornerPoint();

	void change_cornerPoint(Vector2D<int> shiftVector);
	void change_cornerPoint(int xShift, int yShift);

	void makeStart(bool value = true);
	void makeEnd(bool value = true);

private:
	//std::array<int, 2> cornerPoint;
	Vector2D<int> cornerPoint;
	int roomSize;		// currently only squared rooms
	// both variable below cant be true at the same time
	bool isStartRoom;		 
	bool isEndRoom;
	//std::vector<std::array<int, 2>> doorVec;
	std::vector<Vector2D<int>> doorVec;
};

/* ===== Level =====
 * the class creates a floor/level of the dungeon
 */
class Level
{
public:
	Level(int size);
	~Level();

	void printStuff();
	void createRandomRoom();
	void createLevel(int roomCount = 5);
	
	Matrix returnLevelMatrix();

private:
	std::vector<Room> roomVector;
	int levelSize;
	Matrix LevelSpace;  // 0 = free, 1 = not free
	//std::vector<std::array<int, 2>> freeSpace;
	//int entryCount;
};

class Level2
{
public:
	Level2(int size = 1);
	~Level2();

	void createLevel(int roomCount);


private:
	std::vector<Room2> roomVector;
	int levelSize;
};
