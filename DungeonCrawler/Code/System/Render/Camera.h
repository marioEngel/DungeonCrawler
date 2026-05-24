#pragma once
#include <SDL3/SDL.h>
#include "../../ECS/ECS.h"

class Camera
{
public:
	Camera();
	//Camera(int width, int height);
	//Camera(int width, int height, int xpos, int ypos);
	~Camera();

	void CheckCollision(Entity player);

	SDL_FRect mCamera;
	SDL_FRect mCameraEdge;
};
