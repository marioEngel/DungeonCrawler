#pragma once
#include "../../ECS/System.h"
#include "MatrixGenerator.h"

class LevelSystem : public Systems
{
public:
	void init();
	void init2();
	bool checkCollision(int xPos, int yPos);
	void spawnEnemy();

private:
	Entity currentEntity;
	Level currentLevel = Level{ 45 };
};