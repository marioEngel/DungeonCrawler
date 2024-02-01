#pragma once
#include <SDL.h>
#include <string>
#include "ECS/Coordinator.h"

class Game
{
public:
	Game();
	~Game();

	bool gameRunning();
	void clean();
	void init(const char* text, int width, int hight, int flag = 0);
	void makeEnd();

	void initEntities();
	void update();
	void handleEvents();

	static SDL_Renderer* renderer;
	static SDL_Event event;

private:
	bool isRunning = true;
	SDL_Window* window;
};
