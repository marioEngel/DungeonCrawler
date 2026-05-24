#pragma once
#include <SDL3/SDL.h>
#include <functional>

struct Action
{
	SDL_Scancode key;
	std::function<void(void*)> actionFunction;
	void* params;
};