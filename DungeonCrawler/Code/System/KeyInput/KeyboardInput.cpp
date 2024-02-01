#include "KeyboardInput.h"
#include "../../Game.h"
#include <SDL.h>

KeyboardInput::KeyboardInput()
{
	memset(prevKeys, 0, SDL_NUM_SCANCODES);
	currKeys = SDL_GetKeyboardState(NULL);
}

void KeyboardInput::registerInput()
{
	SDL_PollEvent(&Game::event);
	currKeys = SDL_GetKeyboardState(NULL);
}

void KeyboardInput::getPrevKeys()
{
	memcpy(prevKeys, currKeys, SDL_NUM_SCANCODES);
}

eButtonState KeyboardInput::getButtonState(int number)
{
	if (prevKeys[number] == 0)
	{
		if (currKeys[number] == 1)
		{
			return eButtonState::PRESSED;
		}
		else
		{
			return eButtonState::NONE;
		}
	}
	else
	{
		if (currKeys[number] == 1)
		{
			return eButtonState::HELD;
		}
		else
		{
			return eButtonState::RELEASED;
		}
	}
}
