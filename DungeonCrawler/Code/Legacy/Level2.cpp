#pragma once
#include "Level.h"
#include "../../ECS/Coordinator.h"
#include <vector>
#include <SDL_image.h>
#include "../../Component/TileMapComp.h"
#include "../../Math/Matrix.h"
#include "../../Component/CollisionMapComp.h"
#include "../../Component/PositionComp.h"
#include "../../Component/AffiliationComp.h"
#include "../../Component/TextureComp.h"

extern Coordinator gCoordinator;

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
