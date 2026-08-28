#include "RendererUI.h"
#include "../../ECS/Coordinator.h"
#include "Camera/Camera.h"
#include "../../Game.h"
#include "../../Component/Comp_Texture.h"
#include "../../Component/Comp_Position.h"
#include "Misc/TextureFunc.h"
#include "../Collision/Collision.h"
#include "SDL3_ttf/SDL_ttf.h"
#include "../../Component/Comp_Text.h"

extern Coordinator gCoordinator;
extern Camera gCamera;

void SysRendererUI::initRenderertex()
{
	if (renderertex_UI == nullptr)
	{
		renderertex_UI = SDL_CreateTexture(Game::renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, Game::screenWidth, Game::screenHeight);
		SDL_SetTextureBlendMode(renderertex_UI, SDL_BLENDMODE_BLEND);
	} 

	SDL_SetRenderTarget(Game::renderer, renderertex_UI);
	SDL_SetRenderDrawColor(Game::renderer, 0, 0, 0, 0);
	SDL_RenderClear(Game::renderer);
}

void SysRendererUI::loadTexture()
{
	for (const auto& entity : mEntities)
	{
		auto& texture = gCoordinator.GetComponent<Text>(entity);

		if (texture.SDL == nullptr || texture.reloadTexture)
		{
			texture.reloadTexture = false;
			if (texture.font == nullptr)
			{
				texture.font = loadFont();
			}

			texture.SDL = LoadTextureText(texture.font, texture.text, Game::renderer);
			
			texture.width = texture.SDL->w;
			texture.height = texture.SDL->h;
		}
	}
}

TTF_Font* SysRendererUI::loadFont()
{
	std::string fontPath = "Font/joystix/joystix_monospace.otf";
	//std::string fontPath = "Font/08-true-type-fonts/lazy.ttf";
	TTF_Font*  rtnFont = TTF_OpenFont(fontPath.c_str(), 32);
	if (rtnFont == nullptr)
	{
		SDL_Log("Could not load %s! SDL_ttf Error: %s\n", fontPath.c_str(), SDL_GetError());
	}
	return rtnFont;
}

void SysRendererUI::render()
{
	for (const auto& entity : mEntities)
	{
		auto& texture = gCoordinator.GetComponent<Text>(entity);
		auto& position = gCoordinator.GetComponent<Position>(entity);


		if (texture.SDL != nullptr)
		{
			SDL_SetTextureColorMod(
				texture.SDL,
				texture.textColor.r, 
				texture.textColor.g, 
				texture.textColor.b);
			
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

			SDL_RenderTextureRotated(Game::renderer, texture.SDL, &srcRec, &destRec, 0.0f, NULL, SDL_FLIP_NONE);
		}
	}
	SDL_SetRenderTarget(Game::renderer, NULL);
}

SDL_Texture* SysRendererUI::rtnRenderertex()
{
	return renderertex_UI;
}