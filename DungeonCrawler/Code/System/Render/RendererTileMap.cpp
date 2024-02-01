#include "RendererTileMap.h"
#include "../../ECS/Coordinator.h"
#include "TextureFunc.h"
#include "../../Component/TileMapComp.h"
#include "../../Game.h"
#include "../Collision/Collision.h"
#include "Camera.h"

extern Camera gCamera;
extern Coordinator gCoordinator;

// load the textures once
void RendererTileMapSystem::loadTexture()
{
	for (auto const& entity : mEntities)
	{
		auto& tileMap = gCoordinator.GetComponent<TileMap>(entity);

		if (tileMap.textures.empty())
		{
			for (int i = 0; i < tileMap.paths.size(); i++)
			{
				tileMap.textures.emplace_back(LoadTexture(tileMap.paths[i]));
			}
		}
	}
}


void RendererTileMapSystem::renderTileMap()
{
	for (auto const& entity : mEntities)
	{

		auto& tileMap = gCoordinator.GetComponent<TileMap>(entity);

		for (int i = 0; i < tileMap.matrix.size(); i++)
		{
			for (int j = 0; j < tileMap.matrix[i].size(); j++)
			{
				SDL_Rect srcRec;
				SDL_Rect destRec;

				srcRec.x = 0;
				srcRec.y = 0;
				srcRec.w = 32;
				srcRec.h = 32;

				destRec.x = 32 * j;
				destRec.y = 32 * i;
				destRec.w = 32;
				destRec.h = 32;

				if (check_RectVsRect(gCamera.mCamera, destRec))
				{
					destRec.x -= gCamera.mCamera.x;
					destRec.y -= gCamera.mCamera.y;
					SDL_RenderCopy(Game::renderer, tileMap.textures[tileMap.matrix[i][j]], &srcRec, &destRec);
				}
			}
		}
	}
}