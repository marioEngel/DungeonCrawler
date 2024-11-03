#include "MistFunctions.h"


void printMousePosition()
{
	Vector2D<int> tmpMouseInt;
	SDL_GetMouseState(&tmpMouseInt[0], &tmpMouseInt[1]);
	Vector2D<float> tmpMouse{ float(tmpMouseInt[0]), float(tmpMouseInt[1]) };
	std::cout << tmpMouse << '\n';
}

SDL_Rect rtnScreenRect()
{
	return SDL_Rect{ 0, 0, Game::screenWidth, Game::screenHeight };
}

Vector2D<float> rtnCenter(Position& position, Texture& texture)
{
	return position.pos + Vector2D<float>{float(texture.textureWidth) / 2.0f, float(texture.textureHeight) / 2.0f};
}

Vector2D<float> rtnCenter(Position& position, TextureLight& texture)
{
	return position.pos + Vector2D<float>{float(texture.textureWidth) / 2.0f, float(texture.textureHeight) / 2.0f};
}