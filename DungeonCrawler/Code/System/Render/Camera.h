#pragma once
#include <SDL.h>
#include "../../ECS/ECS.h"

class Camera
{
public:
	Camera();
	//Camera(int width, int height);
	//Camera(int width, int height, int xpos, int ypos);
	~Camera();

	void CheckCollision(Entity player);

	SDL_Rect mCamera;
	SDL_Rect mCameraEdge;
};
