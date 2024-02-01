#pragma once
#include "../../ECS/System.h"

class RendererTileMapSystem : public Systems
{
public:
	void loadTexture();

	void renderTileMap();
};

