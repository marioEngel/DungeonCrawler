#pragma once
#include "Render.h"
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

class SysRendererUI : public SysRenderer
{
public:
	void initRenderertex() override;
	void loadTexture() override;
	void render() override;

	TTF_Font* loadFont();
	SDL_Texture* rtnRenderertex();
private:
	SDL_Texture* renderertex_UI;
};