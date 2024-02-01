#pragma once
#include <vector>
#include "../../ECS/System.h"
#include "Level.h"
#include "../../Math/Matrix.h"

// manages everyting related to the dungeon 
// no System needed
class DungeonSystem 
{
public:
	void init();
	bool checkCollision(Vector2D<int> wantedPosition);
	//void positionPlayer();

	Matrix return_collisionMap();
	int return_levelEntity();
	int return_tileMapEntity();
	int return_collisionMapEntity();

private:
	Entity levelEntity;
	Entity tileMapEntity;
	Entity collisionMapEntity;
};
