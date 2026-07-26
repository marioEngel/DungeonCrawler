#pragma once
#include "Render.h"
#include <SDL3/SDL.h>

class SysRendererSprite : public SysRenderer
{
public:
	void initRenderertex() override;
	void loadTexture() override;
	void render() override;

	SDL_Texture* rtnRenderertex();
private:
	SDL_Texture* renderertex_sprite;
};