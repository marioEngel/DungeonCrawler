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

ColorValues gAmbientLight{ 10, 10, 10 };
SDL_BlendMode SDL_BLENDMODE_LIGHT = SDL_ComposeCustomBlendMode(
	SDL_BLENDFACTOR_ONE,                 // src * 1
	SDL_BLENDFACTOR_ONE_MINUS_SRC_COLOR, // dst * (1 - src)
	SDL_BLENDOPERATION_ADD,              // add them together
	SDL_BLENDFACTOR_ONE,                 // alpha src
	SDL_BLENDFACTOR_ONE_MINUS_SRC_ALPHA, // alpha dst
	SDL_BLENDOPERATION_ADD
);

void RendererSystem_Light::initRenderertex()
{
	if (renderertex_light == nullptr)
	{
		renderertex_light = SDL_CreateTexture(Game::renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, Game::screenWidth, Game::screenHeight);
	}
	SDL_SetRenderTarget(Game::renderer, renderertex_light);
	SDL_RenderClear(Game::renderer);
	SDL_SetRenderDrawColor(Game::renderer, gAmbientLight.r, gAmbientLight.g, gAmbientLight.b, 255);
}


void RendererSystem_Light::loadTexture()
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

void RendererSystem_Light::render()
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

			float scale = 1.0f;

			if (texture.hasNoise)
			{
				float t = SDL_GetTicks() * texture.flickerSpeed;
				float flicker = std::sin(t * 7.0f) * 0.1f + std::sin(t * 17.0f) * 0.05f + std::sin(t * 31.0f) * 0.02f;
				flicker *= texture.flickerAmount;

				scale += flicker;
				destRec.x += std::sin(t * 13.0f) * 2.0f;
				destRec.y += std::sin(t * 11.0f) * 1.0f;
			}

			if (check_RectVsRect(gCamera.mCamera, destRec))
			{
				destRec.x -= gCamera.mCamera.x;
				destRec.y -= gCamera.mCamera.y;
				SDL_SetTextureBlendMode(texture.SDL, SDL_BLENDMODE_LIGHT);
				SDL_SetTextureColorMod(texture.SDL,
					clamp((int)(texture.colorRGB.r * scale), 0, 255),
					clamp((int)(texture.colorRGB.g * scale), 0, 255),
					clamp((int)(texture.colorRGB.b * scale), 0, 255)
				);
				 SDL_RenderTextureRotated(Game::renderer, texture.SDL, &srcRec, &destRec, texture.angle, NULL, SDL_FLIP_NONE);
			}
		}
	}
	SDL_SetRenderTarget(Game::renderer, NULL);
}

SDL_Texture* RendererSystem_Light::rtnRenderertex()
{
	return renderertex_light;
}
