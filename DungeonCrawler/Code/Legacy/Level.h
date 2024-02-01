#pragma once
#include <vector>
#include "Room.h"
#include "../../Math/Matrix.h"

/* Should replace the Level folder
 * Dungeon (the whole thing) -> Level (one floor) -> Room (a room)
 * testing still in Level folder
 */

 /* ===== Level =====
  * the class creates a floor/level of the dungeon
  */
class Level
{
public:
	Level();
	~Level();

	void createLevel(int roomCount);
	
	Matrix createCollision();
	Matrix createRender();
	Room& return_StartingRoom();

private:
	std::vector<Room> roomVector;
	int levelSize;
};