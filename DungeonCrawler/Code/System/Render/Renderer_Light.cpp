#include "Renderer_Light.h"
#include "../../ECS/Coordinator.h"
#include "../../Component/TextureLightComp.h"
#include "../../Component/PositionComp.h"
#include "TextureFunc.h"
#include "../../Game.h"
#include "Camera.h"
#include <filesystem>
#include "../Collision/Collision.h"
#include "../../Misc/MistFunctions.h"

extern Coordinator gCoordinator;
extern Camera gCamera;

void RendererLightSystem::initRenderertex()
{
	if (renderertex_light == nullptr)
	{
		renderertex_light = SDL_CreateTexture(Game::renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, Game::screenWidth, Game::screenHeight);
	}
	SDL_SetRenderTarget(Game::renderer, renderertex_light);
	SDL_RenderClear(Game::renderer);
	SDL_SetRenderDrawColor(Game::renderer, 0, 0, 0, 255);
}


void RendererLightSystem::loadTexture()
{
	for (auto& const entity : mEntities)
	{
		auto& texture = gCoordinator.GetComponent<TextureLight>(entity);

		if (texture.textureSDL == nullptr)
		{
			texture.textureSDL = LoadTexture(texture.path, Game::renderer);
		}
	}
}

void RendererLightSystem::render()
{
	for (auto& const entity : mEntities)
	{	
		auto& texture = gCoordinator.GetComponent<TextureLight>(entity);
		auto& position = gCoordinator.GetComponent<Position>(entity);

		SDL_RenderClear(Game::renderer);
		SDL_SetRenderTarget(Game::renderer, renderertex_light);

		if (texture.textureSDL != nullptr)
		{
			SDL_Rect srcRec;
			SDL_Rect destRec;

			srcRec.x = 0;
			srcRec.y = 0;
			srcRec.w = texture.textureWidth;
			srcRec.h = texture.textureHeight;

			destRec.x = int(position.pos[0]);
			destRec.y = int(position.pos[1]);
			destRec.w = texture.textureWidth * texture.scale;
			destRec.h = texture.textureHeight * texture.scale;

			if (check_RectVsRect(gCamera.mCamera, destRec))
			{
				destRec.x -= gCamera.mCamera.x;
				destRec.y -= gCamera.mCamera.y;
				SDL_RenderCopyEx(Game::renderer, texture.textureSDL, &srcRec, &destRec, texture.angle, NULL, SDL_FLIP_NONE);
			}
		}
	}
	SDL_SetRenderTarget(Game::renderer, NULL);
}

SDL_Texture* RendererLightSystem::rtnRenderertex()
{
	return renderertex_light;
}
