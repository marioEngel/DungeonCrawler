#pragma once
#include "Render.h"
#include <SDL3/SDL.h>

class RendererSystem_TileMap : public RendererSystem
{
public:
	void initRenderertex() override;
	void loadTexture() override;
	void render() override;

	void createTileMapRenderertex(bool forceUpdate = false);

	SDL_Texture* rtnRenderertex();
private:
	SDL_Texture* renderertex_map;
};

