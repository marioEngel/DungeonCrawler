#pragma once
#include <SDL3/SDL.h>
#include "../../../ECS/ECS.h"
#include "../../../Math/Vector2D.h"

class Camera
{
public:
	Camera();
	~Camera();

	void CheckCollision(Entity player);
	void transformToBaseCoord(Vector2D<float>& cameraCoord);

	SDL_FRect mCamera;
	float mEdgeWidth;
	float mEdgeHight;
};
