#include "Room.h"

// =============== Room stuff ====================
Room::Room(int xCornerPoint, int yCornerPoint, int size)
{
	cornerPoint = Vector2D<int>{ xCornerPoint, yCornerPoint };
	roomSize = size;
	isStartRoom = false;
	isEndRoom = false;
}

Room::Room(Vector2D<int> inputPoints, int size)
{
	cornerPoint = inputPoints;
	roomSize = size;
	isStartRoom = false;
	isEndRoom = false;
}

Room::~Room()
{
}

int Room::return_roomSize()
{
	return roomSize;
}

int Room::return_roomSize_squared()
{
	return roomSize * roomSize;
}

int Room::return_roomSize_minus1()
{
	return roomSize - 1;
}

Vector2D<int> Room::return_cornerPoint()
{
	return cornerPoint;
}

void Room::change_cornerPoint(Vector2D<int> shiftVector)
{
	cornerPoint = cornerPoint - shiftVector;
}

void Room::change_cornerPoint(int xShift, int yShift)
{
	cornerPoint = cornerPoint - Vector2D<int>{xShift, yShift};
}

void Room::makeStart(bool value)
{
	isStartRoom = value;
}

void Room::makeEnd(bool value)
{
	isEndRoom = value;
}

bool Room::return_isStartRoom()
{
	return isStartRoom;
}