#pragma once
#include "../../ECS/System.h"
#include <SDL3/SDL.h>

class RendererLightSystem : public Systems
{
public:
	void initRenderertex();
	void loadTexture();
	void render();
	
	SDL_Texture* rtnRenderertex();
private:
	SDL_Texture* renderertex_light;
};