#include "RendererSprite.h"
#include "../../ECS/Coordinator.h"
#include "../../Component/TextureComp.h"
#include "../../Component/PositionComp.h"
#include "TextureFunc.h"
#include "../../Game.h"
#include "Camera.h"
#include <filesystem>
#include "../Collision/Collision.h"

extern Coordinator gCoordinator;
extern Camera gCamera;

void RendererSpriteSystem::loadTexture()
{
	for (auto& const entity : mEntities)
	{
		auto& texture = gCoordinator.GetComponent<Texture>(entity);

		if (texture.textureSDL == nullptr)
		{
			texture.textureSDL = LoadTexture(texture.path);
		}
	}
}

void RendererSpriteSystem::renderSprite()
{
	for (auto& const entity : mEntities)
	{
		auto& texture = gCoordinator.GetComponent<Texture>(entity);
		auto& position = gCoordinator.GetComponent<Position>(entity);

		if(texture.textureSDL != nullptr)	
		{
			SDL_Rect srcRec;
			SDL_Rect destRec;

			srcRec.x = 0;
			srcRec.y = 0;
			srcRec.w = texture.textureWidth;
			srcRec.h = texture.textureHeight;

			destRec.x = int(position.pos[0]) - texture.textureWidth/2;
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
}

