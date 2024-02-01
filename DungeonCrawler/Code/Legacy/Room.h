#pragma once
#include "../../Math/Vector2D.h"
#include <vector>

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
	Room(int xCornerPoint, int yCornerPoint, int size);
	Room(Vector2D<int> inputPoints, int size);
	~Room();

	int return_roomSize();
	int return_roomSize_squared();
	int return_roomSize_minus1();
	Vector2D<int> return_cornerPoint();
	bool return_isStartRoom();

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
