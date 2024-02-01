#include <iostream>
#include "Game.h"

Game* game = nullptr;

float gDeltaT{};
int gFrameNumber = 0;

int main(int argc, char* arvg[])
{
	const int FPS = 60;
	const int FrameDelay = 1000 / 60;
	Uint32 frameStart;
	Uint32 frameTime;

	game = new Game();
	game->init("Test", 800, 600, 0);
	game->initEntities();

	while (game->gameRunning())
	{

		gFrameNumber++;
		frameStart = SDL_GetTicks();

		game->update();
		game->handleEvents();

		frameTime = SDL_GetTicks() - frameStart;
		gDeltaT = float(frameTime / 1000.0f);

		//std::cout << 1.0f / (gDeltaT) << std::endl;
		//if (FrameDelay > frameTime)
		//{
		//	SDL_Delay(FrameDelay - frameTime);
		//}		
	}

	game->clean();

	return 0;
}
