#pragma once
#include "../System/Collision/Geometry/Geometry.h"

struct Hitbox
{
	std::shared_ptr<Geometry> geomHitbox;

	// for testing
	const char* path;
	int textureHeight;
	int textureWidth;
	int scale;
	double angle;

	SDL_Texture* textureSDL = NULL;
};