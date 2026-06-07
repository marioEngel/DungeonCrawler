#pragma once
#include "../System/Collision/Geometry/Geometry.h"

enum eCollisionType
{
	PHYSICAL,		// Move object if they collide
	ACTION			// Register action, click on stuff etc.
};

struct Hitbox
{
	std::shared_ptr<Geometry> geomHitbox;
	eCollisionType collisionType;

	// for testing
	const char* path;
	float width = 32.0f;
	float height = 32.0f;
	float scale = 1.0f;
	double angle = 0.0f;

	SDL_Texture* textureSDL = NULL;
};