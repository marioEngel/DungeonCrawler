#pragma once
#include "../System/Collision/Geometry/Geometry.h"

enum eCollisionType
{
	PHYSICAL,		// Move object if they collide
	ACTION			// Register action, click on stuff etc.
};

struct Comp_Hitbox
{
	std::shared_ptr<Geometry> geomComp_Hitbox;
	eCollisionType collisionType;

	// for testing
	const char* path;
	int textureHeight;
	int textureWidth;
	int scale;
	double angle;

	SDL_Texture* textureSDL = NULL;
};