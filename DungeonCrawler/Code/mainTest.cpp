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
	Matrix<int> tmp{2, 4};
	SDL_Rect test{ 0, 0, 11, 23 };

	matrixPrint(tmp);



	return 0;
}
