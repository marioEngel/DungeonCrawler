#pragma once
#include "../../ECS/System.h"
#include <SDL3/SDL.h>

class SysRenderer : public Systems
{
public:
	virtual void initRenderertex() {};
	virtual void loadTexture() {};
	virtual void render() = 0;
private:

};
