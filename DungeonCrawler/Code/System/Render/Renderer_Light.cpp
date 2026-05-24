#include "Renderer_Light.h"
#include "../../ECS/Coordinator.h"
#include "../../Component/Comp_TextureLight.h"
#include "../../Component/Comp_Position.h"
#include "TextureFunc.h"
#include "../../Game.h"
#include "Camera.h"
#include <filesystem>
#include "../Collision/Collision.h"
#include "../../Misc/MiscFunctions.h"
#include "Renderer_Misc.h"


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

		if (texture.SDL == nullptr)
		{	
			texture.SDL = my_apply(texture.function, std::tuple_cat(std::make_tuple(Game::renderer), texture.functionParams));
		}
	}
}

void RendererLightSystem::render()
{
	SDL_RenderClear(Game::renderer);
	for (auto& const entity : mEntities)
	{	
		auto& texture = gCoordinator.GetComponent<TextureLight>(entity);
		auto& position = gCoordinator.GetComponent<Position>(entity);

		if (texture.SDL != nullptr)
		{
			SDL_FRect srcRec;
			SDL_FRect destRec;

			srcRec.x = 0;
			srcRec.y = 0;
			srcRec.w = texture.width;
			srcRec.h = texture.height;

			destRec.x = position.pos[0];
			destRec.y = position.pos[1];
			destRec.w = texture.width;
			destRec.h = texture.height;

			if (check_RectVsRect(gCamera.mCamera, destRec))
			{
				destRec.x -= gCamera.mCamera.x;
				destRec.y -= gCamera.mCamera.y;
				SDL_SetTextureBlendMode(texture.SDL, SDL_BLENDMODE_ADD);
				SDL_SetTextureColorMod(texture.SDL, texture.colorRGB.r, texture.colorRGB.g, texture.colorRGB.b);
				 SDL_RenderTextureRotated(Game::renderer, texture.SDL, &srcRec, &destRec, texture.angle, NULL, SDL_FLIP_NONE);
			}
		}
	}
	SDL_SetRenderTarget(Game::renderer, NULL);
}

SDL_Texture* RendererLightSystem::rtnRenderertex()
{
	return renderertex_light;
}
