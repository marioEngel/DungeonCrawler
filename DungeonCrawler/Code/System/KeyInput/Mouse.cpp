#include "Mouse.h"
#include <SDL3/SDL.h>
#include "../../Game.h"	

Mouse::Mouse()
{
	mMousePosition = Vector2D<float>{ 0,0 };
}

Mouse::~Mouse()
{
}

void Mouse::update()
{
	SDL_GetMouseState(&mMousePosition[0], &mMousePosition[1]);
}

void Mouse::printMousePosition()
{
	std::cout << "Mouse Position: " << mMousePosition << '\n';
}

void Mouse::printMousePosition_color()
{
	std::cout << "Mouse Position: ("
		<< "\033[1;34m" << mMousePosition.returnXval()
		<< "\033[0m" << ", "
		<< "\033[1;31m" << mMousePosition.returnYval()
		<< "\033[0m" << ")\n";
}

void Mouse::click()
{
	switch (Game::event.type)
	{
	case SDL_EVENT_MOUSE_BUTTON_DOWN:
		if (SDL_GetTicks() - mMouseLastClickTime > mMouseClickBuffer)
		{
			if (Game::event.button.button == SDL_BUTTON_LEFT)
			{
				std::cout << "Left click  \n";
				mMouseLastClickTime = SDL_GetTicks();
			}
			else if (Game::event.button.button == SDL_BUTTON_RIGHT)
			{
				std::cout << "       Right click\n";
				mMouseLastClickTime = SDL_GetTicks();
			}
			else
			{
				std::cout << "weird\n";
				mMouseLastClickTime = SDL_GetTicks();
			}
		}
	default:
		break;
	}
}