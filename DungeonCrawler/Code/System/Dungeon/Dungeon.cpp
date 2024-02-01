#include "Dungeon.h"
#include "../../ECS/Coordinator.h"
#include <SDL.h>
#include "../../Component/TileMapComp.h"

extern Coordinator gCoordinator;

void DungeonSystem::init()
{

	int tileMapEntity = gCoordinator.CreateEntity();
	std::vector<SDL_Texture*> emptyTexture{};
	std::vector<const char*> tmpTileTextures =
	{
		"Picture/TileNormal.png",
		"Picture/TileGround.png"
	};
	Matrix renderMap;
	makeEmptyMatrix(renderMap, 50, 50);
	gCoordinator.AddComponent<TileMap>(tileMapEntity, TileMap{ renderMap, tmpTileTextures, emptyTexture });


}