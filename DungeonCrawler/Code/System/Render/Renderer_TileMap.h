#pragma once
#include "../../ECS/System.h"
#include <SDL.h>

class RendererTileMapSystem : public Systems
{
public:
	void initRenderertex();
	void loadTexture();
	void render();

	SDL_Texture* rtnRenderertex();
private:
	SDL_Texture* renderertex_map;
};

