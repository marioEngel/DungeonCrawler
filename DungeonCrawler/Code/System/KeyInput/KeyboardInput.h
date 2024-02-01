#pragma once
#include <cstdint>
#include <SDL.h>

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

	uint8_t prevKeys[SDL_NUM_SCANCODES];
	const uint8_t* currKeys;
};

