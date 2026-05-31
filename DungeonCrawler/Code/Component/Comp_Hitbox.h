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
	int textureHeight;
	int textureWidth;
	int scale;
	double angle;

	SDL_Texture* textureSDL = NULL;
};