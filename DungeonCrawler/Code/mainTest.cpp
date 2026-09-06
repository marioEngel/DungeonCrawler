#include <iostream>
#include "Game.h"
#include "Math/Vector2D.h"
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3/SDL.h>
#include "System/Dungeon/Tower/Floor.h"
#include "Math/Matrix.h"

float gDeltaT{};

int main(int argc, char* arvg[])
{

	Floor floor;
	floor.init();
	floor.generate();



	return 0;
}
