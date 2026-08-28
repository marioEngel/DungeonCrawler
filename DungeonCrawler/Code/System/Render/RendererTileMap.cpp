#include "RendererTileMap.h"
#include "../../ECS/Coordinator.h"
#include "Misc/TextureFunc.h"
#include "../../Component/Comp_TileMap.h"
#include "../../Game.h"
#include "../Collision/Collision.h"
#include "Camera/Camera.h"

extern Camera gCamera;
extern Coordinator gCoordinator;

void SysRendererTileMap::initRenderertex()
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
void SysRendererTileMap::loadTexture()
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

void SysRendererTileMap::createTileMapRenderertex(bool forceUpdate)
{
	for (auto const& entity : mEntities)
	{
		auto& tileMap = gCoordinator.GetComponent<TileMap>(entity);

		if (tileMap.tileMapTexture == nullptr || forceUpdate)
		{		
			int tileMapSizeY = tileMap.matrix.rows();
			int tileMapSizeX = tileMap.matrix.cols();
			float cubeSize = 8.0f;
			int texWidth = static_cast<int>(tileMapSizeX * cubeSize);
			int texHeight = static_cast<int>(tileMapSizeY * cubeSize);

			tileMap.dimension = Vector2D<int>{ texWidth, texHeight };

			tileMap.tileMapTexture = SDL_CreateTexture(Game::renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, texWidth, texHeight);
			SDL_SetRenderTarget(Game::renderer, tileMap.tileMapTexture);
			SDL_RenderClear(Game::renderer);
			SDL_SetRenderDrawColor(Game::renderer, 255, 255, 255, 0);

			SDL_FRect srcRec{ 0.0f, 0.0f, 32.0f, 32.0f };

			for (int row = 0; row < tileMapSizeY; row++)
			{
				for (int col = 0; col < tileMapSizeX; col++)
				{
					SDL_FRect destRec{ cubeSize * col, cubeSize * row, cubeSize, cubeSize };
					SDL_RenderTexture(Game::renderer, tileMap.textures[tileMap.matrix(row, col)], &srcRec, &destRec);

				}
			}
		}
	}
	SDL_SetRenderTarget(Game::renderer, NULL);
}

void SysRendererTileMap::render()
{
	for (auto const& entity : mEntities)
	{
		auto& tileMap = gCoordinator.GetComponent<TileMap>(entity);

		SDL_FRect src{ 0, 0, tileMap.dimension[0], tileMap.dimension[1] };
		SDL_FRect dst{ -gCamera.mCamera.x, -gCamera.mCamera.y, tileMap.dimension[0], tileMap.dimension[1] };
		SDL_RenderTexture(Game::renderer, tileMap.tileMapTexture, &src, &dst);

	}
	SDL_SetRenderTarget(Game::renderer, NULL); 
}

SDL_Texture* SysRendererTileMap::rtnRenderertex()
{
	return renderertex_map;
}