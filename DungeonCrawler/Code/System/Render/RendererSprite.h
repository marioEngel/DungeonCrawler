#pragma once
#include "../../ECS/System.h"

class RendererSpriteSystem : public Systems
{
public:
	void loadTexture();

	void renderSprite();
};