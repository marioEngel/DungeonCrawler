#include "Dungeon.h"
#include "../../ECS/Coordinator.h"
#include "../../Component/LevelComp.h"
#include "../../Component/TileMapComp.h"
#include <SDL.h>
#include "../../Component/CollisionMapComp.h"
#include "../../Component/MatrixPositionComp.h"

extern Coordinator gCoordinator;

void DungeonSystem::init()
{
	levelEntity = gCoordinator.CreateEntity();
	Level tmpLevel{};
	tmpLevel.createLevel(5);
	Matrix collisionMap = tmpLevel.createCollision();
	//printMatrix(collisionMap);
	Matrix renderMap = tmpLevel.createRender();
	gCoordinator.AddComponent<LevelComp>(levelEntity, LevelComp{tmpLevel});

	tileMapEntity = gCoordinator.CreateEntity();
	std::vector<SDL_Texture*> emptyTexture{};
	std::vector<const char*> tmpTileTextures =
	{
		"Picture/TileNormal.png",
		"Picture/TileGround.png"
	};
	gCoordinator.AddComponent<TileMap>(tileMapEntity, TileMap{renderMap, tmpTileTextures, emptyTexture});

	collisionMapEntity = gCoordinator.CreateEntity();
	gCoordinator.AddComponent<CollisionMap>(collisionMapEntity, CollisionMap{ collisionMap });
}

bool DungeonSystem::checkCollision(Vector2D<int> wantedPosition)
{
	auto& collisionMap = gCoordinator.GetComponent<CollisionMap>(collisionMapEntity);
	return (0 == collisionMap.collisionMap[wantedPosition[0]][wantedPosition[1]]);
}

Matrix DungeonSystem::return_collisionMap()
{
	auto& rtnMatrix = gCoordinator.GetComponent<CollisionMap>(collisionMapEntity);
	return rtnMatrix.collisionMap;
}

int DungeonSystem::return_collisionMapEntity()
{
	return collisionMapEntity;
}

int DungeonSystem::return_tileMapEntity()
{
	return tileMapEntity;
}
int DungeonSystem::return_levelEntity()
{
	return levelEntity;
}

/*
void DungeonSystem::init()
{
	levelEntity = gCoordinator.CreateEntity();
	Level tmpLevel{};
	gCoordinator.AddComponent<LevelComp>(levelEntity, LevelComp{ tmpLevel });
	LevelComp& tmpLevelComp = 
	gCoordinator.GetComponent<LevelComp>(levelEntity);
	tmpLevelComp.levelStruc.createLevel(5);
	collisionMap = tmpLevelComp.levelStruc.createCollision();
	renderMap = tmpLevelComp.levelStruc.createRender();

	printMatrixColor(renderMap);
	printMatrixColor(collisionMap);
}
*/


/* ================= old code ==================================
void LevelSystem::init()
{
	Entity levelEntity = gCoordinator.CreateEntity();
	currentEntity = levelEntity;
	//currentLevel = Level{ 50 };
	currentLevel.createLevel(5);
	//currentLevel.printStuff();
	Matrix tmpMatrix = currentLevel.returnLevelMatrix();
	printMatrixColor(tmpMatrix);
	std::cout << "\n";
	Matrix collisionMatrix = calcCollisionMatrix(tmpMatrix);
	printMatrixColor(collisionMatrix);
	std::vector<const char*> tmpTileTextures =
	{
		"Picture/TileNormal.png",
		"Picture/TileGround.png",
		"Picture/TileGround.png"
	};
	std::vector<int> startPos{ 10, 10 };
	std::vector<SDL_Texture*> emptyTexture{};
	gCoordinator.AddComponent<TileMap>(levelEntity, TileMap{ tmpMatrix, tmpTileTextures, emptyTexture, startPos });
	gCoordinator.AddComponent<CollisionMap>(levelEntity, CollisionMap{ collisionMatrix });
	gCoordinator.AddComponent<Affiliation>(levelEntity, Affiliation{ AffKind::Map });
}

void LevelSystem::init2()
{
	Level2 testLevel = Level2{5};
	testLevel.createLevel(5);
}

bool LevelSystem::checkCollision(int xPos, int yPos)
{
	auto& hitbox = gCoordinator.GetComponent<CollisionMap>(currentEntity);
	if (hitbox.collisionMap[xPos][yPos] == 1)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void LevelSystem::spawnEnemy()
{
	Entity tmpEnemy = gCoordinator.CreateEntity();
	gCoordinator.AddComponent<Position>(tmpEnemy, Position{ 96.0f, 96.0f });
	gCoordinator.AddComponent<Texture>(tmpEnemy, Texture{ "Picture/Ratte.png", 32,32,1 });
}

*/