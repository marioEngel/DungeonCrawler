#pragma once
#include <SDL.h>
#include <vector>

// simple Texture
struct Texture
{
	const char* path;
	int textureHeight;
	int textureWidth;
	int scale;
	double angle;

	SDL_Texture* textureSDL = NULL;
};


// more complicated Texture
/*
struct Texture
{
	const char* path;									// path from game.cpp
	int textureHeight;									// self explanatory
	int textureWidth;									// self explanatory
	int scale;											// self explanatory
	std::vector<std::pair<int, int>> frameNumberTime;	// how many frame in a sheet with 
														//  the frame time
	int frameSheet = 0;									// start at the top of framesheet
	int flipXoffset = 0;								// offset if picture is flipped

	// not initionalized
	SDL_RendererFlip renderFlip = SDL_FLIP_NONE;		// flipeed?
	int tmpFrameNumber = 0;								// framesheet from left to right 
	int time = 0;										// time for animation
	SDL_Texture* textureSDL = NULL;						// texture 
	bool animationLock = false;							// cant change framesheet if true
};
*/