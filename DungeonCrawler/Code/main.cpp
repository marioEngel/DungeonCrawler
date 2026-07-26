#include <iostream>
#include "Game.h"
#include <fstream>

Game* game = nullptr;

float gDeltaT{};

int main(int argc, char* arvg[])
{
	const Uint64 FPS = 144;
	const Uint64 FrameDelay = 1'000'000'000ULL / FPS;	// ns pro Frame

	game = new Game();
	game->initECS("Test", 800, 600, 0);
	game->initEntities();

	Uint64 lastTime = SDL_GetTicksNS();

	while (game->gameRunning())
	{
		Uint64 frameStart = SDL_GetTicksNS();

		game->handleEvents();
		game->update(gDeltaT);

		Uint64 frameTime = SDL_GetTicksNS() - frameStart;
		if (FrameDelay > frameTime)
		{
			SDL_DelayNS((Uint32)(FrameDelay - frameTime));
		}		
		Uint64 now = SDL_GetTicksNS();
		gDeltaT = (now - lastTime) / 1'000'000'000.0f;
		if(gDeltaT > 0.25f) gDeltaT = 0.25f;
		lastTime = now;
	}

	game->clean();
	return 0;
}
