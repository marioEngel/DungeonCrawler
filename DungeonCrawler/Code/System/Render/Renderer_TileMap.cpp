#include "Renderer_TileMap.h"
#include "../../ECS/Coordinator.h"
#include "TextureFunc.h"
#include "../../Component/Comp_TileMap.h"
#include "../../Game.h"
#include "../Collision/Collision.h"
#include "Camera.h"

extern Camera gCamera;
extern Coordinator gCoordinator;

void RendererSystem_TileMap::initRenderertex()
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
void RendererSystem_TileMap::loadTexture()
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


void RendererSystem_TileMap::render()
{
	for (auto const& entity : mEntities)
	{

		auto& tileMap = gCoordinator.GetComponent<TileMap>(entity);

		for (int i = 0; i < tileMap.matrix.size(); i++)
		{
			for (int j = 0; j < tileMap.matrix[i].size(); j++)
			{
				SDL_FRect srcRec;
				SDL_FRect destRec;

				srcRec.x = 0;
				srcRec.y = 0;
				srcRec.w = 32.0f;
				srcRec.h = 32.0f;

				float cubeSize = 8.0f;

				destRec.x = cubeSize * j;
				destRec.y = cubeSize * i;
				destRec.w = cubeSize;
				destRec.h = cubeSize;

				if (check_RectVsRect(gCamera.mCamera, destRec))
				{
					destRec.x -= gCamera.mCamera.x;
					destRec.y -= gCamera.mCamera.y;
					SDL_RenderTexture(Game::renderer, tileMap.textures[tileMap.matrix[i][j]], &srcRec, &destRec);
				}
			}
		}
	}
	SDL_SetRenderTarget(Game::renderer, NULL);
}

SDL_Texture* RendererSystem_TileMap::rtnRenderertex()
{
	return renderertex_map;
}