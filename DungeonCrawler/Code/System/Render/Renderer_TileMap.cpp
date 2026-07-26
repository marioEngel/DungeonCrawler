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
		SDL_SetTextureBlendMode(renderertex_map, SDL_BLENDMODE_BLEND);
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

void RendererSystem_TileMap::createTileMapRenderertex(bool forceUpdate)
{
	for (auto const& entity : mEntities)
	{
		auto& tileMap = gCoordinator.GetComponent<TileMap>(entity);

		if (tileMap.tileMapTexture == nullptr || forceUpdate)
		{
			
			tileMap.tileMapTexture = SDL_CreateTexture(Game::renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, Game::screenWidth, Game::screenHeight);
			SDL_SetRenderTarget(Game::renderer, tileMap.tileMapTexture);
			SDL_RenderClear(Game::renderer);
			SDL_SetRenderDrawColor(Game::renderer, 255, 255, 255, 0);

			int tileMapSizeY = tileMap.matrix.size();
			int tileMapSizeX = tileMap.matrix[0].size();

			SDL_FRect srcRec{ 0.0f, 0.0f, 32.0f, 32.0f };
			float cubeSize = 2.0f;

			for (int i = 0; i < tileMapSizeY; i++)
			{
				for (int j = 0; j < tileMapSizeX; j++)
				{
					SDL_FRect destRec{ cubeSize * j, cubeSize * i, cubeSize, cubeSize };
					SDL_RenderTexture(Game::renderer, tileMap.textures[tileMap.matrix[i][j]], &srcRec, &destRec);

				}
			}
		}
	}
	SDL_SetRenderTarget(Game::renderer, NULL);
}

void RendererSystem_TileMap::render()
{
	for (auto const& entity : mEntities)
	{
		auto& tileMap = gCoordinator.GetComponent<TileMap>(entity);

		SDL_FRect src{ 0, 0, Game::screenWidth, Game::screenHeight };
		SDL_FRect dst{ -gCamera.mCamera.x, -gCamera.mCamera.y, Game::screenWidth, Game::screenHeight };
		SDL_RenderTexture(Game::renderer, tileMap.tileMapTexture, &src, &dst);

	}
	SDL_SetRenderTarget(Game::renderer, NULL);
}

SDL_Texture* RendererSystem_TileMap::rtnRenderertex()
{
	return renderertex_map;
}