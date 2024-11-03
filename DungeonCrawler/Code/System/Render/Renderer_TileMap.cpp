#include "Renderer_TileMap.h"
#include "../../ECS/Coordinator.h"
#include "TextureFunc.h"
#include "../../Component/TileMapComp.h"
#include "../../Game.h"
#include "../Collision/Collision.h"
#include "Camera.h"

extern Camera gCamera;
extern Coordinator gCoordinator;

void RendererTileMapSystem::initRenderertex()
{ 
	if (renderertex_map == nullptr)
	{
		renderertex_map	= SDL_CreateTexture(Game::renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, Game::screenWidth, Game::screenHeight);
	}
	SDL_SetRenderTarget(Game::renderer, renderertex_map);
	SDL_RenderClear(Game::renderer);
	SDL_SetRenderDrawColor(Game::renderer, 255, 255, 255, 0);
}


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
				tileMap.textures.emplace_back(LoadTexture(tileMap.paths[i], Game::renderer));
			}
		}
	}
}


void RendererTileMapSystem::render()
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
	SDL_SetRenderTarget(Game::renderer, NULL);
}

SDL_Texture* RendererTileMapSystem::rtnRenderertex()
{
	return renderertex_map;
}