#include <iostream>
#include "Game.h"
#include <fstream>

Game* game = nullptr;

float gDeltaT{};
int gFrameNumber = 0;

// some comment
int main(int argc, char* arvg[])
{
	const int FPS = 60;
	const int FrameDelay = 1000 / FPS;
	Uint32 frameStart;
	Uint32 frameTime;

	game = new Game();
	game->init("Test", 800, 600, 0);
	game->initEntities();

	//std::ofstream outFile("C:/Users/Mario/Desktop/GameDev/directionVals.txt", std::ios::out | std::ios::trunc);
	//if (outFile.is_open()) 
	//{
	//	outFile.close();  // Close the file after truncating
	//}

	while (game->gameRunning())
	{

		gFrameNumber++;
		frameStart = SDL_GetTicks();

		game->update();
		game->handleEvents();

		frameTime = SDL_GetTicks() - frameStart;
		gDeltaT = float(frameTime / 1000.0f);

		//astd::cout << 1.0f / (gDeltaT) << std::endl;
		//if (FrameDelay > frameTime)
		//{
		//	SDL_Delay(FrameDelay - frameTime);
		//}		
	}

	game->clean();

	return 0;
}
