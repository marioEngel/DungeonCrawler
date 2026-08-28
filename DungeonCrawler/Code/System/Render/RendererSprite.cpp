#include "RendererSprite.h"
#include "../../ECS/Coordinator.h"
#include "../../Component/Comp_Texture.h"
#include "../../Component/Comp_Position.h"
#include "Misc/TextureFunc.h"
#include "../../Game.h"
#include "Camera/Camera.h"
#include <filesystem>
#include "../Collision/Collision.h"

extern Coordinator gCoordinator;
extern Camera gCamera;

void SysRendererSprite::initRenderertex()
{
	if (renderertex_sprite == nullptr)
	{
		renderertex_sprite = SDL_CreateTexture(Game::renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, Game::screenWidth, Game::screenHeight);
		SDL_SetTextureBlendMode(renderertex_sprite, SDL_BLENDMODE_BLEND);
	}
	
	SDL_SetRenderTarget(Game::renderer, renderertex_sprite);
	SDL_RenderClear(Game::renderer);
	SDL_SetRenderDrawColor(Game::renderer, 255, 255, 255, 255);
}

void SysRendererSprite::loadTexture()
{
	for (const auto& entity : mEntities)
	{
		auto& texture = gCoordinator.GetComponent<Texture>(entity);

		if (texture.SDL == nullptr)
		{
			texture.SDL = LoadTexture(texture.path, Game::renderer);
		}
	}
}

void SysRendererSprite::render()
{
	for (const auto& entity : mEntities)
	{
		auto& texture = gCoordinator.GetComponent<Texture>(entity);
		auto& position = gCoordinator.GetComponent<Position>(entity);

		if(texture.SDL != nullptr)
		{
			SDL_FRect srcRec;
			SDL_FRect destRec;

			srcRec.x = 0;
			srcRec.y = 0;
			srcRec.w = texture.width;
			srcRec.h = texture.height;

			destRec.x = position.pos[0];
			destRec.y = position.pos[1];
			destRec.w = texture.width * texture.scale;
			destRec.h = texture.height * texture.scale;

			if (check_RectVsRect(gCamera.mCamera, destRec))
			{
				destRec.x -= gCamera.mCamera.x;
				destRec.y -= gCamera.mCamera.y;
				SDL_RenderTextureRotated(Game::renderer, texture.SDL, &srcRec, &destRec, texture.angle, NULL, SDL_FLIP_NONE);
			}
		}
	}
	SDL_SetRenderTarget(Game::renderer, NULL);
}

SDL_Texture* SysRendererSprite::rtnRenderertex()
{
	return renderertex_sprite;
}