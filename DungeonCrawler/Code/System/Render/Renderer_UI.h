#pragma once
#include "../../ECS/System.h"
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

class RendererUISystem : public Systems
{
public:
	void initRenderertex();
	void loadTexture();
	TTF_Font* loadFont();
	void render();

	SDL_Texture* rtnRenderertex();
private:
	SDL_Texture* renderertex_UI;
};