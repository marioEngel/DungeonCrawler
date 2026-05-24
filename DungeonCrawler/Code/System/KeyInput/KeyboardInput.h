#pragma once
#include <cstdint>
#include <SDL3/SDL.h>

enum eButtonState
{
	NONE,
	PRESSED,
	RELEASED,
	HELD,
};

class KeyboardInput
{
public:
	KeyboardInput();
	void registerInput();
	void getPrevKeys();
	eButtonState getButtonState(int number);

	bool prevKeys[SDL_SCANCODE_COUNT];
	const bool* currKeys;
};

